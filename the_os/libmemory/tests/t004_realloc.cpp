#include <memory/block_list.hpp>
#include <textio/simple.hpp>

#include <string>
#include <memory>

#include "expect.hpp"


int main(int argc, char **grgv)
{	
	using namespace textio::simple;
	using namespace textio::simple::fmt;
	
	static const std::string expected_output = ">>> head = 00000000, tail = 00002000\r\n00000000: next=00000800, prev=( null ), size=00000100 status=available\r\n00000800: next=00001000, prev=00000000, size=00000100 status=available\r\n00001000: next=00002000, prev=00000800, size=00000100 status=available\r\n00002000: next=( null ), prev=00001000, size=00000100 status=available\r\n>>> head = 00000000, tail = 00001000\r\n00000000: next=00000800, prev=( null ), size=00000100 status=available\r\n00000800: next=00001000, prev=00000000, size=00000100 status=available\r\n00001000: next=( null ), prev=00000800, size=00001100 status=available\r\n>>> head = 00000000, tail = 00000800\r\n00000000: next=00000800, prev=( null ), size=00000100 status=available\r\n00000800: next=( null ), prev=00000000, size=00001900 status=available\r\n>>> head = 00000000, tail = 00000000\r\n00000000: next=( null ), prev=( null ), size=00002100 status=available\r\n";
	std::string output;
	auto out_i = std::back_inserter(output);
	
  block_list dut;
	auto bytes_ptr = std::make_unique<std::byte[]>(0x100000);
	std::span<std::byte> _ (bytes_ptr.get(), 0x100000);

	dut.insert_range(_);
	
	auto a = dut.allocate(0x0F0);
	auto b = dut.allocate(0x1F0);
	auto c = dut.allocate(0x0F0);
	auto d = dut.allocate(0x3F0);
	pretty_print(dut, out_i);
	dut.deallocate(b);
	pretty_print(dut, out_i);
	dut.reallocate(a, 0x270);
	pretty_print(dut, out_i);
	
	
	expect_eq(output, expected_output);
	
	return 0;
}
