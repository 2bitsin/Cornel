#pragma once

#include <cstdint>
#include <cstddef>

namespace meta
{
  constexpr auto hash32_fnv1a(auto&& input) -> std::uint32_t
  {    
    constexpr auto prime = 0x01000193u;
    constexpr auto offset = 0x811C9DC5u;
    auto hash = offset;
    for (auto&& c : input)
      hash = (hash ^ c) * prime;
    return hash;
  }

  constexpr auto hash64_fnv1a(auto&& input) -> std::uint64_t
  {    
    constexpr auto prime = 0x100000001B3ull;
    constexpr auto offset = 0xCBF29CE484222325ull;
    auto hash = offset;
    for (auto&& c : input)
      hash = (hash ^ c) * prime;
    return hash;
  }

  constexpr auto hash32_fnv1(auto&& input) -> std::uint32_t
  {    
    constexpr auto prime = 0x01000193u;
    constexpr auto offset = 0x811C9DC5u;
    auto hash = offset;
    for (auto&& c : input)
      hash = (hash * prime) ^ c;
    return hash;
  }

  constexpr auto hash64_fnv1(auto&& input) -> std::uint64_t
  {    
    constexpr auto prime = 0x100000001B3ull;
    constexpr auto offset = 0xCBF29CE484222325ull;
    auto hash = offset;
    for (auto&& c : input)
      hash = (hash * prime) ^ c;
    return hash;
  }
}

