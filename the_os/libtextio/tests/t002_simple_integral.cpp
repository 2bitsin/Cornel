#include <iterator>
#include <textio/simple.hpp>
#include "expect.hpp"

#ifdef TESTING
#define main t002_simple_int
#endif

int main(int,char** const) 
{
	using namespace std::string_literals;
	using namespace textio::simple;
	static const auto expected =	" u8.max = 255;\r\n i8.max = -128;\r\nu16.max = 65535;\r\ni16.max = -32768;\r\nu32.max = 4294967295;\r\ni32.max = -2147483648;\r\nu64.max = 18446744073709551615;\r\ni64.max = -9223372036854775808;\r\nbool.true  = true;\r\nbool.false = false;\r\n"s;

	std::string buffer;
	auto output = std::back_inserter(buffer);
	writeln(output, " u8.max = ", (std::uint8_t)255, ";");
	writeln(output, " i8.max = ", (std::int8_t)-128, ";");
	writeln(output, "u16.max = ", (std::uint16_t)65535, ";");
	writeln(output, "i16.max = ", (std::int16_t)-32768, ";");
	writeln(output, "u32.max = ", (std::uint32_t)4294967295, ";");
	writeln(output, "i32.max = ", (std::int32_t)-2147483648, ";");
	writeln(output, "u64.max = ", (std::uint64_t)18446744073709551615ull, ";");
	writeln(output, "i64.max = ", (std::int64_t)-9223372036854775808ll, ";");
	writeln(output, "bool.true  = ", true, ";");
	writeln(output, "bool.false = ", false, ";");
	expect_eq(buffer, expected);
}