#include <iterator>
#include <string>

#include "expect.hpp"

#include <textio/format.hpp>
#include <script/script.hpp>
#include <script/executor.hpp>

#include <meta/string.hpp>

#ifdef TESTING
#define main t000_hello_world
#endif


struct cmd_say
{
	static inline constexpr auto string = meta::string{ "say" };
};

struct cmd_hello
{
	static inline constexpr auto string = meta::string{ "hello" };
};


template <std::size_t Offset, meta::string... ArgN>
struct string_matcher {};

template <std::size_t Offset, meta::string Arg0, meta::string... ArgN>
struct string_matcher<Offset, Arg0, ArgN...>
{
	static inline constexpr auto match(auto&& value) -> std::size_t
	{
		std::basic_string_view lhs { value };
		std::basic_string_view rhs { meta::string_truncate_v<Arg0> };
		if (lhs == rhs)
			return Offset;		
		return string_matcher<Offset+1u, ArgN...>::match(value);
	}	
};

template <std::size_t Offset>
struct string_matcher<Offset>
{
	static inline constexpr auto match(auto&& value) -> std::size_t
	{
		return (std::size_t)-1 ;
	}
};

int main(int,char** const) 
{
  using namespace std::string_literals;
  using namespace textio::fmt;

  static const auto expected = ""s;
  auto buffer = ""s;
  auto o = std::back_inserter(buffer);

	auto q = string_matcher<0u, "say", "do", "hello", "goodbye">::match("hello");

	__debugbreak();
	
  //format_to (o, "Hello, World!\n");
/*
	script::script s;

	script::executor<cmd_say, cmd_hello> e;
	
	
	s.execute(e, R"(

	# Here's a comment!	
	say "Hello World!\n"
	say "Wait for it ...\n"              ...\n             "waaaait for it ....\n"						"... \n"				"Hello World!\n"
	say "Goodbye cruel world!\n"
	say "We need a string that can have a double quote in it: \"\n"
	exit 1
	# gruesome ... isn't it ? (._.)

	)");
	*/
  expect_eq(buffer, expected);
}  

