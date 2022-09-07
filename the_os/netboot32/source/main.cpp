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
auto main () -> void
{
  using namespace std::string_view_literals;
  using namespace textio::simple;
  initialize();

  writeln(tty(), "This is a value : "sv, 0xdeadbeef, " ..."sv);

  writeln(tty(), "This is a bin int value : ", fmt::bin<'x','p'>(0xDEADBEEF), " ...");
  writeln(tty(), "This is a qua int value : ", fmt::qua<'x','p'>(0xDEADBEEF), " ...");
  writeln(tty(), "This is a oct int value : ", fmt::oct<'x','p'>(0xDEADBEEF), " ...");
  writeln(tty(), "This is a dec int value : ", fmt::dec<'x','p'>(0xDEADBEEF), " ...");
  writeln(tty(), "This is a hex int value : ", fmt::hex<'x','p'>(0xDEADBEEF), " ...");

  writeln(tty(), "This is a bin uint value : ", fmt::bin<'x','p'>(0xDEADBEEFu), " ...");
  writeln(tty(), "This is a qua uint value : ", fmt::qua<'x','p'>(0xDEADBEEFu), " ...");
  writeln(tty(), "This is a oct uint value : ", fmt::oct<'x','p'>(0xDEADBEEFu), " ...");
  writeln(tty(), "This is a dec uint value : ", fmt::dec<'x','p'>(0xDEADBEEFu), " ...");
  writeln(tty(), "This is a hex uint value : ", fmt::hex<'x','p'>(0xDEADBEEFu), " ...");
  
  writeln(tty(), "Available conventional memory : ", BDA::conventional_memory_size, " KiB");  
 
  for(;;)
  {
    asm volatile("hlt");
  }

  finalize();
}
