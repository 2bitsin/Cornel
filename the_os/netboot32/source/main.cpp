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
  using namespace std::string_view_literals;

  runtime::initialize();

  println("Available conventional memory : ", BDA::conventional_memory_size, " KiB");

  println("Hello, world! >>", fmt::hex(std::uint8_t(0x12)));
  println("Hello, world! >>", fmt::hex(std::uint16_t(0x1234)));
  println("Hello, world! >>", fmt::hex(std::uint32_t(0x12345678)));
  println("Hello, world! >>", fmt::hex(std::uint64_t(0x123456789ABCDEF0)));

  runtime::finalize();
}
