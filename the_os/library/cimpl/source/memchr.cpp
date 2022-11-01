#include <cstring>
#include <cstdint>
#include <cstddef>

//extern "C"
const void* memchr(const void* ptr, int value, size_t count)
{
  return __builtin_memchr(ptr, value, count);
}
