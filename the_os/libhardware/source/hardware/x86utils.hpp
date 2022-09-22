#pragma once

#include <cstddef>
#include <cstdint>
#include <new>

#include <bits/functexcept.h>

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
      using namespace std;

      if ((std::uintptr_t)value >= 0x100000u)
      { std::__throw_invalid_argument("real_address::from_pointer: pointer >= 0x100000"); }

      return real_address { .off = (uint16_t)(((uintptr_t)value) & 0xfu), 
                            .seg = (uint16_t)(((uintptr_t)value) >> 4u) };
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