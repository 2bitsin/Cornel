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
  static const auto expected = "0\r\n9\r\n99\r\n999\r\n9K 783\r\n97K 671\r\n976K 575\r\n9M 549K 639\r\n95M 376K 255\r\n953M 690K 511\r\n9G 320M 760K 1023\r\n93G 135M 441K 1023\r\n931G 330M 323K 1023\r\n9T 97G 231M 167K 1023\r\n90T 972G 263M 655K 1023\r\n909T 506G 588M 415K 1023\r\n8P 902T 969G 764M 63K 1023\r\n88P 837T 481G 472M 639K 1023\r\n1K\r\n1M\r\n1G 1K\r\n1T\r\n1P 1M 1K 1\r\n1E\r\n0\r\n"s;
  std::string buffer;
  auto output = std::back_inserter(buffer);

	for (auto i = 1ull; i < 1000000000000000000ull; i *= 10u) {
		writeln(output, data_size(i - 1u));
	}
	
	writeln(output, data_size(0x400ull));
	writeln(output, data_size(0x100000ull));
	writeln(output, data_size(0x40000000ull + 0x400ull));
	writeln(output, data_size(0x10000000000ull));
	writeln(output, data_size(0x4000000000000ull + 0x100000ull + 0x401ull));
	writeln(output, data_size(0x1000000000000000ull));
	writeln(output, data_size(0));

  expect_eq(buffer, expected);
}  

