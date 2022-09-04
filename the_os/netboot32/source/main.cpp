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
  initialize();

  println("This is a hex value : ", fmt::hex(0xdeadbeef), " ...");
  println("This is a dec value : ", 0xdeadbeef, " ...");
  println("This is a dec value : ", fmt::dec(0xdeadbeef), " ...");
  println("This is a oct value : ", fmt::oct(0xdeadbeef), " ...");
  println("This is a bin value : ", fmt::bin(0xdeadbeef), " ...");

  println("Available conventional memory : "sv, BDA::conventional_memory_size, " KiB"sv);
  
 
  finalize();
}
