#include <memory/block_list.hpp>

#include <string>
#include <memory>

#include "expect.hpp"


int main(int argc, char **grgv)
{ 
  using namespace textio::simple;
  using namespace textio::simple::fmt;
  
  static const std::string expected_output = "00000010/00000031\r\n00000010/00000036\r\n00000010/00000040\r\n00000000/00000000\r\n00000010/00000008\r\n";
  std::string output;
  auto out_i = std::back_inserter(output);
  
  block_list dut;
  std::span<std::byte> range;
  
  range = dut.align_range(std::span((std::byte*)0x1, 0x40));  
  writeln(out_i, hex<'&'>(range.data()), "/", hex<'&'>(range.size()));
  
  range = dut.align_range(std::span((std::byte*)0x6, 0x40));  
  writeln(out_i, hex<'&'>(range.data()), "/", hex<'&'>(range.size()));
   
  range = dut.align_range(std::span((std::byte*)0x10, 0x40)); 
  writeln(out_i, hex<'&'>(range.data()), "/", hex<'&'>(range.size()));

  range = dut.align_range(std::span((std::byte*)0x8, 0x2)); 
  writeln(out_i, hex<'&'>(range.data()), "/", hex<'&'>(range.size()));

  range = dut.align_range(std::span((std::byte*)0x8, 0x10));  
  writeln(out_i, hex<'&'>(range.data()), "/", hex<'&'>(range.size()));
  
  expect_eq(output, expected_output);
  
  return 0;
}
