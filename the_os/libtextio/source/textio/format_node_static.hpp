#pragma once

#include <concepts>
#include <algorithm>

#include "meta_string.hpp"

namespace textio::fmt::detail
{
	template <meta::string String>
	struct format_node_static 
	{
	  static inline constexpr auto value = String;
	  
	  template <std::output_iterator<char> OIterator, typename... Args>
	  inline static auto apply(OIterator out_iterator, std::tuple<Args...> const& args)
	  {			
			return std::copy(value.begin(), value.end(), out_iterator);
	  }
	};
}