#include <iterator>
#include <textio/simple.hpp>
#include <textio/simple_fmt_data_size.hpp>
#include "expect.hpp"

#ifdef TESTING
#define main t007_simple_random0
#endif

int main(int,char** const) 
{
  using namespace std::string_literals;
  using namespace std::string_view_literals;
  using namespace textio::simple;
	using namespace textio::simple::fmt;
  static const auto expected = ""s;
  std::string buffer;
  auto output = std::back_inserter(buffer);

	writeln(output, data_size(999ull));
	writeln(output, data_size(999999ull));
	writeln(output, data_size(999999999ull));	
	writeln(output, data_size(999999999999ull));

  expect_eq(buffer, expected);
}  

