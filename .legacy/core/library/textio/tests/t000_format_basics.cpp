#include <iterator>
#include <textio/format.hpp>
#include "expect.hpp"

int main(int,char** const) 
{
  using namespace std::string_literals;
  using namespace std::string_view_literals;
	using namespace textio::fmt;
	using namespace textio::fmt::literals;
	auto const expected = "Hello {WORLD} !!!!!!!!!!!0b1010101!!!!!!!!!!!! 0o125*************************** U 0x00000000000055"s;

	std::string buffer;

	format_to<"Hello {{WORLD}} {0:!^#32b} {0:*<#32o} {0:->c} {0:#016x}">(buffer, 0x55);
	
	// TODO : WRITE MORE TESTS
  expect_eq(buffer, expected);
}  

