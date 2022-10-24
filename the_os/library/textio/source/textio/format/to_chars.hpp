#pragma once

#include <concepts>
#include <type_traits>
#include <system_error>
#include <cstddef>
#include <cstdint>
#include <tuple>
#include <bit>

#if defined(__GNUC__)
	#define CAN_IT_BE_INLINED __attribute__((noinline))
#elif	defined(_MSC_VER)
	#define	CAN_IT_BE_INLINED __declspec(noinline)
#endif

namespace textio::fmt::detail
{
  template <typename C, bool Lower_case = true, typename T>
  static inline constexpr auto to_digit(T const& value) -> C
  {
    if (value < 10)
      return C(value + '0');
    if constexpr (Lower_case)
      return C(value - 10 + 'a');
    else
      return C(value - 10 + 'A');
  }

  template <typename T>
  static inline constexpr auto maximum_base10_value ()
  {
    if constexpr (sizeof(T) == 8u)
      return (std::uint64_t)10000000000000000000ull;
    else if constexpr (sizeof(T) == 4u)
      return (std::uint32_t)1000000000ul;
    else if constexpr (sizeof(T) == 2u)
      return (std::uint16_t)10000u;
    else if constexpr (sizeof(T) == 1u)
      return (std::uint8_t)100u;
    else {
      static_assert(sizeof(T*) == 0u, "Unsupported type");
    }
  }
     
  template <typename Char_type, bool Lower_case = true, typename T, typename I>
  requires (std::is_integral_v<T> && !std::is_signed_v<T>)
  static inline constexpr auto to_chars_base_10(I curr, I last, T const& value) 
    -> std::tuple<std::errc, I>
  {       
    for (auto div = maximum_base10_value<T>(); div > 0u; div /= 10u)
    {
      if (div > value)
        continue;       
      if (curr == last) 
        return { std::errc::value_too_large, curr };
      *curr++ = to_digit<Char_type, Lower_case>((value / div) % 10u);       
    }
    
    return { std::errc(), curr };
  }
  
  
  template <bool Lower_case = true, typename T, typename I>
  requires (std::is_integral_v<T> && std::is_signed_v<T>)
  static inline constexpr auto to_chars_base_10(I curr, I last, T const& value)
    -> std::tuple<std::errc, I>
  {
    if (curr == last)
      return { std::errc::value_too_large, curr };
    
    if (value < 0)
    {
      *curr++ = '-';
      return to_chars_base_10<Lower_case>(curr, last, std::make_unsigned_t<T>(-value));
    }
    
    return to_chars_base_10<Lower_case>(curr, last, std::make_unsigned_t<T>(value));
  }

  template <typename Char_type, bool Lower_case = true, size_t Base, typename T, typename I>
  requires (std::is_integral_v<T> && !std::is_signed_v<T> && std::has_single_bit(Base) && Base > 1u && Base <= 32u)
  static inline constexpr auto to_chars_base_pow2(I curr, I last, T const& value)
    -> std::tuple<std::errc, I>
  {   
    constexpr auto digit_width = std::bit_width(Base - 1u);
    constexpr auto digit_mask = Base - 1u;
    constexpr auto initial_shift = ((sizeof(T) * 8u + digit_width - 1u) / digit_width) * digit_width;
    
    for (auto current_shift = initial_shift; current_shift > 0u;)
    {
      current_shift -= digit_width;
      if ((T(1u) << current_shift) > value)
        continue;
      if (curr == last)
        return { std::errc::value_too_large, curr };
      *curr++ = to_digit<Char_type, Lower_case>((value >> current_shift) & digit_mask);
    }
    
    return { std::errc(), curr };
  }
  
  template <bool Lower_case = true, size_t Base, typename T, typename I>
  requires (std::is_integral_v<T>)
  CAN_IT_BE_INLINED // ?
  static inline constexpr auto to_chars(I curr, I last, T const& value)
    -> std::tuple<std::errc, I>
  {
    using char_type = std::remove_cvref_t<decltype(*curr)>; 
    
    if (curr == last) 
    {
      return { std::errc::value_too_large, curr };    
    }
    
    if (value == 0u) 
    {
      *curr++ = to_digit<char_type, Lower_case>(0u);
      return { std::errc(), curr };
    }
    
    if constexpr (Base == 10u) 
    {
      return to_chars_base_10<char_type, Lower_case>(curr, last, value);        
    }
    else 
    {
      static_assert(std::has_single_bit(Base) && Base > 1u && Base <= 32u, "Unsupported base.");
      using U = std::make_unsigned_t<T>;
      return to_chars_base_pow2<char_type, Lower_case, Base>(curr, last, (U const&)value);
    }
  }

  
}