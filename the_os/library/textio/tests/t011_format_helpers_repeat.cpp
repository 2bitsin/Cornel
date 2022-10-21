#include <iterator>
#include <textio/format.hpp>
#include <textio/format/helpers/repeat_value.hpp>
#include "expect.hpp"

#ifdef TESTING
#define main t011_format_helpers_repeat
#endif

int main(int,char** const) 
{
  using namespace std::string_literals;
  using namespace std::string_view_literals;
	using namespace textio::fmt;
	using namespace textio::fmt::literals;
	using namespace textio::fmt::helpers;

	auto const expected = "- /  / ^ / ***** / *,*,*,*,*,*,*,*,*,* / Hello, Hello, Hello, Hello, Hello, Hello"s;
	
	static constexpr auto fmt_s0 = "{} / {} / {} / {} / {} / {}"_fmt; 
	
	const auto buffer = fmt_s0.as<std::string>('-', 
		repeat_value<0>('@'), 
		repeat_value<1>('^'), 
		repeat_value<5>('*'), 
		repeat_value<10>('*', ','), 
		repeat_value(6, "Hello", ", "));
	// TODO : WRITE MORE TESTS
  expect_eq(buffer, expected);
}  

