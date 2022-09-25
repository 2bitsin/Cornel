#include <memory/block_list.hpp>
#include <textio/simple.hpp>

#include <string>
#include <memory>

#include "expect.hpp"


int main(int argc, char **grgv)
{	
	using namespace textio::simple;
	using namespace textio::simple::fmt;
	
	static const std::string expected_output = ">>> head = 00000000, tail = 00000100\r\n00000000: next=00000100, prev=( null ), size=00000100 status=allocated\r\n00000100: next=( null ), prev=00000000, size=000FFF00 status=available\r\n>>> head = 00000000, tail = 00000300\r\n00000000: next=00000100, prev=( null ), size=00000100 status=allocated\r\n00000100: next=00000300, prev=00000000, size=00000200 status=allocated\r\n00000300: next=( null ), prev=00000100, size=000FFD00 status=available\r\n>>> head = 00000000, tail = 00000400\r\n00000000: next=00000100, prev=( null ), size=00000100 status=allocated\r\n00000100: next=00000300, prev=00000000, size=00000200 status=allocated\r\n00000300: next=00000400, prev=00000100, size=00000100 status=allocated\r\n00000400: next=( null ), prev=00000300, size=000FFC00 status=available\r\n>>> head = 00000000, tail = 00000800\r\n00000000: next=00000100, prev=( null ), size=00000100 status=allocated\r\n00000100: next=00000300, prev=00000000, size=00000200 status=allocated\r\n00000300: next=00000400, prev=00000100, size=00000100 status=allocated\r\n00000400: next=00000800, prev=00000300, size=00000400 status=allocated\r\n00000800: next=( null ), prev=00000400, size=000FF800 status=available\r\ntrue\r\n>>> head = 00000000, tail = 00000800\r\n00000000: next=00000100, prev=( null ), size=00000100 status=allocated\r\n00000100: next=00000300, prev=00000000, size=00000200 status=available\r\n00000300: next=00000400, prev=00000100, size=00000100 status=allocated\r\n00000400: next=00000800, prev=00000300, size=00000400 status=allocated\r\n00000800: next=( null ), prev=00000400, size=000FF800 status=available\r\n>>> head = 00000000, tail = 00000800\r\n00000000: next=00000280, prev=( null ), size=00000280 status=allocated\r\n00000280: next=00000300, prev=00000000, size=00000080 status=available\r\n00000300: next=00000400, prev=00000280, size=00000100 status=allocated\r\n00000400: next=00000800, prev=00000300, size=00000400 status=allocated\r\n00000800: next=( null ), prev=00000400, size=000FF800 status=available\r\n>>> head = 00000000, tail = 00000800\r\n00000000: next=00000300, prev=( null ), size=00000300 status=allocated\r\n00000300: next=00000400, prev=00000000, size=00000100 status=allocated\r\n00000400: next=00000800, prev=00000300, size=00000400 status=allocated\r\n00000800: next=( null ), prev=00000400, size=000FF800 status=available\r\n>>> head = 00000000, tail = 00000800\r\n00000000: next=00000200, prev=( null ), size=00000200 status=allocated\r\n00000200: next=00000300, prev=00000000, size=00000100 status=available\r\n00000300: next=00000400, prev=00000200, size=00000100 status=allocated\r\n00000400: next=00000800, prev=00000300, size=00000400 status=allocated\r\n00000800: next=( null ), prev=00000400, size=000FF800 status=available\r\n>>> head = 00000000, tail = 00000C00\r\n00000000: next=00000300, prev=( null ), size=00000300 status=available\r\n00000300: next=00000400, prev=00000000, size=00000100 status=allocated\r\n00000400: next=00000800, prev=00000300, size=00000400 status=allocated\r\n00000800: next=00000C00, prev=00000400, size=00000400 status=allocated\r\n00000C00: next=( null ), prev=00000800, size=000FF400 status=available\r\n";
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
