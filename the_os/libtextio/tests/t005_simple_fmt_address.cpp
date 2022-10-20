#include <iterator>
#include <textio/simple.hpp>
#include "expect.hpp"

#ifdef TESTING
#define main t005_simple_fmt_address
#endif

int main(int,char** const) 
{
  using namespace std::string_literals;
  using namespace textio::simple;
  static const auto expected = sizeof(void*) <= 4 
		? "0x0BADC0DE\r\n0BADC0DE\r\n0xdeadbeef\r\n0b00001011101011011100000011011110\r\n00001011101011011100000011011110\r\n0b11011110101011011011111011101111\r\n"s
		: "0x000000000BADC0DE\r\n000000000BADC0DE\r\n0x00000000deadbeef\r\n0b0000000000000000000000000000000000001011101011011100000011011110\r\n0000000000000000000000000000000000001011101011011100000011011110\r\n0b0000000000000000000000000000000011011110101011011011111011101111\r\n";
  std::string buffer;
  auto output = std::back_inserter(buffer);

  static volatile int* ptr_test_value = (volatile int *)0x0BADC0DE;
  static volatile int& ref_test_value = *ptr_test_value;
  static volatile char* str_ptr_value = (volatile char *)0xDEADBEEF;
  
  writeln(output, fmt::hex<'f'>(ptr_test_value));
  writeln(output, fmt::hex(&ref_test_value));
  writeln(output, fmt::hex<'f', 'l'>(str_ptr_value));
  writeln(output, fmt::bin<'f'>(ptr_test_value));
  writeln(output, fmt::bin(&ref_test_value));
  writeln(output, fmt::bin<'f', 'l'>(str_ptr_value));

  expect_eq(buffer, expected);
}