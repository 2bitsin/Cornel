#include <hardware/macros.hpp>
#include <hardware/assembly.hpp>
#include <hardware/debug.hpp>
#include <hardware/pic8259.hpp>
#include <hardware/vga_text.hpp>

#include <netboot32/interrupts.hpp>
#include <textio/simple.hpp>

CO_PUBLIC
void ISR_handler(ISR_stack_frame& state)
{ 
  using namespace textio::simple;
  if (state.which < 32) 
  {
    writeln(tty(), fmt::repeat<80>('-'));
    writeln(tty(), "Exception #", state.which, " has occured.");
    writeln(tty(), fmt::repeat<80>('-'));
    writeln(tty(), "eip : ", fmt::hex<'p'>(state.eip), "eflags : ", fmt::bin<'p'>(state.eflags));
    writeln(tty(), "eax : ", fmt::hex<'p'>(state.eax), "edi : ", fmt::hex<'p'>(state.edi));
    writeln(tty(), "ebx : ", fmt::hex<'p'>(state.ebx), "esi : ", fmt::hex<'p'>(state.esi));
    writeln(tty(), "ecx : ", fmt::hex<'p'>(state.ecx), "ebp : ", fmt::hex<'p'>(state.ebp));
    writeln(tty(), "edx : ", fmt::hex<'p'>(state.edx), "esp : ", fmt::hex<'p'>(state.esp));
    writeln(tty(), " ds : ", fmt::hex<'p'>(state.ds ), " es : ", fmt::hex<'p'>(state.es ));
    writeln(tty(), " fs : ", fmt::hex<'p'>(state.fs ), " gs : ", fmt::hex<'p'>(state.gs ));
    writeln(tty(), " ss : ", fmt::hex<'p'>(state.ss ), " cs : ", fmt::hex<'p'>(state.cs ));    
    writeln(tty(), fmt::repeat<80>('-'));
    return;
  }
  //println("IRQ : ", state.which);
}

extern "C" const assembly::Xdtr_t G_idtr;

void isr::initialize()
{ 
  assembly::cli();  
  assembly::lidt(G_idtr);
  pic8259::configure(0x20, 0x28);
  assembly::sti();
}

void isr::finalize()
{ 
  assembly::cli();
  pic8259::configure(0x08, 0x70);
  assembly::lidt({ .limit = 0x400, .base = nullptr });
}
