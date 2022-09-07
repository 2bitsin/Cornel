#pragma once

namespace mpl
{

  // is_same_value<...>

  template <auto Lhs, auto Rhs>
  struct is_same_value : std::false_type {};

  template <auto Lhs>
  struct is_same_value<Lhs, Lhs> : std::true_type {};

  template <auto Lhs, auto Rhs>
  static inline constexpr auto is_same_value_v = is_same_value<Lhs, Rhs>::value;


  // contains_value<...>
  template <auto Value, auto... Values>
  struct contains_value;

  template <auto Value, auto First, auto... Values>
  struct contains_value<Value, First, Values...> : contains_value<Value, Values...> {};

  template <auto Value, auto... Values>
  struct contains_value<Value, Value, Values...> : std::true_type {};

  template <auto Value>
  struct contains_value<Value> : std::false_type {};

  template <auto ... Args>
  static inline constexpr auto contains_value_v = contains_value<Args...>::value;
}