#include <iterator>
#include <textio/simple.hpp>
#include "expect.hpp"

#ifdef TESTING
#define main t006_simple_tuple
#endif

int main(int,char** const) 
{
	using namespace std::string_literals;
	using namespace textio::simple;
	static const auto expected = "()\r\n(true)\r\n(A, BC)\r\n(1, 2, 3)\r\n(4660, FOO, 3735928559)\r\n(1234, 11011110101011011011111011101111)\r\n"s;
	std::string buffer;
	auto output = std::back_inserter(buffer);

	std::uint16_t i{ 0x1234 };
	std::string_view j{ "FOO" };
	std::uint32_t k{ 0xDeadBeef };

	writeln(output, std::tuple());
	writeln(output, std::tuple(true));	
	writeln(output, std::tuple('A', "BC"));
	writeln(output, std::tuple(1, 2, 3));
	writeln(output, std::tie(i, j, k));
	writeln(output, std::tuple(fmt::hex(i), fmt::bin(k)));

	expect_eq(buffer, expected);
}