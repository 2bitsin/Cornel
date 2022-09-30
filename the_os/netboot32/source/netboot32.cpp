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
  
  std::span<std::byte> buffer;
  console::writeln("downloading config.ini ...");
  const auto download_result = pxe_interface::download_file("config.ini", buffer);
  if (download_result != pxe_interface::pxenv_status::success)
  {
    using namespace textio::simple::fmt;
    console::writeln("failed to download config.ini : ", hex<'&'>(std::to_underlying (download_result)));    
  }
  console::writeln("config.ini downloaded, size = ", buffer.size_bytes(), " bytes");
  //console::writeln(std::string_view{reinterpret_cast<char*>(buffer.data()), buffer.size()});


  for(;;) { x86arch::yield(); }

  pxe_interface::finalize(false);
  finalize(true);
}
