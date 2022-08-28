#pragma once

#include <string_view>
#include <initializer_list>
#include <charconv>
#include <type_traits>
#include <concepts>

namespace detail::display
{
  template <typename T>
  concept numeric = std::is_integral_v<T> || std::is_floating_point_v<T>;
}

namespace fmt
{
  template <std::integral T>
  struct hex
  {
    hex(T const& value) noexcept: value(value) {}
    T const& value;
  };
}

template <typename... Args>
void display(Args... args);


void display(char value);
void display(std::string_view value);
void display(std::initializer_list<std::string_view> value);

void display(detail::display::numeric auto value) 
{
  char buffer[256];
  const auto result = std::to_chars(buffer, buffer + sizeof(buffer), value);
  display(std::string_view{ buffer, result.ptr });
}

template <typename T>
void display(fmt::hex<T> _hex) 
{
  using namespace std::string_view_literals;
  char buffer[256];
  const auto result = std::to_chars(buffer, buffer + sizeof(buffer), _hex.value, 16);
  display("0x"sv, std::string_view{ buffer, result.ptr });
}

template <typename... Args>
void display(Args... args)
{
  (display(args), ...);
}

template <typename... Args>
void displayln(Args ... args)
{
  display(args...);
  display('\n');
}