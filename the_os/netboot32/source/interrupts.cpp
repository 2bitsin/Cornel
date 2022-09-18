#include <string_view>

#include <misc/macros.hpp>
#include <misc/debug.hpp>
#include <hardware/x86asm.hpp>
#include <hardware/pic8259.hpp>
#include <hardware/console.hpp>

#include <netboot32/interrupts.hpp>
#include <textio/simple.hpp>

/*

Divide-by-zero Error  0 (0x0) Fault #DE No
Debug 1 (0x1) Fault/Trap  #DB No
Non-maskable Interrupt  2 (0x2) Interrupt - No
Breakpoint  3 (0x3) Trap  #BP No
Overflow  4 (0x4) Trap  #OF No
Bound Range Exceeded  5 (0x5) Fault #BR No
Invalid Opcode  6 (0x6) Fault #UD No
Device Not Available  7 (0x7) Fault #NM No
Double Fault  8 (0x8) Abort #DF Yes (Zero)
Coprocessor Segment Overrun 9 (0x9) Fault - No
Invalid TSS 10 (0xA)  Fault #TS Yes
Segment Not Present 11 (0xB)  Fault #NP Yes
Stack-Segment Fault 12 (0xC)  Fault #SS Yes
General Protection Fault  13 (0xD)  Fault #GP Yes
Page Fault  14 (0xE)  Fault #PF Yes
Reserved  15 (0xF)  - - No
x87 Floating-Point Exception  16 (0x10) Fault #MF No
Alignment Check 17 (0x11) Fault #AC Yes
Machine Check 18 (0x12) Abort #MC No
SIMD Floating-Point Exception 19 (0x13) Fault #XM/#XF No
Virtualization Exception  20 (0x14) Fault #VE No
Control Protection Exception  21 (0x15) Fault #CP Yes
Reserved
Hypervisor Injection Exception  
VMM Communication Exception
Security Exception
Reserved  
Triple Fault
*/

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
