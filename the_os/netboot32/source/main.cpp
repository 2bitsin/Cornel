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

  console::writeln("This is a value : "sv, 0xdeadbeef, " ..."sv);
  console::writeln("This is a bin int value : ", fmt::bin<'x','p'>(0xDEADBEEF), " ...");
  console::writeln("This is a qua int value : ", fmt::qua<'x','p'>(0xDEADBEEF), " ...");
  console::writeln("This is a oct int value : ", fmt::oct<'x','p'>(0xDEADBEEF), " ...");
  console::writeln("This is a dec int value : ", fmt::dec<'x','p'>(0xDEADBEEF), " ...");
  console::writeln("This is a hex int value : ", fmt::hex<'x','p'>(0xDEADBEEF), " ...");
  console::writeln("This is a bin uint value : ", fmt::bin<'x','p'>(0xDEADBEEFu), " ...");
  console::writeln("This is a qua uint value : ", fmt::qua<'x','p'>(0xDEADBEEFu), " ...");
  console::writeln("This is a oct uint value : ", fmt::oct<'x','p'>(0xDEADBEEFu), " ...");
  console::writeln("This is a dec uint value : ", fmt::dec<'x','p'>(0xDEADBEEFu), " ...");
  console::writeln("This is a hex uint value : ", fmt::hex<'x','p'>(0xDEADBEEFu), " ..."); 
  console::writeln("Available conventional memory : ", bda::conventional_memory_size, " KiB");  
 
  for(;;)
  {
    assembly::hlt();
  }

  finalize();
}
