#pragma once

#include <cstddef>
#include <cstdint>
#include <concepts>
#include <string_view>
#include <limits>
#include <bit>

#include "../general/helpers.hpp"

namespace textio::simple::fmt
{
  namespace detail
  {
    using namespace std::string_view_literals;

    template <typename T, std::uint8_t Base>
    constexpr auto get_number_of_digits() -> std::size_t
    {
      if (std::has_single_bit(Base)) 
      {
        constexpr auto nbits = std::bit_width(Base - 1u);
        return (sizeof(T) * 8 + nbits - 1) / nbits;       
      }

      if (Base == 10) 
      {
        return std::numeric_limits<T>::digits10 + std::is_signed_v<T> + 1;
      }

      if (Base >= 32) return get_number_of_digits<T, 32>();
      if (Base >= 16) return get_number_of_digits<T, 16>();
      if (Base >=  8) return get_number_of_digits<T,  8>();
      if (Base >=  4) return get_number_of_digits<T,  4>();
      if (Base >=  2) return get_number_of_digits<T,  2>();     
    }


    template <std::uint8_t Base> 
    struct base_traits { static inline constexpr const auto prefix = ""sv; };

    template <> struct base_traits<2>  { static inline constexpr const auto prefix = "0b"sv; };
    template <> struct base_traits<4>  { static inline constexpr const auto prefix = "0q"sv; };
    template <> struct base_traits<8>  { static inline constexpr const auto prefix = "0o"sv; };
    template <> struct base_traits<10> { static inline constexpr const auto prefix = "0d"sv; };
    template <> struct base_traits<16> { static inline constexpr const auto prefix = "0x"sv; };

    template <typename T, std::uint8_t Base>
    struct format_traits: public base_traits<Base>
    {
      static inline constexpr const auto number_of_digits = get_number_of_digits<T, Base>();
    };

    template <std::integral T, std::uint8_t Base, auto... Flags>
    struct format_base: public format_traits<T, Base>
    {
      static inline const constexpr auto base = Base; // chosen base

      // add prefix
      static inline const constexpr auto prefix_flag = ::textio::detail::one_of_v<'f', Flags...>;
      // fixed width
      static inline const constexpr auto fixedw_flag = ::textio::detail::one_of_v<'p', Flags...> || ::textio::detail::one_of_v<'&', Flags...>;  
      // force signed
      static inline const constexpr auto signed_flag = ::textio::detail::one_of_v<'i', Flags...> || std::is_signed_v<T>;      
      // force unsigned
      static inline const constexpr auto nosign_flag = ::textio::detail::one_of_v<'u', Flags...> || ::textio::detail::one_of_v<'&', Flags...>;      
      // force uppercase
      static inline const constexpr auto upperc_flag = ::textio::detail::one_of_v<'U', Flags...> || ::textio::detail::one_of_v<'&', Flags...>;
      // force lowercase
      static inline const constexpr auto lowerc_flag = ::textio::detail::one_of_v<'l', Flags...>;
        
      format_base(T const& value)
      : value(value) 
      { }

      T const& value;     
    };

    template <typename T, auto N>
    requires(N >= 0)
    struct const_repeat_impl
    {
      static inline constexpr const auto times = N; 
      T const& value;
    };

    template <typename T>   
    struct repeat_impl
    {
      T const& value;
      std::uintmax_t times; 
    };


  }

  template <auto... Flags, std::integral T> auto bin(T const& value) -> detail::format_base<T, 2,  Flags...> { return { value }; }
  template <auto... Flags, std::integral T> auto qua(T const& value) -> detail::format_base<T, 4,  Flags...> { return { value }; }
  template <auto... Flags, std::integral T> auto oct(T const& value) -> detail::format_base<T, 8,  Flags...> { return { value }; }
  template <auto... Flags, std::integral T> auto dec(T const& value) -> detail::format_base<T, 10, Flags...> { return { value }; }
  template <auto... Flags, std::integral T> auto hex(T const& value) -> detail::format_base<T, 16, Flags...> { return { value }; }

  template <auto... Flags, typename Q> auto bin(Q const* const (&value)) -> detail::format_base<std::uintptr_t, 2,  '&', Flags...> { return {(std::uintptr_t const&)value}; }
  template <auto... Flags, typename Q> auto qua(Q const* const (&value)) -> detail::format_base<std::uintptr_t, 4,  '&', Flags...> { return {(std::uintptr_t const&)value}; }
  template <auto... Flags, typename Q> auto oct(Q const* const (&value)) -> detail::format_base<std::uintptr_t, 8,  '&', Flags...> { return {(std::uintptr_t const&)value}; }
  template <auto... Flags, typename Q> auto dec(Q const* const (&value)) -> detail::format_base<std::uintptr_t, 10, '&', Flags...> { return {(std::uintptr_t const&)value}; }
  template <auto... Flags, typename Q> auto hex(Q const* const (&value)) -> detail::format_base<std::uintptr_t, 16, '&', Flags...> { return {(std::uintptr_t const&)value}; }

  template <auto N, typename T> auto repeat(T const& value) -> detail::const_repeat_impl<T, N> { return { value }; }
  template <typename T> auto repeat(T const& value, std::uintmax_t times) -> detail::repeat_impl<T> { return { value, times }; }


}