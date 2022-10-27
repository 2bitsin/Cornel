#include <iterator>
#include <string>
#include <string_view>

#include "expect.hpp"

#include <textio/format.hpp>
#include <script/script.hpp>
#include <script/executor.hpp>

#include <meta/string.hpp>

#ifdef TESTING
#define main t000_hello_world
#endif


struct cmd_say: script::command_base<cmd_say, "say", std::string_view>
{
	auto execute(auto&& context, auto&& message) -> int
	{
		std::cout << message << std::endl;
		return 0;
	}
};

struct cmd_exit: script::command_base<cmd_exit, "exit", int>
{
	auto execute(auto&& context, auto&& code) -> int
	{
		std::cout << "Exited with code : " << int(code) << std::endl;
		return 0;
	}
};



int main(int,char** const) 
{
  using namespace std::string_literals;
  using namespace textio::fmt;

  static const auto expected = ""s;
  auto buffer = ""s;
  auto o = std::back_inserter(buffer);



	
	script::script s;

	script::executor<cmd_say, cmd_exit> e;
	
	
	s.execute(e, R"(

	# Here's a comment!	
	say "Hello World!"
	exit 1
	# gruesome ... isn't it ? (._.)

	)");
	
  expect_eq(buffer, expected);
}  

