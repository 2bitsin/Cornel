#include <string_view>

#include <misc/macros.hpp>
#include <misc/debug.hpp>
#include <hardware/assembly.hpp>
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
  "#00 - Divide-by-zero Error",
  "#01 - Debug",
  "#02 - Non-maskable Interrupt",
  "#03 - Breakpoint",
  "#04 - Overflow",
  "#05 - Bound Range Exceeded",
  "#06 - Invalid Opcode",
  "#07 - Device Not Available",
  "#08 - Double Fault",
  "#09 - Coprocessor Segment Overrun",
  "#0A - Invalid TSS",
  "#0B - Segment Not Present",
  "#0C - Stack-Segment Fault",
  "#0D - General Protection Fault",
  "#0E - Page Fault",
  "#0F - Reserved",
  "#10 - x87 Floating-Point Exception",
  "#11 - Alignment Check",
  "#12 - Machine Check",
  "#13 - SIMD Floating-Point Exception",
  "#14 - Virtualization Exception",
  "#15 - Control Protection Exception",
  "#16",
  "#17",
  "#18",
  "#19",
  "#1A",
  "#1B",
  "#1C - Hypervisor Injection Exception",
  "#1D - VMM Communication Exception",
  "#1E - Security Exception",
  "#1F"
};

CO_PUBLIC
int ISR_handler(ISR_stack_frame& state)
{ 
  using namespace textio::simple;

  if (state.which < 32) 
  {
    console::writeln(fmt::repeat<79>('-'));
    console::writeln("Exception ", G_exception_string[state.which], " has occured.");
    console::writeln(fmt::repeat<79>('-'));

    console::writeln("cs:eip=", fmt::hex<'p'>(state.cs), ":", fmt::hex<'p'>(state.eip), " fs=", fmt::hex<'p'>(state.fs), " gs=", fmt::hex<'p'>(state.gs));
    console::writeln("ss:esp=", fmt::hex<'p'>(state.ss), ":", fmt::hex<'p'>(state.esp), " ebp=", fmt::hex<'p'>(state.ebp), " (esp + ", (state.ebp - state.esp), ")");
    console::writeln("ds:esi=", fmt::hex<'p'>(state.ds), ":", fmt::hex<'p'>(state.esi), " -> es:edi=", fmt::hex<'p'>(state.ds), ":", fmt::hex<'p'>(state.edi));
    console::writeln("edx:eax=", fmt::hex<'p'>(state.edx), ":", fmt::hex<'p'>(state.eax), "=", (state.edx*0x100000000ull + state.eax), ", ", std::tuple(state.edx, state.eax));
    console::writeln("ecx:ebx=", fmt::hex<'p'>(state.ecx), ":", fmt::hex<'p'>(state.ebx), "=", (state.ecx*0x100000000ull + state.ebx), ", ", std::tuple(state.ecx, state.ebx));

    console::writeln("eflags=", fmt::bin<'p'>(state.eflags));
    console::writeln(fmt::repeat<79>('-'));    
    return 0;
  }
  console::writeln( "IRQ : ", state.which - 32);
  pic8259::end_of_interrupt(state.which);
  return 0;
}

extern "C" const assembly::Xdtr_t G_idtr;

static std::uint16_t save_mask = 0;

void isr::initialize()
{ 
  assembly::cli();  
  assembly::lidt(G_idtr);
  pic8259::configure(0x20, 0x28);
  save_mask = pic8259::read_mask();
  pic8259::write_mask(1);
  assembly::sti();
}

void isr::finalize()
{ 
  assembly::cli();
  pic8259::write_mask(save_mask);
  pic8259::configure(0x08, 0x70);
  assembly::lidt({ .limit = 0x400, .base = nullptr });
}
