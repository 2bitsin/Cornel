#include <cstring>
#include <cstdint>
#include <cstddef>

//extern "C"
//{
  const void* memchr(const void* ptr, int value, size_t count)
  {
    auto const* bytes = (std::uint8_t const*)(ptr);
    for (std::size_t i = 0u; i < count; ++i)
    {
      if (bytes[i] == value)
        return (void*)(bytes + i);
    }
    return nullptr;
  }
//}