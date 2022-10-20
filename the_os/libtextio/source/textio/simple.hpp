#pragma once

#include <cassert>
#include <utility>
#include <algorithm>
#include <ranges>
#include <iterator>
#include <string_view>
#include <functional>
#include <limits>
#include <charconv>
#include <cctype>
#include <tuple>

#include "general/cstdio_iterator.hpp"
#include "general/error.hpp"
#include "general/helpers.hpp"
#include "general/consts.hpp"
#include "simple/fmt.hpp"

#define __LIBTEXTIO__ 1

namespace textio::simple
{
  template <std::output_iterator<char> I, typename... Args>
  requires (sizeof...(Args) > 1)
  static inline auto write(I out_i, Args&&... args) -> I;
  
  /*******************************/
  /*                             */ 
  /* Individual write prototypes */
  /*                             */ 
  /*******************************/


  template <std::output_iterator<char> I, typename... Q>
  static inline auto write(I out_i, std::tuple<Q...> const& what) -> I;

  template <std::output_iterator<char> I, std::size_t N>
  static inline auto write(I out_i, char const (&value) [N]) -> I;

  template <std::output_iterator<char> I, std::size_t N>
  static inline auto write(I out_i, char const* value) -> I;

  template <std::output_iterator<char> I, std::integral Arg0> 
  static inline auto write(I out_i, Arg0 const& arg0) -> I;

  template <std::output_iterator<char> I, typename T, auto N>
  static inline auto write(I out_i, fmt::detail::const_repeat_impl<T, N> what) -> I;

  template <std::output_iterator<char> I, typename T>
  static inline auto write(I out_i, fmt::detail::repeat_impl<T> what) -> I;

  template <std::output_iterator<char> I, typename T, auto Base, auto... Flags>
  static inline auto write(I out_i, fmt::detail::format_base<T, Base, Flags...> what) -> I;

  template <std::output_iterator<char> I, typename T>
  requires (!std::is_same_v<T, char>)
  static inline auto write(I out_i, T const* const what) -> I;

  template <std::output_iterator<char> I, ::textio::detail::is_writable<I> T>
  static inline auto write(I out_i, T&& what)->I;
  

  /************************************/
  /*                                  */  
  /* Individual write implementations */
  /*                                  */  
  /************************************/

  template <std::output_iterator<char> I>
  static inline auto write(I out_i, std::string_view value) -> I
  {
    for(auto subv: value)
      out_i = write(out_i, subv);
    return out_i;
  }

  template <std::output_iterator<char> I, std::size_t N>
  static inline auto write(I out_i, char const (&value) [N]) -> I
  {   
    using std::string_view;
    return write(out_i, string_view(value));
  }

  template <std::output_iterator<char> I, std::size_t N>
  static inline auto write(I out_i, char const* value) -> I
  {   
    using std::string_view;
    return write(out_i, string_view(value));
  }
  
  template <std::output_iterator<char> I, std::integral Arg0> 
  static inline auto write(I out_i, Arg0 const& arg0) -> I
  {
    if constexpr (std::is_same_v<Arg0, char>)
    {
      *out_i++ = arg0;
      return out_i;
    }
    else if constexpr (std::is_same_v<Arg0, bool>)
    {
      return write(out_i, arg0 ? "true" : "false");
    }
    else
    {
      static const constexpr auto length = std::numeric_limits<Arg0>::digits10 + 2;
      char buffer [length+200];
      static_assert(length > 1);
      std::to_chars_result result;
      result = std::to_chars(std::begin(buffer), std::end(buffer), arg0);
      if (result.ec != std::errc()) 
      {
        using ::textio::detail::throw_conversion_error;
        throw_conversion_error("std::to_chars failed to convert integral type to string");
      }
      return write(out_i, std::string_view(buffer, result.ptr));
    }   
  } 

  template <std::output_iterator<char> I, typename T, auto N>
  static inline auto write(I out_i, fmt::detail::const_repeat_impl<T, N> what) -> I
  {   
    for(auto i = 0u; i < what.times; ++i) {
      out_i = write(out_i, what.value);
    }
    return out_i;
  }

  template <std::output_iterator<char> I, typename T>
  static inline auto write(I out_i, fmt::detail::repeat_impl<T> what) -> I
  {   
    for(auto i = 0u; i < what.times; ++i) {
      out_i = write(out_i, what.value);
    }
    return out_i;
  }

