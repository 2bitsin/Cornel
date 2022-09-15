#include <memory/block_list.hpp>

#include <string>
#include <memory>

#include "expect.hpp"


int main(int argc, char **grgv)
{
	

	static const std::string expected_output = ">>> head = 00000000, tail = 00000000\r\n00000000: next=( null ), prev=( null ), size=00100000 status=available\r\n>>> head = 00000000, tail = 00000250\r\n00000000: next=00000090, prev=( null ), size=00000090 status=allocated\r\n00000090: next=000000E0, prev=00000000, size=00000050 status=allocated\r\n000000E0: next=00000120, prev=00000090, size=00000040 status=allocated\r\n00000120: next=00000170, prev=000000E0, size=00000050 status=allocated\r\n00000170: next=00000200, prev=00000120, size=00000090 status=allocated\r\n00000200: next=00000250, prev=00000170, size=00000050 status=allocated\r\n00000250: next=( null ), prev=00000200, size=000FFDB0 status=available\r\n>>> head = 00000000, tail = 00000250\r\n00000000: next=00000090, prev=( null ), size=00000090 status=allocated\r\n00000090: next=000000E0, prev=00000000, size=00000050 status=allocated\r\n000000E0: next=00000120, prev=00000090, size=00000040 status=allocated\r\n00000120: next=00000170, prev=000000E0, size=00000050 status=allocated\r\n00000170: next=00000200, prev=00000120, size=00000090 status=allocated\r\n00000200: next=00000250, prev=00000170, size=00000050 status=allocated\r\n00000250: next=( null ), prev=00000200, size=000FFDB0 status=available\r\n>>> head = 00000000, tail = 00000250\r\n00000000: next=00000090, prev=( null ), size=00000090 status=allocated\r\n00000090: next=00000120, prev=00000000, size=00000090 status=available\r\n00000120: next=00000170, prev=00000090, size=00000050 status=allocated\r\n00000170: next=00000200, prev=00000120, size=00000090 status=allocated\r\n00000200: next=00000250, prev=00000170, size=00000050 status=allocated\r\n00000250: next=( null ), prev=00000200, size=000FFDB0 status=available\r\n>>> head = 00000000, tail = 00000250\r\n00000000: next=00000170, prev=( null ), size=00000170 status=available\r\n00000170: next=00000200, prev=00000000, size=00000090 status=allocated\r\n00000200: next=00000250, prev=00000170, size=00000050 status=allocated\r\n00000250: next=( null ), prev=00000200, size=000FFDB0 status=available\r\n>>> head = 00000000, tail = 00000000\r\n00000000: next=( null ), prev=( null ), size=00100000 status=available\r\n";

  auto buffer = std::make_unique<std::byte []> (1024u*1024u);

  block_list dut;

  dut.initialize({buffer.get(), 1024u * 1024u});
	
	std::string output;
	
	pretty_print(dut, std::back_inserter(output));	

	auto a = dut.allocate(128);
	auto b = dut.allocate(64);
	auto c = dut.allocate(48);
	auto d = dut.allocate(64);
	auto e = dut.allocate(128);
	auto f = dut.allocate(64);

	pretty_print(dut, std::back_inserter(output));	
	
	std::memset(a, 0, 128);
	std::memset(b, 0, 64);
	std::memset(c, 0, 48);
	std::memset(d, 0, 64);
	std::memset(e, 0, 128);
	std::memset(f, 0, 64);

	pretty_print(dut, std::back_inserter(output));	

	dut.deallocate(b);	
	dut.deallocate(c);
	
	pretty_print(dut, std::back_inserter(output));	

	dut.deallocate(a);
	dut.deallocate(d);
  
	pretty_print(dut, std::back_inserter(output));	

	dut.deallocate(e);
	dut.deallocate(f);

	pretty_print(dut, std::back_inserter(output));	
	
	
	expect_eq(output, expected_output);
	
	return 0;
}