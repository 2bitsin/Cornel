#include <memory/block_list.hpp>

#include <string>
#include <memory>

#include "expect.hpp"


int main(int argc, char **grgv)
{
	

	static const std::string expected_output = ">>> head=00000000, tail=00000000\n0x000000: next=**null**, prev=**null**, size=00100000 status=available \n>>> head=00000000, tail=00000250\n0x000000: next=00000090, prev=**null**, size=00000090 status=allocated \n0x000090: next=000000e0, prev=00000000, size=00000050 status=allocated \n0x0000e0: next=00000120, prev=00000090, size=00000040 status=allocated \n0x000120: next=00000170, prev=000000e0, size=00000050 status=allocated \n0x000170: next=00000200, prev=00000120, size=00000090 status=allocated \n0x000200: next=00000250, prev=00000170, size=00000050 status=allocated \n0x000250: next=**null**, prev=00000200, size=000ffdb0 status=available \n>>> head=00000000, tail=00000250\n0x000000: next=00000090, prev=**null**, size=00000090 status=allocated \n0x000090: next=000000e0, prev=00000000, size=00000050 status=allocated \n0x0000e0: next=00000120, prev=00000090, size=00000040 status=allocated \n0x000120: next=00000170, prev=000000e0, size=00000050 status=allocated \n0x000170: next=00000200, prev=00000120, size=00000090 status=allocated \n0x000200: next=00000250, prev=00000170, size=00000050 status=allocated \n0x000250: next=**null**, prev=00000200, size=000ffdb0 status=available \n>>> head=00000000, tail=00000250\n0x000000: next=00000090, prev=**null**, size=00000090 status=allocated \n0x000090: next=00000120, prev=00000000, size=00000090 status=available \n0x000120: next=00000170, prev=00000090, size=00000050 status=allocated \n0x000170: next=00000200, prev=00000120, size=00000090 status=allocated \n0x000200: next=00000250, prev=00000170, size=00000050 status=allocated \n0x000250: next=**null**, prev=00000200, size=000ffdb0 status=available \n>>> head=00000000, tail=00000250\n0x000000: next=00000170, prev=**null**, size=00000170 status=available \n0x000170: next=00000200, prev=00000000, size=00000090 status=allocated \n0x000200: next=00000250, prev=00000170, size=00000050 status=allocated \n0x000250: next=**null**, prev=00000200, size=000ffdb0 status=available \n>>> head=00000000, tail=00000000\n0x000000: next=**null**, prev=**null**, size=00100000 status=available \n";

  auto buffer = std::make_unique<std::byte []> (1024u*1024u);

  block_list dut;

  dut.insert_range({buffer.get(), 1024u * 1024u});
	
	std::string output;
	
	pretty_print(dut, std::back_inserter(output));	

	auto a = dut.allocate(128);
	auto b = dut.allocate(64);
	auto c = dut.allocate(48);
	auto d = dut.allocate(64);
	auto e = dut.allocate(128);
	auto f = dut.allocate(64);

	pretty_print(dut, std::back_inserter(output));	
	
	std::memset(a, 0xcc, 128);
	std::memset(b, 0xcc, 64);
	std::memset(c, 0xcc, 48);
	std::memset(d, 0xcc, 64);
	std::memset(e, 0xcc, 128);
	std::memset(f, 0xcc, 64);

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