#pragma once

#include "meta_type_list.hpp"
#include "meta_string.hpp"
#include "format_base_convert.hpp"
#include "format_variable.hpp"

namespace textio::fmt::detail
{	
	template <meta::string String, size_t DefaultIndex>
	struct format_node_insert
	: public format_variable<String, DefaultIndex>
	{	
		using variable = format_variable<String, DefaultIndex>;
		
		using variable::argument_index;
		using variable::options_string;
		using variable::next_default_index;
		using variable::uses_default;
				  
	  template <typename Collect, typename... Args>
	  static void print(Collect& collect, std::tuple<Args...> const& args)
	  {
			collect.append("(\u001b[43m");

			collect.append(std::to_string(argument_index));
			collect.append("\u001b[42m:\u001b[45m");
			collect.append(options_string.as_string_view());
			
			collect.append("\u001b[0m)");		
	  }
	};
}