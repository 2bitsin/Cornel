#include <cstring>
#include <cstdint>
#include <cstddef>

//extern "C"
const void* memchr(const void* ptr, int value, size_t count)
{
  auto src = (const std::uint8_t*)ptr;  
  while (count-- > 0)
  {
    if (*src == value)
      return src;
    src++;
  }
  return nullptr;
}
