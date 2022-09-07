#include <cstdint>
#include <span>
#include <charconv>

#include <textio/simple.hpp>
#include <netboot32/interrupts.hpp>
#include <netboot32/runtime.hpp>
#include <hardware/vga_text.hpp>
#include <hardware/macros.hpp>
#include <hardware/bios_data_area.hpp>
#include <hardware/assembly.hpp>

void initialize()
{
  //isr::initialize();
  runtime::initialize();
}

void finalize()
{
  runtime::finalize();
  //isr::finalize();
}

CO_PUBLIC 
CO_SECTION(".text.main")
auto main () -> void
{
  using namespace std::string_view_literals;
  using namespace textio::simple;
//  initialize();

  writeln(tty(), "This is a dec value : "sv, 0xdeadbeef, " ..."sv);
  writeln(tty(), "This is a dec value : "sv, fmt::dec(0xdeadbeef), " ..."sv);
  writeln(tty(), "This is a hex value : "sv, fmt::hex(0xdeadbeef), " ..."sv);
  writeln(tty(), "This is a oct value : "sv, fmt::oct(0xdeadbeef), " ..."sv);
  writeln(tty(), "This is a bin value : "sv, fmt::bin(0xdeadbeef), " ..."sv);
  writeln(tty(), "Available conventional memory : "sv, BDA::conventional_memory_size, " KiB"sv);  
 
//  finalize();
}
