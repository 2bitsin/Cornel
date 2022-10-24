#include <iterator>
#include <string>

#include "expect.hpp"

#include <textio/format.hpp>
#include <script/script.hpp>
#include <script/executor.hpp>

#ifdef TESTING
#define main t000_hello_world
#endif

int main(int,char** const) 
{
  using namespace std::string_literals;
  using namespace textio::fmt;

  static const auto expected = ""s;
  auto buffer = ""s;
  auto o = std::back_inserter(buffer);

  //format_to (o, "Hello, World!\n");

	script::script s;

	script::executor<script::descriptor<1,2,3>{}> e;
	
	s.execute(e, R"(

	# Here's a comment!	
	say "Hello World!\n"
	say "Wait for it ...\n"              ...\n             "waaaait for it ....\n"						"... \n"				"Hello World!\n"
	say "Goodbye cruel world!\n"
	say "We need a string that can have a double quote in it: \"\n"
	exit 1
	# gruesome ... isn't it ? (._.)

	)");

  expect_eq(buffer, expected);
}  

