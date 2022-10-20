#pragma once

#include "meta/type_list.hpp"
#include "meta/string.hpp"

namespace textio::fmt::detail
{
  template <meta::string String, size_t DefaultIndex>
  constexpr auto format_static();
  
  template <meta::string String, size_t DefaultIndex>
  constexpr auto format_insert()
  {
    
    constexpr auto index = String.template find<'}'>();
    if constexpr (index != String.npos)
    {
      constexpr auto prev_string = String.template substr<0, index>();
      constexpr auto next_string = String.template substr<index + 1u, String.size() - (index + 1u)>();
                  
      using node_type = format_node_insert<prev_string, DefaultIndex>;      
      
      constexpr auto next_index = node_type::next_default_index;
      
      if constexpr (!next_string.empty())
      { return meta::list_prepend<node_type, decltype (format_static<next_string, next_index>())> {}; }
      else 
      { return meta::list_prepend<node_type> {}; }
    }
    else
    { return meta::list_prepend<format_node_insert<String, DefaultIndex>> {}; }
  }
  
  template <meta::string String, size_t DefaultIndex>
  constexpr auto format_static() 
  {
    constexpr auto index0 = String.template find<'{'>();
		constexpr auto index1 = String.template find<'}'>();

		if constexpr (index0 == String.npos && index1 == String.npos)
		{			
			return meta::list_prepend<format_node_static<String>> {}; 
		}				
    else if constexpr ((index1 != String.npos) && ((index0 != String.npos && index1 < index0) || (index0 == String.npos)))
    { 
			static_assert(String[index1 + 1u] == '}', "Can't spell }, needs to be }}");
			constexpr auto prev_string = String.template substr<0, index1 + 1u>();
			constexpr auto next_string = String.template substr<index1 + 2u, String.size() - (index1 + 2u)>();				
			
			if constexpr (!next_string.empty()) 
			{ return meta::list_prepend<format_node_static<prev_string>, decltype(format_static<next_string, DefaultIndex>())>{}; }
			else
			{ return meta::list_prepend<format_node_static<prev_string>> {}; }
		}   
    else 
    {
      constexpr auto is_escape_sequence = !(String[index0 + 1u] != '{');
      constexpr auto prev_string = String.template substr<0, index0 + is_escape_sequence>();
      constexpr auto next_string = String.template substr<index0 + 1u + is_escape_sequence, String.size() - (index0 + 1u + is_escape_sequence)>();
			
			if constexpr (next_string.empty()) 
			{ return meta::list_prepend<format_node_static<prev_string>> {}; }
			else if constexpr (is_escape_sequence)					
			{ return meta::list_prepend<format_node_static<prev_string>, decltype(format_static<next_string, DefaultIndex>())>{}; }
			else
			{	return meta::list_prepend<format_node_static<prev_string>, decltype(format_insert<next_string, DefaultIndex>())>{}; }
    }
  }
}