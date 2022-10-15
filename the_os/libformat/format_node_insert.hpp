#pragma once

#include "meta_type_list.hpp"
#include "meta_string.hpp"
#include "format_base_convert.hpp"

namespace textio::fmt::detail
{	
	template <meta::string String, typename VariableInfo>
	struct format_node_insert		             
	{		
		static inline constexpr auto value = String;	
				  
	  template <typename Collect, typename... Args>
	  static void print(Collect& collect, std::tuple<Args...> const& args)
	  {
			collect.append("\u001b[43m").append(value.as_string_view()).append("\u001b[0m");		
	  }
	};
}