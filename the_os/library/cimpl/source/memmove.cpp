#include <cstddef>
#include <cstdint>
#include <cstring>

extern "C" 
void* memmove (void *dest, const void *src, std::size_t len)
{
  auto d = (std::byte*)dest;
  auto s = (std::byte const*)src;
  if (d < s)
  {
    while (len--)
      *d++ = *s++;
  }
  else
  {
    auto lasts = s + (len-1);
    auto lastd = d + (len-1);
    while (len--)
      *lastd-- = *lasts--;
  }
  return dest;
}
