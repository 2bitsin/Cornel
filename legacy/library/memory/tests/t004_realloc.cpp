#include <memory/block_list.hpp>

#include <string>
#include <memory>

#include "expect.hpp"


int main(int argc, char **grgv)
{ 
  using namespace textio::simple;
  using namespace textio::simple::fmt;
  
  static const std::string expected_output = ">>> head=00000000, tail=00000100\n0x000000: next=00000100, prev=**null**, size=00000100 status=allocated \n0x000100: next=**null**, prev=00000000, size=000fff00 status=available \n>>> head=00000000, tail=00000300\n0x000000: next=00000100, prev=**null**, size=00000100 status=allocated \n0x000100: next=00000300, prev=00000000, size=00000200 status=allocated \n0x000300: next=**null**, prev=00000100, size=000ffd00 status=available \n>>> head=00000000, tail=00000400\n0x000000: next=00000100, prev=**null**, size=00000100 status=allocated \n0x000100: next=00000300, prev=00000000, size=00000200 status=allocated \n0x000300: next=00000400, prev=00000100, size=00000100 status=allocated \n0x000400: next=**null**, prev=00000300, size=000ffc00 status=available \n>>> head=00000000, tail=00000800\n0x000000: next=00000100, prev=**null**, size=00000100 status=allocated \n0x000100: next=00000300, prev=00000000, size=00000200 status=allocated \n0x000300: next=00000400, prev=00000100, size=00000100 status=allocated \n0x000400: next=00000800, prev=00000300, size=00000400 status=allocated \n0x000800: next=**null**, prev=00000400, size=000ff800 status=available \ntrue\r\n>>> head=00000000, tail=00000800\n0x000000: next=00000100, prev=**null**, size=00000100 status=allocated \n0x000100: next=00000300, prev=00000000, size=00000200 status=available \n0x000300: next=00000400, prev=00000100, size=00000100 status=allocated \n0x000400: next=00000800, prev=00000300, size=00000400 status=allocated \n0x000800: next=**null**, prev=00000400, size=000ff800 status=available \n>>> head=00000000, tail=00000800\n0x000000: next=00000280, prev=**null**, size=00000280 status=allocated \n0x000280: next=00000300, prev=00000000, size=00000080 status=available \n0x000300: next=00000400, prev=00000280, size=00000100 status=allocated \n0x000400: next=00000800, prev=00000300, size=00000400 status=allocated \n0x000800: next=**null**, prev=00000400, size=000ff800 status=available \n>>> head=00000000, tail=00000800\n0x000000: next=00000300, prev=**null**, size=00000300 status=allocated \n0x000300: next=00000400, prev=00000000, size=00000100 status=allocated \n0x000400: next=00000800, prev=00000300, size=00000400 status=allocated \n0x000800: next=**null**, prev=00000400, size=000ff800 status=available \n>>> head=00000000, tail=00000800\n0x000000: next=00000200, prev=**null**, size=00000200 status=allocated \n0x000200: next=00000300, prev=00000000, size=00000100 status=available \n0x000300: next=00000400, prev=00000200, size=00000100 status=allocated \n0x000400: next=00000800, prev=00000300, size=00000400 status=allocated \n0x000800: next=**null**, prev=00000400, size=000ff800 status=available \n>>> head=00000000, tail=00000c00\n0x000000: next=00000300, prev=**null**, size=00000300 status=available \n0x000300: next=00000400, prev=00000000, size=00000100 status=allocated \n0x000400: next=00000800, prev=00000300, size=00000400 status=allocated \n0x000800: next=00000c00, prev=00000400, size=00000400 status=allocated \n0x000c00: next=**null**, prev=00000800, size=000ff400 status=available \n";
  std::string output;
  auto out_i = std::back_inserter(output);
  
  block_list dut;
  auto bytes_ptr = std::make_unique<std::byte[]>(0x100000);
  std::span<std::byte> _ (bytes_ptr.get(), 0x100000);

  dut.insert_range(_);
  
  auto a = dut.allocate(0x0F0); std::memset(a, 0xcc, 0x0F0);
  pretty_print(dut, out_i);
  auto b = dut.allocate(0x1F0); std::memset(b, 0xcc, 0x1F0);
  pretty_print(dut, out_i);
  auto c = dut.allocate(0x0F0); std::memset(c, 0xcc, 0x0F0);
  pretty_print(dut, out_i);
  auto d = dut.allocate(0x3F0); std::memset(d, 0xcc, 0x3F0);
  pretty_print(dut, out_i);
  writeln(out_i, dut.deallocate(b));
  pretty_print(dut, out_i);
  a = dut.reallocate(a, 0x270); std::memset(a, 0xcc, 0x270);
  pretty_print(dut, out_i);
  a = dut.reallocate(a, 0x2F0); std::memset(a, 0xcc, 0x2F0);
  pretty_print(dut, out_i);
  a = dut.reallocate(a, 0x1F0); std::memset(a, 0xcc, 0x1F0);
  pretty_print(dut, out_i);
  a = dut.reallocate(a, 0x3F0); std::memset(a, 0xcc, 0x3F0);
  pretty_print(dut, out_i);
  
  expect_eq(output, expected_output);
  
  return 0;
}
