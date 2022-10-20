#include <string_view>
#include <cstdio>

#include <utils/macros.hpp>
#include <utils/debug.hpp>
#include <hardware/x86assembly.hpp>
#include <hardware/pic8259.hpp>
#include <hardware/rtccmos.hpp>

#include <netboot32/keyboard.hpp>
#include <netboot32/interrupts.hpp>

#include <textio/simple.hpp>

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

CO_PUBLIC
int ISR_handler(interrupts::stack_frame& state)
{ 
  using namespace textio::simple;
  using namespace textio::simple::fmt;
  using namespace std;

  if (state.which < 32) 
  {
    writeln_to(stdout, repeat<79>('-'));
    writeln_to(stdout, "Exception #", hex<'p'>((uint8_t)state.which), " - ", G_exception_string[state.which], " has occured.");
    writeln_to(stdout, repeat<79>('-'));
    writeln_to(stdout, "cs:eip=" , hex<'p'>(state.cs ), ":", hex<'p'>(state.eip), " fs=", hex<'p'>(state.fs), " gs=", hex<'p'>(state.gs));
    writeln_to(stdout, "ss:esp=" , hex<'p'>(state.ss ), ":", hex<'p'>(state.esp), " ebp=", hex<'p'>(state.ebp), " (esp + ", (state.ebp - state.esp), ")");
    writeln_to(stdout, "ds:esi=" , hex<'p'>(state.ds ), ":", hex<'p'>(state.esi), " es:edi=", hex<'p'>(state.ds), ":", hex<'p'>(state.edi));
    writeln_to(stdout, "edx:eax=", hex<'p'>(state.edx), ":", hex<'p'>(state.eax), " = ", dec<'p'>(state.edx*0x100000000ull + state.eax), ", ", dec<'p'>(state.edx), ":", dec<'p'>(state.eax));
    writeln_to(stdout, "ecx:ebx=", hex<'p'>(state.ecx), ":", hex<'p'>(state.ebx), " = ", dec<'p'>(state.ecx*0x100000000ull + state.ebx), ", ", dec<'p'>(state.ecx), ":", dec<'p'>(state.ebx));
    writeln_to(stdout, "eflags=", bin<'p'>(state.eflags));
    writeln_to(stdout, repeat<79>('-'));    
    
    __debugbreak();
    std::abort();
  }
  const auto IRQ_num = state.which - 0x20;
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
