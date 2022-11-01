#include <iterator>
#include <vector>
#include <textio/format.hpp>
#include "expect.hpp"

#include <vfsio/memory_block.hpp>

#ifdef TESTING
#define main t000_vfsio_memory_block
#endif



int main(int,char** const) 
{
  using namespace std::string_literals;
  using namespace std::string_view_literals;
  using namespace textio::fmt;
  using namespace textio::fmt::literals;

  const auto expected = ""s;
  auto buffer = ""s;

  auto out_i = std::back_inserter(buffer);

	std::vector<std::byte> data(0x100000u, std::byte(0xFFu));
	vfsio::memory_block block(data);

	

  return 0;
}  

