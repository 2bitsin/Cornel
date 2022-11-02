#include <cstdlib>
#include <cstdint>
#include <cstddef>

extern "C"
int memcmp(void const* lhs, void const* rhs, std::size_t count)
{
  auto s1 = (std::uint8_t const *)lhs;
  auto s2 = (std::uint8_t const *)rhs;
  while (count-- > 0)
  {
    if (*s1++ != *s2++)
      return s1[-1] < s2[-1] ? -1 : 1;
  }
  return 0;    
}