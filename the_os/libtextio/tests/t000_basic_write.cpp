#include <iterator>

#include <simple.hpp>

int t000_basic_write(int,char** const) 
{
	using namespace std::string_literals;
	using namespace textio::simple;
	static const auto expected = "Hello World!"s;
	std::string buffer;	
	write(std::back_inserter(buffer), "Hello", ' ', "World", '!');
  return !(buffer == expected);
}