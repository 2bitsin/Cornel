#pragma once

#include <new>
#include <cstddef>
#include <cstdint>
#include <tuple>
#include <concepts>

#include <textio/format/vconvert.hpp>
#include <textio/format.hpp>

namespace x86arch
{
  #pragma pack(push, 1)
  struct address16
  {
    static auto from (void const* value)  -> x86arch::address16;

    template <typename T>
    requires requires (T const& so)
    { 
      { so.seg } -> std::convertible_to<std::uint16_t>;
      { so.off } -> std::convertible_to<std::uint16_t>;
    }
    static auto from(T const& so) -> address16
    {
      return address16 { .off = so.off, .seg = so.seg };
    }

    template <typename T>
    requires requires (T&& what) 
    { 
      { what.data() } -> std::convertible_to<void const*>; 
    } 
    static auto from (T&& what)  -> x86arch::address16
    {
      return from(what.data());
    }

    auto as_void_p() const  -> void*;

    template <typename T>
    auto as() const  -> T
    { 
      return std::launder((T)as_void_p()); 
    }

    template <std::uintmax_t N>
    inline auto get() const 
    {  
      static_assert(N < 2u);
      if constexpr (N == 0u) 
      { return off; }
      else if constexpr (N == 1u)
      { return seg; } 
    }

    template <typename char_type>
    inline auto format(textio::fmt::detail::vconvert_base<char_type>& vconv_r) const 
     -> textio::fmt::detail::convert_error
    {
      using ::textio::fmt::format_to;
      return format_to<"{:04x}:{:04x}">(vconv_r, seg, off);
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