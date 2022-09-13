#include <cstddef>
#include <cstdint>
#include <span>
#include <charconv>

#include <misc/macros.hpp>
#include <textio/simple.hpp>
#include <netboot32/interrupts.hpp>
#include <netboot32/runtime.hpp>
#include <hardware/console.hpp>
#include <hardware/bios_data_area.hpp>
#include <hardware/assembly.hpp>
#include <hardware/atwenty.hpp>

void initialize()
{
  runtime::initialize();
  isr::initialize();
  atwenty::initialize();
}

void finalize()
{
  atwenty::finalize();
  isr::finalize();
  runtime::finalize();
}

CO_PUBLIC 
auto main () -> void
{
  using namespace std::string_view_literals;
  using namespace textio::simple;
  initialize();
  
  console::writeln("Available conventional memory : ", bda::conventional_memory_size, " KiB");  
  asm("int $0x3");
  for(;;)
  {
    assembly::hlt();
  }

  finalize();
}
