#pragma once

#include "meta_type_list.hpp"
#include "meta_string.hpp"
#include "format_base_convert.hpp"
#include "format_variable.hpp"
#include "format_value_convert.hpp"

namespace textio::fmt::detail
{	
	template <meta::string String, size_t DefaultIndex>
	struct format_node_insert
	: public format_variable<String, DefaultIndex>
	{	
		using char_type = typename decltype(String)::char_type;

		using variable = format_variable<String, DefaultIndex>;
		
		using variable::argument_index;
		using variable::options_string;
		using variable::next_default_index;
		using variable::uses_default;
				  
	  template <typename Collect, typename... Args>
	  inline static auto apply(Collect&& collect, std::tuple<Args...> const& args)
	  {
			using value_type_dry = std::remove_const_t<std::remove_reference_t<std::tuple_element_t<argument_index, std::tuple<Args...>>>>;
			using converter = format_value_convert<value_type_dry, char_type, options_string>;
			return converter::apply(std::forward<Collect>(collect), std::get<argument_index>(args));
		}	  
	};
}