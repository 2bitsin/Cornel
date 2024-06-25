#include <iterator>
#include <string>
#include <string_view>

#include "expect.hpp"

#include <textio/format.hpp>
#include <script/interpreter.hpp>
#include <script/executor.hpp>

#include <meta/string.hpp>

#ifdef TESTING
#define main t000_hello_world
#endif


struct cmd_say: script::command_base<cmd_say, "say", std::vector<std::string>>
{
	auto execute(auto&& context, auto&& all_messages) -> int
	{
		for(auto&& curr_msg : all_messages)
			context.log<"{} ">(curr_msg);
		context.log<"\n">();
		return 0;
	}
};

struct cmd_exit: script::command_base<cmd_exit, "exit", int>
{
	auto execute(auto&& context, auto&& code) -> int
	{
		context.log<"Exited with code : {}\n">(code);
		return 0;
	}
};

struct cmd_hello : script::command_base<cmd_hello, "hello">
{
	auto execute(auto&& context) -> int
	{
		context.log<"Hello World!\n">();
		return 0;
	}
};

struct cmd_perform : script::command_base<cmd_perform, "perform", std::string_view>
{
	auto execute(auto&& context, auto&& command) -> int
	{
		context.log<"* Program performs {} with great success!\n">(command);
		return 0;
	}
};

template <std::output_iterator<char> O>
struct executor: script
	::executor<executor<O>, 
		cmd_perform,
		cmd_exit, 
		cmd_hello,
		cmd_say
	>
{
	O out_i;
	
	executor(O out_i) : out_i(out_i) {}

	auto stdout_handle() const 
	{
		return out_i;
	}

	template <meta::string Format_string>
	auto log(auto&& ... what) 
	{
		using textio::fmt::format_to;
		format_to<Format_string>(stdout_handle(), std::forward<decltype(what)>(what)...);
	}
};


int main(int,char** const) 
{
  using namespace std::string_literals;
  using namespace textio::fmt;

  static const auto expected = "Hello World!\nGoodbye cruel World! \n* Program performs hirakiri with great success!\nExited with code : 1\n"s;
  auto buffer = ""s;
  auto o = std::back_inserter(buffer);
	
	script::interpreter s;
	executor e { o };
		
	s.execute(e, R"(
		# Here's a comment!	
		hello
		say "Goodbye\n"   "cruel"   "World!"
		perform hirakiri		
		exit 1
		blub
		perform hira kiri
		# gruesome ... isn't it ? (._.)
	)");

	
  expect_eq(buffer, expected);
}  

