#include <iterator>
#include <vector>
#include <filesystem>

#include "expect.hpp"

#include <textio/format.hpp>

#include <vfsio/vfat_volume.hpp>
#include <vfsio/cfile_block.hpp>



#ifdef TESTING
#define main t000_vfsio_vfat_mount
#endif

int main(int,char** const) 
try
{
  using namespace std::string_literals;
  using namespace std::string_view_literals;

	std::filesystem::current_path(R"(C:\Users\alex\Desktop\projects\leisure\Cornel\the_os\library\vfsio\workspace)");

  const auto expected = ""s;
  auto buffer = ""s;

  auto out_i = std::back_inserter(buffer);

	vfsio::cfile_block img("000_fat12.img", "rb");
	auto [error_v, volume_v] = vfsio::vfat_volume::mount (img);
	if (volume_v != nullptr)
	{
		auto root_v = volume_v->open("");
		
		while (true)
		{
			std::byte buff[32];
			auto s = root_v->read(buff);
			std::printf("> %.*s\n", s, buff);
			if (s == 0)
				break;
		}
	}

	
  return 0;
}  
catch (std::exception const& ex)
{
	return 0;
}

