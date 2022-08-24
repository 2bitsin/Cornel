#include <cstdint>
#include <isr_handler.hpp>


extern "C"
{
  
  void ISR_handler(ISR_stack_frame& stack_frame)
  {

  }

  void* __dso_handle = nullptr;
  
  void __cxa_atexit(void (*destructor)(void*), void* arg, void* dso_handle)
  {

  }

  void __cxa_finalize(void* dso_handle)
  {

  }

  __attribute__((section(".Main")))
  void Main (void)
  {
    asm("hlt");
  }
}