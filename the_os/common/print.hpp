#pragma once

#include <string_view>
#include <initializer_list>
#include <charconv>
#include <type_traits>
#include <concepts>
#include <algorithm>
#include <cctype>

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

  template <std::integral T, int Base, auto... Flags>
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
 
}

void print_attach_output(void (*_output_fun)(char));
void print_detach_output(void (*_output_fun)(char));

template <typename... Args>
void print(Args... args);

void print(char value);
void print(std::string_view value);
void print(std::initializer_list<std::string_view> value);

template <auto Size>
void print(char const (&value)[Size])
{
  print(std::string_view(value, std::size(value)));
}

void print(detail::print::numeric auto value) 
{
  char buffer[256];
  const auto result = std::to_chars(buffer, buffer + sizeof(buffer), value);
  print(std::string_view{ buffer, result.ptr });
}

template <typename T, auto Base, auto... Flags>
void print(fmt::base_impl<T, Base, Flags...> fmt_spec) 
{
  using namespace std::string_view_literals;
  char buffer[256];
  const auto result = std::to_chars(buffer, buffer + sizeof(buffer), fmt_spec.value, Base);

  if constexpr (fmt_spec.flags_upperface) {
    std::transform(buffer, result.ptr, buffer, &std::toupper);
  }

  if constexpr (!fmt_spec.flags_no_padding)
  {
    print(&"0000000000000000"[result.ptr - buffer], 
      std::string_view{ buffer, result.ptr });
  }
  else
  {
    print(std::string_view{ buffer, result.ptr });
  }
}

template <typename... Args>
void print(Args... args)
{
  (print(std::forward<Args>(args)), ...);
}

template <typename... Args>
void println(Args ... args)
{
  print(std::forward<Args>(args)...);
  print('\n');
}