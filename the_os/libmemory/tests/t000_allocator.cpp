#include <memory/block_list.hpp>

#include <sstream>
#include <memory>

#include "expect.hpp"


int main(int argc, char **grgv)
{
	std::ostringstream sst;

	static const std::string expected_output = "head = 00000000, tail = 00000000\n00000000 : next=( null ) prev=( null ) status=available size=00100000\ntotal_size=00100000, allocated=00000000, available=00100000\n\nhead = 00000000, tail = 000001f0\n00000000 : next=00000080 prev=( null ) status=allocated size=00000080\n00000080 : next=000000c0 prev=00000000 status=allocated size=00000040\n000000c0 : next=000000f0 prev=00000080 status=allocated size=00000030\n000000f0 : next=00000130 prev=000000c0 status=allocated size=00000040\n00000130 : next=000001b0 prev=000000f0 status=allocated size=00000080\n000001b0 : next=000001f0 prev=00000130 status=allocated size=00000040\n000001f0 : next=( null ) prev=000001b0 status=available size=000ffe10\ntotal_size=00100000, allocated=000001f0, available=000ffe10\n\nhead = 00000000, tail = 000001f0\n00000000 : next=00000080 prev=( null ) status=allocated size=00000080\n00000080 : next=000000f0 prev=00000000 status=available size=00000070\n000000f0 : next=00000130 prev=00000080 status=allocated size=00000040\n00000130 : next=000001b0 prev=000000f0 status=allocated size=00000080\n000001b0 : next=000001f0 prev=00000130 status=allocated size=00000040\n000001f0 : next=( null ) prev=000001b0 status=available size=000ffe10\ntotal_size=00100000, allocated=00000180, available=000ffe80\n\nhead = 00000000, tail = 000001f0\n00000000 : next=00000130 prev=( null ) status=available size=00000130\n00000130 : next=000001b0 prev=00000000 status=allocated size=00000080\n000001b0 : next=000001f0 prev=00000130 status=allocated size=00000040\n000001f0 : next=( null ) prev=000001b0 status=available size=000ffe10\ntotal_size=00100000, allocated=000000c0, available=000fff40\n\nhead = 00000000, tail = 00000000\n00000000 : next=( null ) prev=( null ) status=available size=00100000\ntotal_size=00100000, allocated=00000000, available=00100000\n\n";

  auto buffer = std::make_unique<std::byte []> (1024u*1024u);

  block_list dut;

  dut.initialize({buffer.get(), 1024u * 1024u});
	
	pretty_print(dut, sst);
	sst << "\n";

	auto a = dut.allocate(128);
	auto b = dut.allocate(64);
	auto c = dut.allocate(48);
	auto d = dut.allocate(64);
	auto e = dut.allocate(128);
	auto f = dut.allocate(64);

	pretty_print(dut, sst);
	sst << "\n";

	dut.deallocate(b);	
	dut.deallocate(c);
	
	pretty_print(dut, sst);
	sst << "\n";

	dut.deallocate(a);
	dut.deallocate(d);
  
	pretty_print(dut, sst);
	sst << "\n";

	dut.deallocate(e);
	dut.deallocate(f);

	pretty_print(dut, sst);
	sst << "\n";
	const auto result = sst.str();
	expect_eq(result, expected_output);
	
	return 0;
}