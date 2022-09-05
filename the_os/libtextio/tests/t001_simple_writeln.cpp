#include <iterator>
#include <simple.hpp>
#include "expect.hpp"

#ifdef TESTING
#define main t001_simple_writeln
#endif

int main(int,char** const) 
{
	using namespace std::string_literals;
	using namespace textio::simple;
	static const auto expected = "Hello World!\r\n"s;
	std::string buffer;	
	writeln(std::back_inserter(buffer), "Hello", ' ', "World", '!');
  expect_eq(buffer, expected);
}