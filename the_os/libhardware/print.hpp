#pragma once

#include <string_view>
#include <span>
#include <initializer_list>
#include <charconv>
#include <type_traits>
#include <concepts>
#include <algorithm>
#include <cctype>
#include <bit>

#include <utilities.hpp>
#include <utilities_mpl.hpp>

namespace detail::print
{
  template <typename T>
  concept numeric = std::is_integral_v<T> || std::is_floating_point_v<T>;
}

namespace fmt
{
  enum flags : std::uint8_t
  {    
    upper, 
    no_padding,
    no_prefix
  };

  template <std::integral T, std::size_t Base, auto... Flags>
  struct base_impl
  {
    static inline constexpr auto base_value = Base;    
    static inline constexpr auto flags_upperface = mpl::contains_value_v<flags::upper, Flags...>;
    static inline constexpr auto flags_no_padding = mpl::contains_value_v<flags::no_padding, Flags...>;
    static inline constexpr auto flags_no_prefix = mpl::contains_value_v<flags::no_prefix, Flags...>;

    base_impl(T const& value) noexcept: value(value) {}
    T const& value;
  };  

  template <auto ... Flags, std::integral T> static inline constexpr auto hex (T const& value) -> base_impl<T, 16, Flags...> { return value; }
  template <auto ... Flags, std::integral T> static inline constexpr auto dec (T const& value) -> base_impl<T, 10, Flags...> { return value; }
  template <auto ... Flags, std::integral T> static inline constexpr auto oct (T const& value) -> base_impl<T, 8,  Flags...> { return value; }
  template <auto ... Flags, std::integral T> static inline constexpr auto bin (T const& value) -> base_impl<T, 2,  Flags...> { return value; }
 
  struct repeat
  {
    repeat(char value, std::size_t times): 
      value(value), 
      times(times) 
    {}

    char value;
    std::size_t times;
  };

}

void print_attach_output(void (*_output_fun)(char));
void print_detach_output(void (*_output_fun)(char));

template <typename... Args>
requires(sizeof...(Args) > 1)
void print(Args&&... args);

void print(char value);
void print(std::string_view value);
void print(std::initializer_list<std::string_view> value);
//void print(std::span<char> value);

template <std::size_t Size>
static inline void print(char const (&value)[Size])
{
 std::string_view value_sv (std::begin(value), std::end(value)); 
 while (value_sv.back() == '\0')
  value_sv.remove_suffix(1);
 print(value_sv);
}

static inline void print(detail::print::numeric auto value) 
{
  std::array<char, 256> buffer_array;
  const auto result = std::to_chars(buffer_array.begin(), buffer_array.end(), value);
  print(std::string_view{ buffer_array.begin(), result.ptr });
}

static inline void print(fmt::repeat fmt_spec)
{
  for(auto i = 0u; i < fmt_spec.times; ++i)
    print(fmt_spec.value);
}

template <typename T, auto Base, auto... Flags>
static inline void print(fmt::base_impl<T, Base, Flags...> fmt_spec) 
{
  using namespace std::string_view_literals;
  std::array<char, 256> buffer_array;
  std::ranges::fill(buffer_array, '@');
  
  const auto result = std::to_chars(buffer_array.begin(), buffer_array.end(), fmt_spec.value, Base);

  if constexpr (!fmt_spec.flags_no_prefix)
  {
    if constexpr (fmt_spec.base_value == 2 ) print("0b"); // Binary
    if constexpr (fmt_spec.base_value == 4 ) print("0q"); // Quaternary
    if constexpr (fmt_spec.base_value == 8 ) print("0o"); // Octal
    if constexpr (fmt_spec.base_value == 10) print("0d"); // Decimal
    if constexpr (fmt_spec.base_value == 16) print("0x"); // Hexadecimal
  }

  if constexpr (std::has_single_bit(fmt_spec.base_value) && !fmt_spec.flags_no_padding) 
  {
    static constexpr auto nbits = std::bit_width(fmt_spec.base_value - 1);
    static constexpr auto width = (sizeof(T) * 8 + nbits - 1) / nbits;
    print(fmt::repeat('0', width - (result.ptr - buffer_array.begin())));
  }

  if constexpr (fmt_spec.flags_upperface) 
  {
    std::transform(buffer_array.begin(), result.ptr, buffer_array.begin(), &std::toupper);
  }

  print(std::string_view {buffer_array.begin(), result.ptr});
}

template <typename... Args>
requires (sizeof...(Args) > 1)
static inline void print(Args&&... args)
{
  (print(std::forward<Args>(args)), ...);
}

template <typename... Args>
static inline void println(Args&&... args)
{
  print(std::forward<Args>(args)...);
  print('\n');
}