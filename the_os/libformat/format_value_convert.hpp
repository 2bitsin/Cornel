#pragma once

#include <concepts>
#include <algorithm>
#include <charconv>

namespace textio::fmt::detail
{

  template <typename Value_type, typename CharT, meta::string Options>
  struct format_value_convert   
  {
    static_assert(sizeof(Value_type*) == 0, "format_value_convert: Unsupported type");
    using char_type = char;

    template <std::output_iterator<char_type> OIterator, typename Value>
    inline static auto apply(OIterator out_iterator, Value&& value) -> OIterator    
    {
      return out_iterator;
    }
  };

  template <std::integral Value, typename CharT, meta::string Options>
  struct format_value_convert<Value, CharT, Options>
  {
    using char_type = char;
		/*
    using flag_list = meta::value_list_from_array<Options>;
    
    static inline constexpr auto is_lower_bin = meta::value_list_contains_v<flag_list, 'b'>;
    static inline constexpr auto is_upper_bin = meta::value_list_contains_v<flag_list, 'B'>;
    
    static inline constexpr auto is_lower_oct = meta::value_list_contains_v<flag_list, 'o'>;
    static inline constexpr auto is_upper_oct = meta::value_list_contains_v<flag_list, 'O'>;
    
    static inline constexpr auto is_lower_hex = meta::value_list_contains_v<flag_list, 'x'>;
    static inline constexpr auto is_upper_hex = meta::value_list_contains_v<flag_list, 'X'>;
    
    static inline constexpr auto is_any_bin = is_lower_bin || is_upper_bin;
    static inline constexpr auto is_any_oct = is_lower_oct || is_upper_oct;
    static inline constexpr auto is_any_hex = is_lower_hex || is_upper_hex;         

    static_assert(is_upper_bin + is_lower_bin + is_upper_oct + is_lower_oct + is_upper_hex + is_lower_hex <= 1, "format_value_convert: Only one of b, B, o, O, x, or X can be specified");

    static inline constexpr auto base_value = is_any_bin ? 2 : (is_any_oct ? 8 : (is_any_hex ? 16 : 10));

    static inline constexpr auto is_lower = is_lower_bin || is_lower_oct || is_lower_hex;
    static inline constexpr auto is_upper = is_upper_bin || is_upper_oct || is_upper_hex;

    static inline constexpr auto is_prefix = meta::value_list_contains_v<flag_list, '#'>;
		*/
    template <std::output_iterator<char_type> OIterator, typename ValueAux>
    inline static auto apply(OIterator out_iterator, ValueAux&& value) -> OIterator   
    {
			/*
      char_type buffer [128];
      const auto result_v = std::to_chars(std::begin(buffer), std::end(buffer), std::forward<Value>(value), base_value);
      if (result_v.ec != std::errc())
      { return out_iterator; }
      
      if constexpr (is_lower)
      { std::transform(std::begin(buffer), result_v.ptr, std::begin(buffer), [](char_type c) { return std::tolower(c); }); }
      else if constexpr (is_upper)
      { std::transform(std::begin(buffer), result_v.ptr, std::begin(buffer), [](char_type c) { return std::toupper(c); }); }
      
      if constexpr (is_prefix)
      {
        if constexpr (is_any_bin)
        { *out_iterator++ = '0'; *out_iterator++ = 'b'; }
        else if constexpr (is_any_oct)
        { *out_iterator++ = '0'; *out_iterator++ = 'o'; }
        else if constexpr (is_any_hex)
        { *out_iterator++ = '0'; *out_iterator++ = 'x'; }
        else 
        { *out_iterator++ = '0'; *out_iterator++ = 'd'; }
      }
      return std::copy(std::begin(buffer), result_v.ptr, out_iterator);
			*/
    }
  };

  template <typename CharT, meta::string Options>
  struct format_value_convert<bool, CharT, Options>
  {
    using char_type = char;
    //using flags = format_flags_parse<bool, Options>;

    static inline constexpr meta::string value_true  { "true"  };
    static inline constexpr meta::string value_false { "false" };

    template <std::output_iterator<char_type> OIterator, typename Value>
    inline static auto apply(OIterator out_iterator, Value&& value) -> OIterator    
    {
      if (value == true) 
      { return std::copy(value_true.begin(), value_true.end(), out_iterator); }
      else
      { return std::copy(value_false.begin(), value_false.end(), out_iterator); }
    }
  };
  
}