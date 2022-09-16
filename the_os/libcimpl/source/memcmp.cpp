#include <cstdlib>
#include <cstdint>
#include <cstddef>

extern "C"
int memcmp(void const* lhs, void const* rhs, std::size_t count)
{
  auto const* lhs_bytes = (std::uint8_t const*)(lhs);
  auto const* rhs_bytes = (std::uint8_t const*)(rhs);
  for (std::size_t i = 0u; i < count; ++i)
  {
    if (lhs_bytes[i] == rhs_bytes[i])
      continue;
    return lhs_bytes[i] < rhs_bytes[i] ? -1 : 1;
  }
  return 0;
}