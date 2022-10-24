#include <string_view>
#include <cstdio>

#include <utils/macros.hpp>
#include <utils/debug.hpp>
#include <hardware/x86assembly.hpp>
#include <hardware/pic8259.hpp>
#include <hardware/rtccmos.hpp>

#include <netboot/keyboard.hpp>
#include <netboot/interrupts.hpp>

#include <textio/format.hpp>
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

static auto ISR_display_crash_info(interrupts::stack_frame const& state) -> void
{
  using namespace textio::fmt;
  using namespace textio::fmt::helpers;

  format_to<"{}\n">(stdout, repeat_value<79>('-'));
  format_to<"Exception #{:08x} - {:s} has occured.\n">(stdout, state.which, G_exception_string[state.which]);
  format_to<"{}\n">(stdout, repeat_value<79>('-'));
  format_to<"cs:eip={:#04x}:{:#08x} fs={:#04x} gs={:#04x}\n">(stdout, state.cs, state.eip, state.fs, state.gs);
  format_to<"ss:esp={:#04x}:{:#08x} ebp={:#08x} (esp{:+d})\n">(stdout, state.ss, state.esp, state.ebp, state.ebp - state.esp);
  format_to<"ds:esi={:#04x}:{:#08x} es:edi={:#04x}:{:#08x}\n">(stdout, state.ds, state.esi, state.ds, state.edi);
  format_to<"edx:eax={:#08x}:{:#08x} {:0>20d} {0:0>10d}:{1:0>10d}\n">(stdout, state.edx, state.eax, (state.edx*0x100000000ull + state.eax));
  format_to<"ecx:ebx={:#08x}:{:#08x} {:0>20d} {0:0>10d}:{1:0>10d}\n">(stdout, state.ecx, state.ebx, (state.ecx*0x100000000ull + state.ebx));
  format_to<"eflags={:#032b}\n">(stdout, state.eflags);
  format_to<"{}\n">(stdout, repeat_value<79>('-'));
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

    // DIE! ... (x _ x)
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
