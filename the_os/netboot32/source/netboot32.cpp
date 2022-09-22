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

#include <netboot32/pxe_api.hpp>
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

void print_memory_map()
{
  using textio::simple::fmt::hex;
  using textio::simple::fmt::dec;

  x86arch::bios_acpi_memory_map_entry_t entry { 0, 0, 0, 0 };  
  std::uint32_t offset = 0u;
  std::uint32_t length = 0u;
  std::uint32_t proffs = 0u;
  console::writeln("Getting memory map ... ");
__next:
  proffs = offset;
  if (std::errc{} == x86arch::bios_acpi_memory_map_read(entry, length, offset, offset)) 
  {    
    if (length > 0u)
    {
      std::string_view type_str;
      switch(entry.type)
      {
      case  1: type_str = "Available   " ; break;
      case  2: type_str = "Reserved    " ; break;
      case  3: type_str = "ACPI Reclaim" ; break;
      case  4: type_str = "ACPI NVS    " ; break;
      default: type_str = " ---------- " ; break;

      }
      console::writeln(hex<'&'>(proffs), " | ", hex<'&'>(entry.base), " .. ", hex<'&'>(entry.base + entry.size), " | ", type_str," (", entry.type, ") | ", dec<'&'>(entry.attr));      
      if (offset != 0u)
        goto __next;
    }
  }  
  console::writeln("Done.");
}

CO_PUBLIC 
auto main (PXENVplus& _PXENVplus, bangPXE& _bangPXE) -> void
{
  initialize_context context {_PXENVplus, _bangPXE};
  initialize(true);
  pxe_api::initialize(true, context);
  print_memory_map();
  for(;;) { x86arch::yield(); }

  pxe_api::finalize(false, context);
  finalize(true);
}
