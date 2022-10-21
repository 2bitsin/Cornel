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

#include <netboot32/pxe_interface.hpp>
#include <netboot32/interrupts.hpp>
#include <netboot32/runtime.hpp>
#include <netboot32/memory.hpp>

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
  using namespace std::string_view_literals;
  using namespace textio::fmt;
  using namespace textio::fmt::literals;

  initialize(true);
  pxe_interface::initialize(true, _PXENVplus, _bangPXE);
  
  format_to<"Loading {} ...\n">(stdout, "config.ini");

  ::memory::buffer<std::byte> config_ini_buffer;
  [[maybe_unused]] const auto result 
    = pxe_interface::download_file("config.ini", config_ini_buffer);
 
  for(;;) { x86arch::yield(); }

  pxe_interface::finalize(false);
  finalize(true);
}
