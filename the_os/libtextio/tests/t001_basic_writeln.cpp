#include <iterator>

#include <simple.hpp>

int t001_basic_writeln(int,char** const) 
{
	using namespace std::string_literals;
	using namespace textio::simple;
	static const auto expected = "Hello World!\r\n"s;
	std::string buffer;	
	writeln(std::back_inserter(buffer), "Hello", ' ', "World", '!');
  return !(buffer == expected);
}