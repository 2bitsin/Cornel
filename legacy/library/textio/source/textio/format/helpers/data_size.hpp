#pragma once

#include <cstddef>
#include <cstdint>

#include <concepts>
#include <type_traits>

#include <textio/format/vconvert.hpp>

namespace textio::fmt::helpers::detail
{
  using ::textio::fmt::detail::vconvert_base;
  using ::textio::fmt::detail::convert_error;

  template <typename T, auto... Flags> 
  struct data_size_impl
  {
    T const& value;

    data_size_impl(T const& value): value(value) { }
    
    template <typename char_type>
    auto format(vconvert_base<char_type>& vconv_r) const noexcept -> convert_error
    {
      using namespace ::textio::fmt;

      static constexpr auto sz_K = 1024ull;
      static constexpr auto sz_M = 1024ull * sz_K;
      static constexpr auto sz_G = 1024ull * sz_M;
      static constexpr auto sz_T = 1024ull * sz_G;
      static constexpr auto sz_P = 1024ull * sz_T;
      static constexpr auto sz_E = 1024ull * sz_P;

      static constexpr std::uint64_t sz[] = { sz_E, sz_P, sz_T, sz_G, sz_M, sz_K, 1u };
      static constexpr std::string_view sx[] = { "E", "P", "T", "G", "M", "K", "" };
      
      if (value == 0) {
        return format_to(vconv_r, 0);
      }

      bool first = true;
      convert_error error_v { convert_error::none };
      for (auto i = 0u; i < std::size(sz); ++i)
      {
        const auto part_value = (value / sz[i]) % 1024u;
        if (0 == part_value)
          continue;
        if (!first)
          error_v = format_to<"{}{}{}">(vconv_r, ' ', part_value, sx[i]);
        else
          error_v = format_to<"{}{}">(vconv_r, part_value, sx[i]);
        first = false;
      }      
      return error_v;
    }
  };
}

namespace textio::fmt::helpers
{
  template <typename T, auto... Flags>
  requires (std::is_integral_v<T>)
  inline auto data_size(T const& value) 
    -> detail::data_size_impl<T, Flags...>
  {
    return detail::data_size_impl<T, Flags...>(value);
  }
}