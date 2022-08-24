#include <cstdint>

/*
TODO: 
  - Implement interrupts 
    - IDT and stubs
    - Exception handlers
    - Remap PIC
  - PXE boot implementation
    - Rebuild GDT
    - Implement API calls
*/



extern "C"
__attribute__((__cdecl__))
void ISR_handler()
{}


extern "C"
void Main (void)
{

}