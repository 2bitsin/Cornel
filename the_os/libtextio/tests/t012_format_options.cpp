#include <iostream>
#include <iterator>
#include <textio/format.hpp>
#include "expect.hpp"

#ifdef TESTING
#define main t009_format_options
#endif

template <typename... Node>
void print(meta::type_list<Node...> const& w)
{
	((std::cout << '\'' << Node::value.as_string_view() << '\'' << "\n"), ...);
}

int main(int,char** const) 
{
  using namespace std::string_literals;
  using namespace std::string_view_literals;
	using namespace textio::fmt;
	using namespace textio::fmt::literals;

	auto const expected = ""s;

	std::string buffer;

	//detail::format_options<int, char> options ("#08");

	print(detail::format_encode<"edx:eax={:#08x}:{:#08x} {:<20d} {0:<10d}:{1:<10d}\n">());

	//format_to<"edx:eax={:#08x}:{:#08x} {:<20d} {0:<10d}:{1:<10d}\n">(stdout, 0x1234, 0x56789abc, 0xdef0, 0x1234);
	
	//const auto buffer = fmt_s0.as<std::string>();
	
	// TODO : WRITE MORE TESTS
  expect_eq(buffer, expected);
}  
