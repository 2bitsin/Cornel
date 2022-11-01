#include <cstddef>
#include <cstdint>
#include <cstring>

extern "C" 
void* memmove (void *dest, const void *src, std::size_t len)
{
  return __builtin_memmove(dest, src, len);
}
