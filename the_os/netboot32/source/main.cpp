#include <cstdint>
#include <span>
#include <charconv>

#include <interrupts.hpp>
#include <runtime.hpp>
#include <macros.hpp>
#include <print.hpp>
#include <bios_data_area.hpp>
#include <assembly.hpp>

void initialize()
{
  isr::initialize();
  runtime::initialize();
}

void finalize()
{
  runtime::finalize();
  isr::finalize();
}

CO_PUBLIC 
CO_SECTION(".text.main")
auto main () -> void
{
  initialize();

  println("Available conventional memory : ", BDA::conventional_memory_size, " KiB");
  
 
  finalize();
}
