#pragma once

#include <cstddef>
#include <cstdint>
#include <new>

namespace x86arch
{
  #pragma pack(push, 1)
  struct real_address
  {
    template <typename T = void>
    constexpr inline auto to_pointer() const noexcept
    { return std::launder((T*)(seg*0x10u + off)); }

    static constexpr inline auto from_pointer(void const* value) noexcept
    { 
      return real_address {
        .off = (std::uint8_t)(((std::uintptr_t)value) & 0xfu),
        .seg = (std::uint8_t)(((std::uintptr_t)value) >> 4u)   
      };
    }

  #pragma GCC diagnostic push
  #pragma GCC diagnostic ignored "-Wpedantic"
    union
    {
      struct { std::uint16_t off; std::uint16_t seg; };
      struct { std::uint16_t ip;  std::uint16_t cs;  };
    };
  #pragma GCC diagnostic pop
  };
  #pragma pack(pop)
}