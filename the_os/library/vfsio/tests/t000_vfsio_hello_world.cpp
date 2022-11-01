#include <iterator>
#include <vector>
#include <textio/format.hpp>
#include "expect.hpp"

#include <vfsio/Iblock.hpp>
#include <vfsio/Ivolume.hpp>

#ifdef TESTING
#define main t000_vfsio_hello_world
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



  return 0;
}  

