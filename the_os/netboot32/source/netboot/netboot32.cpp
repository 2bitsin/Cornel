#include <cstddef>
#include <cstdint>
#include <span>
#include <charconv>
#include <vector>
#include <string>

#include <hardware/bios_data_area.hpp>
#include <hardware/x86bios.hpp>
#include <hardware/x86assembly.hpp>

#include <utils/macros.hpp>

#include <textio/format.hpp>

#include <pxe/pxe.hpp>
#include <netboot/interrupts.hpp>
#include <netboot/runtime.hpp>
#include <netboot/memory.hpp>

void initialize(bool first_time)
{
  runtime::initialize(first_time);
  interrupts::initialize(first_time);    
  memory::initialize(first_time);
}

void finalize(bool last_time)
{
  memory::finalize(last_time);  
  interrupts::finalize(last_time);
  runtime::finalize(last_time);
}

CO_PUBLIC 
auto main (PXENVplus& _PXENVplus, bangPXE& _bangPXE) -> void
{    
  using namespace textio::fmt;
  initialize(true);

  __debugbreak();
  asm("int $0x3");

  pxe::initialize(true, _PXENVplus, _bangPXE);
  
  format_to<"Loading {} ...\n">(stdout, "netboot32.cfg");

  ::memory::buffer<std::byte> config_ini_buffer;
  [[maybe_unused]] const auto result = pxe::download_file("netboot32.cfg", config_ini_buffer);
 

  for(;;) { 
    x86arch::yield(); 
  }

  pxe::finalize(false);
  finalize(true);
}
