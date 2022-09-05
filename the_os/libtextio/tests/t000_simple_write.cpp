#include <iterator>
#include <simple.hpp>
#include "expect.hpp"

#ifdef TESTING
#define main t000_simple_write
#endif

int main(int,char** const) 
{
	using namespace std::string_literals;
	using namespace textio::simple;
	static const auto expected = "Hello World!"s;
	std::string buffer;	
	write(std::back_inserter(buffer), "Hello", ' ', "World", '!');
  expect_eq(buffer, expected);
}