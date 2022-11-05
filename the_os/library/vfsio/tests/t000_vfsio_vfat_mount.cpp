#include <iterator>
#include <vector>
#include <filesystem>

#include "expect.hpp"

#include <textio/format.hpp>


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


	
  return 0;
}  
catch (std::exception const& ex)
{
	return 0;
}

