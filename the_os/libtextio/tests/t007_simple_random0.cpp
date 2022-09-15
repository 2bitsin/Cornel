#include <iterator>
#include <textio/simple.hpp>
#include "expect.hpp"

#ifdef TESTING
#define main t007_simple_random0
#endif

int main(int,char** const) 
{
  using namespace std::string_literals;
  using namespace std::string_view_literals;
  using namespace textio::simple;
  static const auto expected = "This is a dec value : 3735928559 ...\r\nThis is a dec value : 3735928559 ...\r\nThis is a hex value : deadbeef ...\r\nThis is a oct value : 33653337357 ...\r\nThis is a bin value : 11011110101011011011111011101111 ...\r\n"s;
  std::string buffer;
  auto output = std::back_inserter(buffer);

  volatile std::uint16_t conventional_memory_size = 640;

  writeln(output, "This is a dec value : "sv, 0xdeadbeef, " ..."sv);
  writeln(output, "This is a dec value : "sv, fmt::dec(0xdeadbeef), " ..."sv);
  writeln(output, "This is a hex value : "sv, fmt::hex(0xdeadbeef), " ..."sv);
  writeln(output, "This is a oct value : "sv, fmt::oct(0xdeadbeef), " ..."sv);
  writeln(output, "This is a bin value : "sv, fmt::bin(0xdeadbeef), " ..."sv);

  expect_eq(buffer, expected);
}  

