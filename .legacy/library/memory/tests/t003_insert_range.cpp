#include <memory/block_list.hpp>

#include <string>
#include <memory>

#include "expect.hpp"


int main(int argc, char **grgv)
{ 
  using namespace textio::simple;
  using namespace textio::simple::fmt;
  
  static const std::string expected_output = ">>> head=00000000, tail=00002000\n0x000000: next=00000800, prev=**null**, size=00000100 status=available \n0x000800: next=00001000, prev=00000000, size=00000100 status=available \n0x001000: next=00002000, prev=00000800, size=00000100 status=available \n0x002000: next=**null**, prev=00001000, size=00000100 status=available \n>>> head=00000000, tail=00001000\n0x000000: next=00000800, prev=**null**, size=00000100 status=available \n0x000800: next=00001000, prev=00000000, size=00000100 status=available \n0x001000: next=**null**, prev=00000800, size=00001100 status=available \n>>> head=00000000, tail=00000800\n0x000000: next=00000800, prev=**null**, size=00000100 status=available \n0x000800: next=**null**, prev=00000000, size=00001900 status=available \n>>> head=00000000, tail=00000000\n0x000000: next=**null**, prev=**null**, size=00002100 status=available \n";
  std::string output;
  auto out_i = std::back_inserter(output);
  
  block_list dut;
  auto bytes_ptr = std::make_unique<std::byte[]>(0x100000);
  std::span<std::byte> _ (bytes_ptr.get(), 0x100000);
  
  dut.insert_range(_.subspan(0x0800, 0x100));
  dut.insert_range(_.subspan(0x1000, 0x100));
  dut.insert_range(_.subspan(0x2000, 0x100));
  dut.insert_range(_.subspan(0x0000, 0x100));
  pretty_print(dut, out_i);
  dut.insert_range(_.subspan(0x1100, 0xf00));
  pretty_print(dut, out_i);
  dut.insert_range(_.subspan(0x0900, 0x700));
  pretty_print(dut, out_i);
  dut.insert_range(_.subspan(0x0100, 0x700));
  pretty_print(dut, out_i);
  
  expect_eq(output, expected_output);
  
  return 0;
}
