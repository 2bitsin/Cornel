#pragma once

#include <cstddef>
#include <cstdint>
#include <tuple>
#include <new>
#include <concepts>

#include <textio/format.hpp>

namespace x86arch
{
  #pragma pack(push, 1)
  struct address16
  {
    static auto from (void const* value) noexcept -> x86arch::address16;

    template <typename T>
    requires requires (T&& what) { { what.data() } -> std::convertible_to<void const*>; } 
    static auto from (T&& what) noexcept -> x86arch::address16
    {
      return from(what.data());
    }

    auto to_pointer() const noexcept -> void*;

    template <typename T>
    auto as() const noexcept -> T
    { 
      return std::launder((T)to_pointer()); 
    }

    template <std::uintmax_t N>
    inline auto get() const noexcept
    {  
      static_assert(N < 2u);
      if constexpr (N == 0u) 
      { return off; }
      else if constexpr (N == 1u)
      { return seg; } 
    }

    template <std::output_iterator<char> O>
    inline auto format(O out_i) const -> O
    {
      using ::textio::fmt::format_to;
      return format_to<"{:04x}:{:04x}">(out_i, seg, off);
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

namespace std
{
  template <>  
  struct tuple_size<::x86arch::address16>: std::integral_constant<std::size_t, 2u> {};
  
  template <std::size_t N>
  struct tuple_element<N, ::x86arch::address16>: std::type_identity<std::uint16_t> {};
}