#include <algorithm>
#include <string_view>
#include <cstdio>
#include <span>

#include <utils/macros.hpp>
#include <utils/debug.hpp>
#include <hardware/x86/assembly.hpp>
#include <hardware/ibm/pic8259.hpp>
#include <hardware/ibm/rtccmos.hpp>

#include <netboot/keyboard.hpp>
#include <netboot/interrupts.hpp>

#include <textio/logger.hpp>
#include <textio/format/helpers/repeat_value.hpp>

extern "C" const x86arch::Xdtr_t G_idtr;

static std::uint16_t G_save_mask = 0;

static inline constexpr std::string_view G_exception_string [] = 
{
  "Divide-by-zero",
  "Debug",
  "Non-maskable Interrupt",
  "Breakpoint",
  "Overflow",
  "Bound Range Exceeded",
  "Invalid Opcode",
  "Device Not Available",
  "Double Fault",
  "Coprocessor Segment Overrun",
  "Invalid TSS",
  "Segment Not Present",
  "Stack-Segment Fault",
  "General Protection Fault",
  "Page Fault",
  "",
  "x87 Floating-Point Exception",
  "Alignment Check",
  "Machine Check",
  "SIMD Floating-Point Exception",
  "Virtualization Exception",
  "Control Protection Exception",
  "",
  "",
  "",
  "",
  "",
  "",
  "Hypervisor Injection Exception",
  "VMM Communication Exception",
  "Security Exception",
  ""
};

static constexpr auto GDT_base(std::uint64_t entry) noexcept -> std::uint32_t
{
  const auto base0 = ((entry >> 16u) & 0xffffffu) ;
  const auto base1 = ((entry >> 56u) & 0xffu);
  return base0 | (base1 << 24u);
}

static constexpr auto GDT_limit(std::uint64_t entry) noexcept -> std::uint32_t
{
  const auto limit0 = ((entry >> 0u) & 0xffffu);
  const auto limit1 = ((entry >> 48u) & 0xfu);
  const auto limit = limit0 | (limit1 << 16u);
  const auto is_4k = (entry >> 55u) & 1u;
  return ((limit + 1u) * (is_4k ? 4096u : 1u)) - 1u;
}

static constexpr auto GDT_dpl(std::uint64_t entry) noexcept -> std::uint8_t
{
  return (entry >> 45u) & 0x3u;
}

static constexpr auto GDT_type_s(std::uint64_t entry) noexcept 
  -> std::string_view
{
  const bool is_32bit = (entry >> 54u) & 1u;
  const bool type_bit = (entry >> 44u) & 1u;
  const bool exec_bit = (entry >> 43u) & 1u;
  const bool dico_bit = (entry >> 42u) & 1u;

  if (type_bit)
  {
    if (exec_bit)
      return is_32bit ? "CS32" : "CS16";
    else
      if (dico_bit)
        return is_32bit ? "SS32" : "SS16";
      else
        return is_32bit ? "DS32" : "DS16";
  }
  else 
  {
    return "SYSEG";
  }
}

static auto ISR_segment_info(std::string_view register_v, std::span<std::uint64_t> gdt_s, std::uint16_t selector_v)
{
  const auto descriptor_v = gdt_s[selector_v >> 3u];
  Glog.info<" {:<3s}={:02x} {:08x} {:08x} {:s} DPL={:d}">(
    register_v, 
    selector_v, 
    GDT_base   (descriptor_v), 
    GDT_limit  (descriptor_v), 
    GDT_type_s (descriptor_v), 
    GDT_dpl    (descriptor_v)
  );
}

static auto ISR_eflags_info(std::uint32_t value_v)
{
  Glog.info<"{}{}{}{}">();
}

static auto ISR_display_crash_info(interrupts::stack_frame const& state) -> void
{
  using namespace textio::fmt::helpers;

  const auto gdt_s = x86arch::sgdt();

  Glog.info(repeat_value<79>('*'));
  Glog.info<"EXCEPTION #{:02x} ({:s})">(state.which, G_exception_string[state.which]);
  Glog.info(repeat_value<79>('*'));

  Glog.info<" EAX={:08x} EBX={:08x} ECX={:08x} EDX={:08x}">(state.eax, state.ebx, state.ecx, state.edx);
  Glog.info<" ESI={:08x} EDI={:08x} EBP={:08x} ESP={:08x}">(state.esi, state.edi, state.ebp, state.esp);  
  Glog.info<" EIP={:08x} EFL={:08x}">(state.eip, state.eflags);
 
  ISR_segment_info("CS", gdt_s, state.cs);
  ISR_segment_info("DS", gdt_s, state.ds);
  ISR_segment_info("ES", gdt_s, state.es);
  ISR_segment_info("FS", gdt_s, state.fs);
  ISR_segment_info("GS", gdt_s, state.gs);
  ISR_segment_info("SS", gdt_s, state.ss);

  Glog.info(repeat_value<79>('*')); 
}

static auto ISR_dispatch(const std::uint8_t IRQ_num) -> int
{  
  switch(IRQ_num)
  {
  case 0x00: /* timer::irq();           */break;
  case 0x01: keyboard::irq();             break;
  case 0x02: /*                         */break;
  case 0x03: /* serial::irq2();         */break;
  case 0x04: /* serial::irq1();         */break;
  case 0x05: /* parallel::irq2();       */break;
  case 0x06: /* floppy::irq();          */break;
  case 0x07: /* parallel::irq1();       */break;
  case 0x08: /* clock::irq();           */break;
  case 0x09: /*                         */break;
  case 0x0a: /*                         */break;
  case 0x0b: /*                         */break;
  case 0x0c: /* mouse::irq();           */break;
  case 0x0d: /* coprocessor::irq();     */break;
  case 0x0e: /* ide_ata::irq1();        */break;
  case 0x0f: /* ide_ata::irq2();        */break;
  }
  pic8259::end_of_interrupt(IRQ_num);
  return 0;
}

CO_PUBLIC 
int ISR_handler(interrupts::stack_frame& state)
{ 
  if (state.which < 32) 
  {
    ISR_display_crash_info(state);    
    // Invoke bochs debugger
    __debugbreak(); 
    // Enabele IRQs so we can CTRL+ALT+DEL 
    x86arch::sti(); 
    std::abort();
  }
  return ISR_dispatch(state.which - 0x20u);
}

auto interrupts::default_interrupt_mask() -> std::uint16_t
{
  return G_save_mask;
}

void interrupts::initialize([[maybe_unused]] bool first_time)
{ 
  x86arch::cli(); 
  
  x86arch::lidt(G_idtr);  
  pic8259::switch_to_prot_mode();
  G_save_mask = pic8259::read_mask();
  pic8259::write_mask(0);
  x86arch::sti();
}
          
void interrupts::finalize([[maybe_unused]] bool last_time)
{ 
  x86arch::cli();
  pic8259::write_mask(G_save_mask);
  pic8259::switch_to_real_mode();
  x86arch::lidt({ .limit = 0x400, .base = nullptr });
}
