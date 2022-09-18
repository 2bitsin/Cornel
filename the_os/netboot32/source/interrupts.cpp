#include <string_view>

#include <misc/macros.hpp>
#include <misc/debug.hpp>
#include <hardware/x86asm.hpp>
#include <hardware/pic8259.hpp>
#include <hardware/console.hpp>

#include <netboot32/interrupts.hpp>
#include <textio/simple.hpp>

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

CO_PUBLIC
int ISR_handler(ISR_stack_frame& state)
{ 
  using namespace textio::simple;
  using namespace textio::simple::fmt;
  using namespace std;

  if (state.which < 32) 
  {
    console::writeln(repeat<79>('-'));
    console::writeln("Exception #", hex<'p'>((uint8_t)state.which), " - ", G_exception_string[state.which], " has occured.");
    console::writeln(repeat<79>('-'));

    console::writeln("cs:eip=" , hex<'p'>(state.cs ), ":", hex<'p'>(state.eip), " fs=", hex<'p'>(state.fs), " gs=", hex<'p'>(state.gs));
    console::writeln("ss:esp=" , hex<'p'>(state.ss ), ":", hex<'p'>(state.esp), " ebp=", hex<'p'>(state.ebp), " (esp + ", (state.ebp - state.esp), ")");
    console::writeln("ds:esi=" , hex<'p'>(state.ds ), ":", hex<'p'>(state.esi), " es:edi=", hex<'p'>(state.ds), ":", hex<'p'>(state.edi));
    console::writeln("edx:eax=", hex<'p'>(state.edx), ":", hex<'p'>(state.eax), " = ", dec<'p'>(state.edx*0x100000000ull + state.eax), ", ", dec<'p'>(state.edx), ":", dec<'p'>(state.eax));
    console::writeln("ecx:ebx=", hex<'p'>(state.ecx), ":", hex<'p'>(state.ebx), " = ", dec<'p'>(state.ecx*0x100000000ull + state.ebx), ", ", dec<'p'>(state.ecx), ":", dec<'p'>(state.ebx));

    console::writeln("eflags=", bin<'p'>(state.eflags));
    console::writeln(repeat<79>('-'));    

    __debugbreak();
    return 0;
  }
  const auto IRQ_num = state.which - 0x20;
  pic8259::end_of_interrupt(IRQ_num);
  return 0;
}

extern "C" const x86arch::Xdtr_t G_idtr;

static std::uint16_t save_mask = 0;

void isr::initialize([[maybe_unused]] bool first_time)
{ 
  x86arch::cli();  
  x86arch::lidt(G_idtr);
  pic8259::configure(0x20, 0x28);
  save_mask = pic8259::read_mask();
  pic8259::write_mask(0);
  x86arch::sti();
}
          
void isr::finalize([[maybe_unused]] bool last_time)
{ 
  x86arch::cli();
  pic8259::write_mask(save_mask);
  pic8259::configure(0x08, 0x70);
  x86arch::lidt({ .limit = 0x400, .base = nullptr });
}
