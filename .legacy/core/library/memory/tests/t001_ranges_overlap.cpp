#include <memory/block_list.hpp>

#include <string>
#include <memory>

#include "expect.hpp"


int main(int argc, char **grgv)
{	
	using namespace textio::simple;
	using namespace textio::simple::fmt;
	
	static const std::string expected_output = "true\r\nfalse\r\nfalse\r\n";
	std::string output;
	auto out_i = std::back_inserter(output);
	
  block_list dut;
	
	writeln(out_i, dut.ranges_overlap(std::span((std::byte*)0, 0x20), std::span((std::byte*)0x10, 0x20)));
	writeln(out_i, dut.ranges_overlap(std::span((std::byte*)0, 0x20), std::span((std::byte*)0x20, 0x20)));
	writeln(out_i, dut.ranges_overlap(std::span((std::byte*)0, 0x10), std::span((std::byte*)0x20, 0x20)));

	expect_eq(output, expected_output);
	
	return 0;
}
