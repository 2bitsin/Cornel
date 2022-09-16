#include <cstddef>
#include <cstdint>
#include <span>
#include <charconv>
#include <vector>
#include <string>

#include <hardware/console.hpp>
#include <hardware/bios_data_area.hpp>
#include <hardware/x86arch.hpp>
#include <hardware/atwenty.hpp>

#include <misc/macros.hpp>

#include <textio/simple.hpp>

#include <netboot32/pxe_api.hpp>
#include <netboot32/interrupts.hpp>
#include <netboot32/runtime.hpp>
#include <netboot32/memory.hpp>


void initialize(bool first_time, initialize_context const& context)
{
  runtime::initialize(first_time);
  isr::initialize(first_time);  
  atwenty::initialize(first_time);
  memory::initialize(first_time);
  pxe_api::initialize(first_time, context);  
}

void finalize(bool last_time, initialize_context const& context)
{
  pxe_api::finalize(last_time, context);
  memory::finalize(last_time);
  atwenty::finalize(last_time);
  isr::finalize(last_time);
  runtime::finalize(last_time);
}

CO_PUBLIC 
auto main (pxe_api::PXENVplus& _PXENVplus, 
           pxe_api::bangPXE& _bangPXE) 
  -> void
{
  initialize_context context
  {
    _PXENVplus, 
    _bangPXE
  };

  initialize(true, context);

  using namespace std::string_view_literals;
  using namespace std::string_literals;
  using namespace textio::simple::fmt;
  
  for(;;)  
  {
    x86arch::yield();
  }

  finalize(true, context);
}
