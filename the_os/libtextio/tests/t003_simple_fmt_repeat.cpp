#include <iterator>
#include <textio/simple.hpp>
#include "expect.hpp"

#ifdef TESTING
#define main t003_simple_fmt_repeat
#endif

int main(int,char** const) 
{
	using namespace std::string_literals;
	using namespace textio::simple;
	static const auto expected = "Hello!Hello!Hello!Hello!Hello!Hello!Hello!Hello!Hello!Hello!\r\n\r\n123123123\r\n"s;
	std::string buffer;
	auto output = std::back_inserter(buffer);
	writeln(output, fmt::repeat<10>("Hello!"));
	writeln(output, fmt::repeat<0>("Hello!"));
	writeln(output, fmt::repeat<3>(123));	
	expect_eq(buffer, expected);
}