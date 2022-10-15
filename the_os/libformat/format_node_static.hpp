#pragma once

#include "meta_string.hpp"

namespace textio::fmt::detail
{
	template <meta::string String>
	struct format_node_static 
	{
	  static inline constexpr auto value = String;
	  
	  template <typename Collect, typename... Args>
	  inline static void print(Collect& collect, std::tuple<Args...> const& args)
	  {
			collect.append("(\u001b[41m").append(value.as_string_view()).append("\u001b[0m)");
	  }
	};
}