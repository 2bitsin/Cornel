#include <macros.hpp>
#include <assembly.hpp>
#include <interrupts.hpp>
#include <print.hpp>

CO_PUBLIC
void ISR_handler(ISR_stack_frame& stack_frame)
{}

extern "C" const assembly::Xdtr_t G_idtr;

void isr::initialize()
{ assembly::lidt(G_idtr); }

void isr::finalize()
{ 
  const assembly::Xdtr_t t { .limit = 0x400, .base = nullptr };
  assembly::lidt(t);
}

struct isr_initialize
{
  isr_initialize() noexcept 
  { 
    println(__func__);
    isr::initialize(); 
  }

  ~isr_initialize() noexcept 
  { 
    println(__func__);
    isr::finalize(); 
  }
};

static volatile isr_initialize _isr_initialize{};