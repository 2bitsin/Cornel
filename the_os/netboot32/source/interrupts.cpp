#include <macros.hpp>
#include <assembly.hpp>
#include <interrupts.hpp>
#include <print.hpp>
#include <debug.hpp>
#include <pic8259.hpp>

CO_PUBLIC
void ISR_handler(ISR_stack_frame& state)
{ 
  if (state.which < 32) 
  {
    println(fmt::repeat('-', 80));
    println("Exception #", state.which, " has occured.");
    println(fmt::repeat('-', 80));
    println("eip : ", fmt::hex(state.eip), "eflags : ", fmt::bin(state.eflags));
    println("eax : ", fmt::hex(state.eax), "edi : ", fmt::hex(state.edi));
    println("ebx : ", fmt::hex(state.ebx), "esi : ", fmt::hex(state.esi));
    println("ecx : ", fmt::hex(state.ecx), "ebp : ", fmt::hex(state.ebp));
    println("edx : ", fmt::hex(state.edx), "esp : ", fmt::hex(state.esp));
    println(" ds : ", fmt::hex(state.ds ), " es : ", fmt::hex(state.es ));
    println(" fs : ", fmt::hex(state.fs ), " gs : ", fmt::hex(state.gs ));
    println(" ss : ", fmt::hex(state.ss ), " cs : ", fmt::hex(state.cs ));    
    println(fmt::repeat('-', 80));
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