  template <std::output_iterator<char> I, typename T, auto Base, auto... Flags>
  static inline auto write(I out_i, fmt::detail::format_base<T, Base, Flags...> what) -> I
  {   
    char buffer [what.number_of_digits];

    std::to_chars_result result;
    if constexpr (what.signed_flag && !what.nosign_flag) 
    {
      auto const& value = (std::make_signed_t<T> const&)what.value;
      result = std::to_chars(std::begin(buffer), std::end(buffer), std::abs(value), what.base);
      if (result.ec != std::errc()) 
      {
        using ::textio::detail::throw_conversion_error;
        throw_conversion_error("std::to_chars failed to convert integral type to string");
      }     
      if (value < 0) 
        out_i = write(out_i, '-');
    }
    else 
    {
      auto const& value = (std::make_unsigned_t<T> const&)what.value;
      result = std::to_chars(std::begin(buffer), std::end(buffer), value, what.base);
      if (result.ec != std::errc()) 
      {
        using ::textio::detail::throw_conversion_error;
        throw_conversion_error("std::to_chars failed to convert integral type to string");
      }
    }
    std::string_view value_s{ std::begin(buffer), result.ptr };
    if (what.upperc_flag) std::ranges::transform(buffer, std::begin(buffer), [] (auto c) { return std::toupper(c); });
    if (what.lowerc_flag) std::ranges::transform(buffer, std::begin(buffer), [] (auto c) { return std::tolower(c); });
    if (what.prefix_flag) out_i = write(out_i, what.prefix);    
    if (what.fixedw_flag) out_i = write(out_i, fmt::repeat('0', what.number_of_digits - value_s.size()));
    out_i = write(out_i, value_s);
    return out_i;
  }


  template <std::output_iterator<char> I, typename T>
  requires (!std::is_same_v<T, char>)
  static inline auto write(I out_i, T const* const what) -> I
  {
    return write(out_i, fmt::hex<'&','x'>(what));
  } 
  
  namespace detail
  {
    template <std::output_iterator<char> I, typename Tuple, std::size_t... N>
    static inline auto write(I out_i, Tuple const& what, std::index_sequence<N...>) -> I
    {
      ((out_i = ::textio::simple::write(out_i, ',', ' ', std::get<N + 1>(what))), ...);
      return out_i;
    }
  }

  template <std::output_iterator<char> I, typename... Q>
  static inline auto write(I out_i, std::tuple<Q...> const& what) -> I
  {
    out_i = write(out_i, '(');
    if constexpr (sizeof... (Q) > 0) {
      out_i = write(out_i, std::get<0>(what));
    }
    if constexpr (sizeof... (Q) > 1) {
      out_i = detail::write(out_i, what, std::make_index_sequence<sizeof...(Q) - 1>());
    }
    out_i = write(out_i, ')');
    return out_i;
  }

  /*****************************/
  /*                           */ 
  /* Catch-all write overloads */
  /*                           */ 
  /*****************************/

  template <std::output_iterator<char> I, ::textio::detail::is_writable<I> T>
  static inline auto write(I out_i, T&& what)->I
  {
    return what.write(out_i);
  }

  template <std::output_iterator<char> I, typename... Args>
  requires (sizeof...(Args) > 1)
  static inline auto write(I out_i, Args&&... args) -> I
  {       
    ((out_i = write(out_i, std::forward<Args>(args))),...);
    return out_i;
  }

  template <std::output_iterator<char> I, typename... Args>
  requires (sizeof...(Args) >= 1)
  static inline auto writeln(I out_i, Args&&... args) -> I
  {   
    return write(out_i, std::forward<Args>(args)..., consts::crlf_s);
  }

  template <typename... Args>
  static inline auto writeln_to(std::FILE* o_file, Args&&... args) -> int
  {
    using ::textio::detail::cstdio_iterator;
    auto cstdio_i = writeln(cstdio_iterator{ o_file }, std::forward<Args>(args)...);
    return cstdio_i.status();
  }

  template <typename... Args>
  static inline auto write_to(std::FILE* o_file, Args&&... args) -> int
  {
    using ::textio::detail::cstdio_iterator;
    auto cstdio_i = write(cstdio_iterator{ o_file }, std::forward<Args>(args)...);
    return cstdio_i.status();
  }
}