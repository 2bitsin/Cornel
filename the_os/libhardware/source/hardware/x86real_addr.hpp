#pragma once

#include <cstddef>
#include <cstdint>
#include <tuple>
#include <new>

namespace x86arch
{
  #pragma pack(push, 1)
  struct real_address
  {
    static auto from_pointer(void const* value) noexcept -> x86arch::real_address;

    auto to_pointer() const noexcept -> void*;

    template <typename T>
    auto to_pointer() const noexcept -> T*
    { 
      return std::launder((T*)to_pointer()); 
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

  #ifdef __LIBTEXTIO__
    template <std::output_iterator<char> O>
    inline auto write(O out_i) const noexcept -> O
    {
      using ::textio::simple::fmt::hex;
      return ::textio::simple::write(out_i, hex<'&'>(seg), ':', hex<'&'>(off));
    }
  #endif

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
  struct tuple_size<::x86arch::real_address>: std::integral_constant<std::size_t, 2u> {};
  
  template <std::size_t N>
  struct tuple_element<N, ::x86arch::real_address>: std::type_identity<std::uint16_t> {};
}