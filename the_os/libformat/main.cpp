#include <type_traits>
#include <iostream>
#include <string_view>
#include <string>

#include "format_as.hpp"


int main(int, char**)
{
  using namespace textio::fmt;

  constexpr meta::string fmt_s { "Hello {{o} {:#X} {2} {:#X} {0} {:#X} {1} {:#X}" };
  
  const auto s = format_as<fmt_s, std::string>(0x12345678u, 0x87654321, 0x0BADC0DE, 0xDEADBEEF);
  std::cout << s << std::endl;

  return 0;
}