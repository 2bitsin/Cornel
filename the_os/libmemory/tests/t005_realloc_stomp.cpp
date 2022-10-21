#include <memory/block_list.hpp>
#include <textio/simple.hpp>

#include <string>
#include <memory>

#include "expect.hpp"


int main(int argc, char **grgv)
{ 
  using namespace textio::simple;
  using namespace textio::simple::fmt;
  
  static const std::string expected_output = ">>> head=00000000, tail=00000a00\n0x000000: next=00000100, prev=**null**, size=00000100 status=allocated \n0x000100: next=00000300, prev=00000000, size=00000200 status=allocated \n0x000300: next=00000600, prev=00000100, size=00000300 status=allocated \n0x000600: next=00000a00, prev=00000300, size=00000400 status=allocated \n0x000a00: next=**null**, prev=00000600, size=000ff600 status=available \n>>> head=00000000, tail=00000a00\n0x000000: next=00000100, prev=**null**, size=00000100 status=allocated \n0x000100: next=00000300, prev=00000000, size=00000200 status=allocated \n0x000300: next=00000600, prev=00000100, size=00000300 status=available \n0x000600: next=00000a00, prev=00000300, size=00000400 status=allocated \n0x000a00: next=**null**, prev=00000600, size=000ff600 status=available \n>>> head=00000000, tail=00000a00\n0x000000: next=00000100, prev=**null**, size=00000100 status=allocated \n0x000100: next=00000500, prev=00000000, size=00000400 status=allocated \n0x000500: next=00000600, prev=00000100, size=00000100 status=available \n0x000600: next=00000a00, prev=00000500, size=00000400 status=allocated \n0x000a00: next=**null**, prev=00000600, size=000ff600 status=available \nq[0x000]==11\r\nq[0x001]==11\r\nq[0x1EF]==11\r\nq[0x200]==22\r\n>>> head=00000000, tail=00001000\n0x000000: next=00000100, prev=**null**, size=00000100 status=allocated \n0x000100: next=00000600, prev=00000000, size=00000500 status=available \n0x000600: next=00000a00, prev=00000100, size=00000400 status=allocated \n0x000a00: next=00001000, prev=00000600, size=00000600 status=allocated \n0x001000: next=**null**, prev=00000a00, size=000ff000 status=available \nq[0x000]==11\r\nq[0x001]==11\r\nq[0x1EF]==11\r\nq[0x200]==22\r\nq[0x3EF]==22\r\nq[0x400]==CC\r\n";
  std::string output;
  auto out_i = std::back_inserter(output);
  
  block_list dut;
  auto bytes_ptr = std::make_unique<std::byte[]>(0x100000);
  std::span<std::byte> _ (bytes_ptr.get(), 0x100000);

  for(auto& __ : _) __ = (std::byte)0xcc;

  dut.insert_range(_);
  
  auto a = dut.allocate(0x0F0); std::memset(a, 0x00, 0x0F0);
  auto b = dut.allocate(0x1F0); std::memset(b, 0x11, 0x1F0);
  auto c = dut.allocate(0x2F0); std::memset(c, 0x22, 0x2F0);
  auto d = dut.allocate(0x3F0); std::memset(d, 0x33, 0x3F0);

  pretty_print(dut, out_i); 
  dut.deallocate(c);
  pretty_print(dut, out_i); 
  b = dut.reallocate(b, 0x3F0);
  pretty_print(dut, out_i); 

  {
    std::span q ((std::uint8_t*)b, 0x3F0);
    writeln(out_i, "q[0x000]==", hex<'&'>(q[0x000]));
    writeln(out_i, "q[0x001]==", hex<'&'>(q[0x001]));
    writeln(out_i, "q[0x1EF]==", hex<'&'>(q[0x1EF]));
    writeln(out_i, "q[0x200]==", hex<'&'>(q[0x200]));
  }

  b = dut.reallocate(b, 0x5F0);
  pretty_print(dut, out_i); 
  {
    std::span q ((std::uint8_t*)b, 0x5F0);
    writeln(out_i, "q[0x000]==", hex<'&'>(q[0x000]));
    writeln(out_i, "q[0x001]==", hex<'&'>(q[0x001]));
    writeln(out_i, "q[0x1EF]==", hex<'&'>(q[0x1EF]));
    writeln(out_i, "q[0x200]==", hex<'&'>(q[0x200]));
    writeln(out_i, "q[0x3EF]==", hex<'&'>(q[0x3EF]));
    writeln(out_i, "q[0x400]==", hex<'&'>(q[0x400]));
  }

  expect_eq(output, expected_output);
  
  return 0;
}
