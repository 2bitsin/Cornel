#include <cstdint>
#include <isr_handler.hpp>


extern "C"
__attribute__((__cdecl__))
void ISR_handler(ISR_stack_frame& stack_frame)
{

}

extern "C"
__attribute__((section(".Main")))
void Main (void)
{
  asm("hlt");
}