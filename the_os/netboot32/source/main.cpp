#include <cstdint>
#include <span>
#include <charconv>

#include <interrupts.hpp>
#include <runtime.hpp>
#include <macros.hpp>
#include <print.hpp>
#include <bios_data_area.hpp>
#include <assembly.hpp>

CO_PUBLIC 
CO_SECTION(".text.main")
auto main () -> void
{
  runtime::initialize();
  using namespace std::string_view_literals;

  println("Available conventional memory : "sv, BDA::conventional_memory_size, " KiB"sv);

  for(auto i = 0; i < 16; ++i)
  {
    println("Hello, world! "sv, fmt::hex<fmt::upper>(i));
  }

  runtime::finalize();
}
