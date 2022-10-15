#pragma once

#include "meta_type_list.hpp"
#include "meta_string.hpp"

namespace textio::fmt::detail
{
	template <meta::string String>
	constexpr auto format_static();
	
	template <meta::string String>
	constexpr auto format_insert()
	{
	  constexpr auto index = String.template find<'}'>();
	  if constexpr (index != String.npos)
		{
	    constexpr auto prev_string = String.template substr<0, index>();
	    constexpr auto next_string = String.template substr<index + 1u, String.size() - (index + 1u)>();
			
			if constexpr (!next_string.empty())
			{ return meta::list_prepend<format_node_insert<prev_string>, decltype (format_static<next_string>())> {}; }
			else 
			{ return meta::type_list<format_node_insert<prev_string>> {}; }
	  }
	  else
	  { return meta::type_list<format_node_insert<String>> {}; }
	}
	
	template <meta::string String>
	constexpr auto format_static() 
	{
	  constexpr auto index = String.template find<'{'>();
		if constexpr (index == String.npos)
	  { return meta::type_list<format_node_static<String>> {}; }   
		else 
		{
			constexpr auto esc = !(String[index + 1u] != '{');
			constexpr auto prev_string = String.template substr<0, index + esc>();
			constexpr auto next_string = String.template substr<index + 1u + esc, String.size() - (index + 1u + esc)>();
			if constexpr (!next_string.empty()) 
			{
				using next_type = std::conditional_t<!esc, decltype (format_insert<next_string>()), 
				                                           decltype (format_static<next_string>())>;
				return meta::list_prepend<format_node_static<prev_string>, next_type> {};
			}
			else 
			{	return meta::type_list<format_node_static<prev_string>> {}; }
		}
	}
}