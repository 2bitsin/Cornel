#include <type_traits>
#include <iostream>
#include <string_view>
#include <string>

#include "format_as.hpp"
#include "format_options.hpp"

int main(int, char**)
{
  using namespace textio::fmt;
	using namespace textio::fmt::detail;

  constexpr meta::string fmt_s { "Hello {{o} {:#X} {0:#x} {0:#08x} {0:08x}" };
  
  const auto s = format_as<fmt_s, std::string>(0x12345678u);
  std::cout << s << std::endl;

	

  return 0;
}