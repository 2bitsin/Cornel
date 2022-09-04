#include <iterator>
#include <iostream>
#include <simple.hpp>

int t002_basic_write_int32(int,char** const) 
{
	using namespace std::string_literals;
	using namespace textio::simple;
	static const auto expected = "Hello World!\r\n"s;
	std::string buffer;	

	//writeln(std::back_inserter(buffer), "Hello", ' ', "World", '!');
  //return !(buffer == expected);
	std::cout << "Hello!";

	return 0;
}