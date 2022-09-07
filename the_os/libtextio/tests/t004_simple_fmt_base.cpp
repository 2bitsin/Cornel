#include <iterator>
#include <textio/simple.hpp>
#include "expect.hpp"

#ifdef TESTING
#define main t004_simple_fmt_base
#endif

int main(int,char** const) 
{
	using namespace std::string_literals;
	using namespace textio::simple;
	static const auto expected = "0d3735928559\r\n00000000000\r\n0xbadc0de\r\n0o00000000777\r\n777\r\n0b01010101101010100101010110101010\r\n-0b00000000000000000000000000000001\r\n-0x1\r\n0xffffffff\r\n0x123456789ABCDEF0\r\n0x123456789abcdef0\r\n"s;
	std::string buffer;
	auto output = std::back_inserter(buffer);
	writeln(output, fmt::dec<'x','p'>(0xDEADBEEF));
	writeln(output, fmt::dec<'p'>(0));
	writeln(output, fmt::hex<'x'>(0xBADC0DE));
	writeln(output, fmt::oct<'x','p'>(0777));
	writeln(output, fmt::oct(0777));
	writeln(output, fmt::bin<'p','x'>(0x55AA55AA));
	writeln(output, fmt::bin<'p','x','i'>(0xFFFFFFFFu));
	writeln(output, fmt::hex<'x','i'>(0xFFFFFFFF));
	writeln(output, fmt::hex<'x','u'>(0xFFFFFFFF));
	writeln(output, fmt::hex<'x', 'p', 'U'>(0x123456789ABCDEF0ull));
	writeln(output, fmt::hex<'x', 'p', 'U', 'l'>(0x123456789ABCDEF0ull));

	expect_eq(buffer, expected);
}