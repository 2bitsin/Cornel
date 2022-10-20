#include <iterator>
#include <textio/format.hpp>
#include "expect.hpp"

#ifdef TESTING
#define main t009_format_basic
#endif

int main(int,char** const) 
{
  using namespace std::string_literals;
  using namespace std::string_view_literals;
	using namespace textio::fmt;

	auto const expected = "Hello {WORLD} !!!!!!!!!!!0b1010101!!!!!!!!!!!! 0o125*************************** U 0x00000000000055"s;

	std::string buffer;
	format_to<"Hello {{WORLD}} {0:!^#32b} {0:*<#32o} {0:->c} {0:#016x}">(std::back_inserter(buffer), 0x55);
	
	// TODO : WRITE MORE TESTS
  expect_eq(buffer, expected);
}  

