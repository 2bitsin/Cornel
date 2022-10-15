#pragma once

#include "meta_type_list.hpp"
#include "meta_string.hpp"
#include "format_variable.hpp"

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
            
      //using variable_info = format_variable_parse<prev_string, DefaultIndex>;
      using node_type = format_node_insert<prev_string, void>;
      
      //static constexpr auto next_index = variable_info::uses_default ? DefaultIndex + 1u : DefaultIndex;      
      constexpr auto next_index = DefaultIndex;
      
      if constexpr (!next_string.empty())
      { return meta::list_prepend<node_type, decltype (format_static<next_string, next_index>())> {}; }
      else 
      { return meta::type_list<node_type> {}; }
    }
    else
    { 
      using variable_info = format_variable_parse<String, DefaultIndex>;
      return meta::type_list<format_node_insert<String, variable_info>> {}; 
    }
  }
  
  template <meta::string String, size_t DefaultIndex>
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
        using next_type = std::conditional_t<!esc, decltype (format_insert<next_string, DefaultIndex>()), 
                                                   decltype (format_static<next_string, DefaultIndex>())>;
        return meta::list_prepend<format_node_static<prev_string>, next_type> {};
      }
      else 
      { return meta::type_list<format_node_static<prev_string>> {}; }
    }
  }
}