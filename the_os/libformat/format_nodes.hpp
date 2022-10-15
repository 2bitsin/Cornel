#pragma once

#include "meta_type_list.hpp"
#include "meta_string.hpp"

namespace textio::fmt::detail
{
	template <meta::string String>
	struct format_node_static 
	{
	  static inline constexpr auto value = String;
	  
	  template <typename Collect, typename... Args>
	  static void print(Collect& collect, std::tuple<Args...> const& args)
	  {
			collect.append(value.as_string_view());
	  }
	};
	
	template <meta::string String>              
	struct format_node_insert
	{		
	  static inline constexpr auto value = String;
	  
	  template <typename Collect, typename... Args>
	  static void print(Collect& collect, std::tuple<Args...> const& args)
	  {
			collect.append("[[").append(value.as_string_view()).append("]]");		
	  }
	};
}