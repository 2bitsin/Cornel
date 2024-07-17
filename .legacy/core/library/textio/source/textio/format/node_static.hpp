#pragma once

#include <string_view>
#include <meta/string.hpp>
#include <textio/format/vconvert.hpp>

namespace textio::fmt::detail
{
  template <meta::string String>
  struct format_node_static 
  {    
    using char_type = typename decltype(String)::char_type;
    
    template <typename... Args>
    inline static auto apply(vconvert_base<char_type>& vconv_r, std::tuple<Args...> const&) -> convert_error
    {     
      if constexpr (String.size() > 0) 
      {
        using char_type = typename decltype(String)::char_type;
        static const constexpr auto value_string  __attribute__ ((aligned (1))) = String;
        return vconv_r.put(std::basic_string_view<char_type>{ value_string });
      }
      else 
      {
        return convert_error::none;
      }
    }
  };
}