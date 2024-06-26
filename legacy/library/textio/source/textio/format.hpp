#pragma once

#include <tuple>
#include <concepts>
#include <string>
#include <iterator>

#include <meta/type_list.hpp>
#include <meta/string.hpp>

#include <textio/format/node_static.hpp>
#include <textio/format/node_insert.hpp>
#include <textio/format/optimize.hpp>
#include <textio/format/parse.hpp>
#include <textio/format/variable.hpp>
#include <textio/general/error.hpp>

namespace textio::fmt::detail
{
  template <meta::string String>
  inline constexpr auto format_encode()
  {
    return format_optimize_empty(format_optimize_merge(format_static<meta::string_truncate_v<String>, 0u>()));
  }
    
  template <typename Char_type, typename ArgsTuple, typename... NodeN>
  inline auto format_to_impl(vconvert_base<Char_type>& vconv_r, meta::type_list<NodeN...>, ArgsTuple const& args) -> convert_error
  { 
    convert_error error_v { convert_error::none };
    [[maybe_unused]]
    auto const result_v = (((error_v = NodeN::apply(vconv_r, args)) == convert_error::none) && ...);    
    return error_v;
  }
}

namespace textio::fmt
{
  ///////////////////////////////////////////////////
  // format_to vconvert
  ///////////////////////////////////////////////////
  template <meta::string Format_string = meta::string{"{}"}, typename... ArgN>
  auto format_to(detail::vconvert_base<typename decltype(Format_string)::char_type>& vconv_r, ArgN&&... args_v) -> convert_error
  {
    using namespace detail;
    return format_to_impl(vconv_r, format_encode<Format_string>(), std::forward_as_tuple(std::forward<ArgN>(args_v)...));
  }

  ///////////////////////////////////////////////////
  // format_to FILE*
  ///////////////////////////////////////////////////
  template <meta::string Format_string = meta::string{"{}"}, typename... ArgN>
  auto format_to(FILE* ofile_v, ArgN&&... args_v) -> convert_error
  {
    using namespace detail;
    using char_type = typename decltype(Format_string)::char_type;
    vconvert_cstdio<char_type> vconv_r{ ofile_v };
    return format_to_impl(vconv_r, format_encode<Format_string>(), std::forward_as_tuple(std::forward<ArgN>(args_v)...));   
  }
  
  ///////////////////////////////////////////////////
  // format_to output iterator
  /////////////////////////////////////////////////// 
  template <meta::string Format_string = meta::string{"{}"}, std::output_iterator<typename decltype(Format_string)::char_type> Output_type, typename... ArgN>
  auto format_to(Output_type& output_v, ArgN&&... args_v) -> convert_error
  {
    using namespace detail;
    using char_type = typename decltype(Format_string)::char_type;
    vconvert_iterator<Output_type, char_type> vconv_r{ output_v };
    return format_to_impl(vconv_r, format_encode<Format_string>(), std::forward_as_tuple(std::forward<ArgN>(args_v)...));       
  }
  
  ///////////////////////////////////////////////////
  // format_to std::string
  /////////////////////////////////////////////////// 
  template <meta::string Format_string = meta::string{"{}"}, typename... ArgN>
  auto format_to(std::basic_string<typename decltype(Format_string)::char_type>& ostring_v, ArgN&&... args) -> convert_error
  {
    using namespace detail;
    using char_type = typename decltype(Format_string)::char_type;
    vconvert_back_insert<char_type, std::basic_string<char_type>> vconv_r{ ostring_v };
    return format_to_impl(vconv_r, format_encode<Format_string>(), std::forward_as_tuple(std::forward<ArgN>(args)...));
  }
  
  ///////////////////////////////////////////////////
  // format_as string or other container 
  ///////////////////////////////////////////////////   
  template <meta::string Format_string = meta::string{"{}"}, typename AsWhat = std::string, typename... ArgN>
  auto format_as(ArgN&&... args) -> AsWhat
  {
    using namespace detail;
    AsWhat collect;
    vconvert_back_insert<typename decltype(Format_string)::char_type, AsWhat> vconv_r { collect };
    auto const error_v = format_to_impl(vconv_r, format_encode<Format_string>(), std::forward_as_tuple(std::forward<ArgN>(args)...));
    if (error_v != convert_error::none) {
      //TODO: What to do in case of error ?
      return AsWhat{};
    }
    return collect;
  } 
}

namespace textio::fmt
{ 
  template <meta::string Format_string>
  struct statement
  {
    constexpr statement() = default;
    using char_type = typename decltype(Format_string)::char_type;

    static inline constexpr auto encoded = detail::format_encode<Format_string>();
    
    template <typename... ArgN>
    auto to(FILE* file_v, ArgN&& ... argn) const -> convert_error {
      using namespace detail;
      vconvert_cstdio<char_type> vconv_r{ file_v };
      return format_to_impl(vconv_r, encoded, std::forward_as_tuple(std::forward<ArgN>(argn)...));
    }

    template <std::output_iterator<char_type> Output_type, typename... ArgN>
    auto to(Output_type& output_v, ArgN&& ... argn) const -> convert_error {
      using namespace detail;
      vconvert_iterator<Output_type, char_type> vconv_r{ output_v };
      return format_to_impl(vconv_r, encoded, std::forward_as_tuple(std::forward<ArgN>(argn)...));
    }

    template <typename... ArgN>
    auto to(std::basic_string<char_type>& ostring_v, ArgN&& ... argn) const -> convert_error {
      using namespace detail;
      vconvert_back_insert<char_type, std::basic_string<char_type>> vconv_r{ ostring_v };
      return format_to_impl(vconv_r, encoded, std::forward_as_tuple(std::forward<ArgN>(argn)...));
    }

    template <typename AsWhat = std::string, typename... ArgN>
    auto as(ArgN&& ... argn) const -> AsWhat 
    {
      using namespace detail;
      AsWhat collect;
      vconvert_back_insert<char_type, AsWhat> vconv_r{ collect };
      auto const error_v = format_to_impl(vconv_r, encoded, std::forward_as_tuple(std::forward<ArgN>(argn)...));
      if (error_v != convert_error::none) {
        return AsWhat{};
      }
      return collect;
    }
    
  };

  namespace literals
  {
    template<meta::string Format_string>
    constexpr auto operator "" _fmt()
    {
      return statement<Format_string>();
    }
  }
}