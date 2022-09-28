#include <cstddef>
#include <cstdint>
#include <span>
#include <charconv>
#include <vector>
#include <string>

#include <hardware/console.hpp>
#include <hardware/bios_data_area.hpp>
#include <hardware/x86bios.hpp>

#include <utils/macros.hpp>

#include <textio/simple.hpp>

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
  initialize(true);
  pxe_interface::initialize(true, _PXENVplus, _bangPXE);
    
  std::uint32_t file_size { 0 };
  if (pxe_interface::pxenv_status::success == pxe_interface::tftp_get_fsize("config.ini", file_size)) {
    console::writeln("File size of config.ini : ", file_size);
  }
  else {
    console::writeln("Failed to get file size of config.ini");
  }

  for(;;) { x86arch::yield(); }

  pxe_interface::finalize(false);
  finalize(true);
}
