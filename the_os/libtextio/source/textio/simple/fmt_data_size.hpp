#pragma once

#include <cstddef>
#include <cstdint>

#include <concepts>
#include <type_traits>

#include "../simple.hpp"

namespace textio::simple::fmt::detail
{
  template <typename T, auto... Flags> 
  struct data_size_impl
  {
    T const& value;

    data_size_impl(T const& value): value(value) { }
    
    template <std::output_iterator<char> O>
    inline auto write(O out_i) const noexcept -> O
    {
      static constexpr auto sz_K = 1024ull;
      static constexpr auto sz_M = 1024ull * sz_K;
      static constexpr auto sz_G = 1024ull * sz_M;
      static constexpr auto sz_T = 1024ull * sz_G;
      static constexpr auto sz_P = 1024ull * sz_T;
      static constexpr auto sz_E = 1024ull * sz_P;

      static constexpr std::uint64_t sz[] = { sz_E, sz_P, sz_T, sz_G, sz_M, sz_K, 1u };
      static constexpr char const* sx[] = { "E", "P", "T", "G", "M", "K", "" };
      
      if (value == 0)
      {
        return ::textio::simple::write(out_i, '0');
      }

      bool first = true;
      for (auto i = 0u; i < std::size(sz); ++i)
      {
        const auto part_value = (value / sz[i]) % 1024u;
        if (0 == part_value)
          continue;
        if (!first)
          out_i = ::textio::simple::write(out_i, ' ', part_value, sx[i]);
        else
          out_i = ::textio::simple::write(out_i, part_value, sx[i]);
        first = false;
      }
      
      return out_i;
    }
  };
}

namespace textio::simple::fmt
{
  template <typename T, auto... Flags>
  requires (std::is_integral_v<T>)
  inline auto data_size(T const& value) 
    -> detail::data_size_impl<T, Flags...>
  {
    return detail::data_size_impl<T, Flags...>(value);
  }
}