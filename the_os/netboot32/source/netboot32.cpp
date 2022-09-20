#include <cstddef>
#include <cstdint>
#include <span>
#include <charconv>
#include <vector>
#include <string>

#include <hardware/console.hpp>
#include <hardware/bios_data_area.hpp>
#include <hardware/x86arch.hpp>
#include <hardware/x86call16.hpp>

#include <utils/macros.hpp>

#include <textio/simple.hpp>

#include <netboot32/pxe_api.hpp>
#include <netboot32/interrupts.hpp>
#include <netboot32/runtime.hpp>
#include <netboot32/memory.hpp>
#include <netboot32/netboot32.hpp>

namespace netboot32
{
  void initialize(bool first_time)
  {
    runtime::initialize(first_time);
    isr::initialize(first_time);    
    memory::initialize(first_time);
  }

  void finalize(bool last_time)
  {
    memory::finalize(last_time);  
    isr::finalize(last_time);
    runtime::finalize(last_time);
  }
}

void test_call16()
{
  x86arch::call16_context ctx { 
    .eax    = 0,
    .ebx    = 0,
    .ecx    = 0,
    .edx    = 0,
    .esi    = 0,
    .edi    = 0,
    .ebp    = 0,
    .ds     = 0,
    .es     = 0,
    .fs     = 0,
    .gs     = 0,
    .esp    = 0,
    .ss     = 0,
    .flags  = 0
  };

  x86arch::call16_stack_allocate(ctx, 0x1000);
  x86arch::call16_invoke(ctx, 0x12);
  x86arch::call16_stack_deallocate(ctx);
  console::writeln("INT 12H, EAX = ", ctx.eax & 0xffff);

}

CO_PUBLIC 
auto main (PXENVplus& _PXENVplus, bangPXE& _bangPXE) 
  -> void
{
  initialize_context context {_PXENVplus, _bangPXE};
  netboot32::initialize(true);
  pxe_api::finalize(true, context);

  test_call16();
    
  for(;;)  
  {
    x86arch::yield();
  }

  pxe_api::finalize(false, context);
  netboot32::finalize(true);
}
