#include <iterator>
#include <vector>
#include <textio/format.hpp>
#include "expect.hpp"

#include <vfsio/vfat_volume.hpp>
#include <vfsio/cfile_block.hpp>

#ifdef TESTING
#define main t000_vfsio_vfat_mount
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

	vfsio::cfile_block img("000_fat12.img", "r");
	auto volume_p = vfsio::vfat_volume::mount (img);

	
	

  return 0;
}  

