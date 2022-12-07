#pragma once

#include <meta/string.hpp>
#include <textio/format/vconvert.hpp>

namespace textio::fmt::detail
{
  template <meta::string String>
  struct format_node_static 
  {
    static inline constexpr auto value = String;
		using char_type = std::remove_cvref_t<decltype(String[0])>;
    
    template <typename... Args>
    inline static auto apply(vconvert_base<char_type>& vconv_r, std::tuple<Args...> const&) -> convert_error
    {     
      return vconv_r.put(value);
    }
  };
}