#include <cstddef>
#include <cstdint>

#include <cstring>

extern "C"
{
  auto strlen (const char* str) -> std::size_t
  {
    auto s = (const uint8_t*)str;
    while (*s)
      s++;
    return s - (const uint8_t*)str;
  }
}