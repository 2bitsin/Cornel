#include <macros.hpp>
#include <assembly.hpp>
#include <interrupts.hpp>

CO_PUBLIC
void ISR_handler(ISR_stack_frame& stack_frame)
{}

extern "C" const assembly::Xdtr_t G_idtr;

void isr::initialize()
{ assembly::lidt(G_idtr); }

void isr::finalize()
{}