#include <cstdint>
#include <cstddef>

extern "C" 
{
  auto  core_main() -> void;
}

auto core_main() -> void
{
  uint16_t * dest = (uint16_t*)0xb8000;
  int j = 0;
  for(;;) 
  {
    for(auto i = 0; i < 80*25; ++i) 
    {
      dest[i] = ((j + 0x20)&0xff)+0x1400;
      j = (j + 1) % 96;
    }
  }
}