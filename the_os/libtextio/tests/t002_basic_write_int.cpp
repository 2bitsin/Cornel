#include <iterator>
#include <simple.hpp>
#include "expect.hpp"

#ifdef TESTING
#define main t002_basic_write_int
#endif

int main(int,char** const) 
{
	using namespace std::string_literals;
	using namespace textio::simple;
	static const auto expected = "1234567890\r\nHello World! The number is : -9876543210 !!!\r\n"s;
	std::string buffer;
	writeln(std::back_inserter(buffer), 1234567890);
	writeln(std::back_inserter(buffer), "Hello", ' ', "World", '!', ' ', "The number is : ", -9876543210, " !!!");
	expect_eq(buffer, expected);
}