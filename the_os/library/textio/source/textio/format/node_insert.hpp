#pragma once

#include <meta/type_list.hpp>
#include <meta/string.hpp>
#include <textio/format/base_convert.hpp>
#include <textio/format/variable.hpp>
#include <textio/format/vconvert.hpp>
#include <textio/format/options.hpp>

#include <concepts>


////////////////////////
// 
//	Forward declarations
// 
////////////////////////
namespace textio::fmt
{
  template <typename User_type, meta::string Options>
  struct user_convert;  
}

////////////////////////
// 
//	Some useful concepts
// 
////////////////////////
namespace textio::fmt::detail
{ 
  template <typename Value_type, typename Output_type, meta::string Options>
  concept is_formatable_external = requires (Value_type&& value, Output_type output_v) { 
    { user_convert<std::remove_cvref_t<Value_type>, Options>::apply(output_v, value) } -> std::convertible_to<convert_error>;
  };

  template <typename Value_type, typename Output_type, meta::string Options>
  concept is_formatable_internal_with_options = requires (Value_type const& value, Output_type output_v) { 
    { value.template format<Options>(output_v) } -> std::convertible_to<convert_error>;
  };

  template <typename Value_type, typename Output_type, meta::string Options>
  concept is_formatable_internal_without_options = requires (Value_type const& value, Output_type output_v) { 
    { value.format(output_v) } -> std::convertible_to<convert_error>;
  };	

	template <typename Value_type, typename Output_type, meta::string Options>
	concept is_formatable = is_formatable_internal_without_options<Value_type, Output_type, Options>		
		                   || is_formatable_internal_with_options<Value_type, Output_type, Options>
											 || is_formatable_external<Value_type, Output_type, Options>;
}

////////////////////////
// 
//	format_node_insert
// 
////////////////////////
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

    static inline constexpr auto value = String;
    static inline constexpr auto options = format_options<char_type, options_string>;
          

    template <typename What>
    inline static auto apply(vconvert_base<char_type> &vconv_r, What&& value_v)
    {
			using value_type = std::remove_cvref_t<What>;

      if constexpr (is_formatable_internal_without_options<value_type, vconvert_base<char_type>&, options_string>)
				return value_v.format(vconv_r);
      if constexpr (is_formatable_internal_with_options<value_type, vconvert_base<char_type>&, options_string>)
				return value_v.format<options_string>(vconv_r);
			if constexpr (is_formatable_external<value_type, vconvert_base<char_type>&, options_string>)
				return user_convert<value_type, options_string>::apply(vconv_r, std::forward<What>(value_v));
      else
        return vconv_r.put(std::forward<What>(value_v), options);
    }


    template <typename... Args>
    inline static auto apply(vconvert_base<char_type> &vconv_r, std::tuple<Args...> const& args) -> convert_error
    {
      return apply(vconv_r, std::get<argument_index>(args));      
    }   
  };
}