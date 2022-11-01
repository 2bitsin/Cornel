#include <cstdlib>
#include <cstdint>
#include <cstddef>

extern "C"
int memcmp(void const* lhs, void const* rhs, std::size_t count)
{
  return __builtin_memcmp(lhs, rhs, count);
}