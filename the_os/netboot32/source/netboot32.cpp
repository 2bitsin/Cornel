#include <cstddef>
#include <cstdint>
#include <span>
#include <charconv>
#include <vector>
#include <string>

#include <hardware/console.hpp>
#include <hardware/bios_data_area.hpp>
#include <hardware/x86arch.hpp>

#include <utils/macros.hpp>

#include <textio/simple.hpp>

#include <netboot32/pxe_api.hpp>
#include <netboot32/interrupts.hpp>
#include <netboot32/runtime.hpp>
#include <netboot32/memory.hpp>
#include <netboot32/netboot32.hpp>
#include <netboot32/call16.hpp>

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
  netboot32::call16_context_type ctx;
  ctx.eax = 0x12344321;
  ctx.ebx = 0x56788765;
  ctx.ecx = 0x9abccba9;
  ctx.edx = 0xdef00fed;
  ctx.esi = 0x12345678;
  ctx.edi = 0x9abcdef0;
  ctx.ebp = 0x12345678;
  ctx.ds = 0x1234;
  ctx.es = 0x5678;
  ctx.fs = 0x9abc;
  ctx.gs = 0xdef0;

  ctx.flags = 0b0'0000'0000'0000'0000;

  netboot32::call16_int(ctx, 0x12);

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
