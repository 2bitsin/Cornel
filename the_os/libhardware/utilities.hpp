#pragma once 

#include <cstddef>
#include <cstdint>
#include <type_traits>
#include <concepts>

template <std::size_t Address, typename T> auto&& variable_at = *reinterpret_cast<T*>(Address);

constexpr auto quantize_to (std::integral auto m, std::integral auto v)
{
  return ((v + m - 1) / m) * m;
}

template <std::integral auto M>
constexpr auto quantize_to (std::integral auto v)
{
  return ((v + M - 1) / M) * M;
}
