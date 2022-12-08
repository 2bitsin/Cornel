#pragma once

#include <meta/string.hpp>
#include "../general/error.hpp"

#include <charconv>

namespace textio::fmt::detail
{
    
  enum class fmt_align
  {
    none,
    left,
    right,
    center
  };
  
  enum class fmt_type
  {
    none,
    lower_binary,
    upper_binary,
    lower_octal,
    upper_octal,
    lower_decimal,
    upper_decimal,
    lower_hexadecimal, 
    upper_hexadecimal,
    character,
    string,
    lower_pointer,
    upper_pointer,      
    lower_float_hexadecimal,
    upper_float_hexadecimal,
    lower_float_scientific,
    upper_float_scientific,
    lower_float_fixed,
    upper_float_fixed,
    lower_float_geneneral,
    upper_float_geneneral
  };

  template <typename CharT>
  struct format_options
  {    
    using char_type   = CharT;

    static constexpr inline auto fmt_type_from_char (char_type char_v)
    {
      switch (char_v)
      {
      case 'b': return fmt_type::lower_binary;
      case 'B': return fmt_type::upper_binary;
      case 'o': return fmt_type::lower_octal;
      case 'O': return fmt_type::upper_octal;
      case 'd': return fmt_type::lower_decimal;
      case 'D': return fmt_type::upper_decimal;       
      case 'x': return fmt_type::lower_hexadecimal;
      case 'X': return fmt_type::upper_hexadecimal;
      case 'c': return fmt_type::character;
      case 's': return fmt_type::string;
      case 'p': return fmt_type::lower_pointer;
      case 'P': return fmt_type::upper_pointer;
      case 'a': return fmt_type::lower_float_hexadecimal;
      case 'A': return fmt_type::upper_float_hexadecimal;
      case 'e': return fmt_type::lower_float_scientific;
      case 'E': return fmt_type::upper_float_scientific;
      case 'f': return fmt_type::lower_float_fixed;
      case 'F': return fmt_type::upper_float_fixed;
      case 'g': return fmt_type::lower_float_geneneral;
      case 'G': return fmt_type::upper_float_geneneral;
      default: return fmt_type::none;
      }
    }
    
    static constexpr fmt_align fmt_align_from_char(char_type ch)
    {
      switch (ch)
      {
      case '>': return fmt_align::right;
      case '^': return fmt_align::center;
      case '<': return fmt_align::left;
      default: break;
      }
      return fmt_align::none;
    }
    
    fmt_align   direction   { fmt_align::none };
    char_type   fill_char   { ' '             };    
    int32_t     width       { 0               };
    int32_t     precision   { 0               };    
    bool        prefix_base { false           };
    bool        pad_zeros   { false           };
    bool        plus_sign   { false           };
    fmt_type    format_type { fmt_type::none  };
    
    static constexpr inline bool is_digit(char_type char_v)
    {
      return char_v >= '0' && char_v <= '9';
    }

    static constexpr inline bool is_direction(char_type char_v)
    {
      return fmt_align_from_char(char_v) != fmt_align::none;
    }

    static constexpr inline bool is_sign(char_type char_v)
    {
      return meta::string("+-").contains(char_v);
    }

    static constexpr inline bool is_format_char(char_type char_v)
    {
      return fmt_type_from_char(char_v) != fmt_type::none;
    }

    template <size_t Size>
    constexpr format_options(char_type const (&value)[Size])
    : format_options(meta::string(value))
    {}

    constexpr format_options(meta::empty_string_t<char_type> const&)
    {}

    template <size_t Size>
    constexpr format_options(meta::string<Size, char_type> const& value)
    {
      using namespace meta::literals;
      size_t index = 0;
      
      ////////////////////////////////////////////////////////////////////////
      // This is going to be all one long constructor, I apologize,
      // but I'm too tired to fight the compiler right now.
      ///////////////////////////////////////////////////////////////////////

      //////////////////////////
      // Parse algiment and fill
      //////////////////////////
      if (index + 1u < value.size() && is_direction(value[index + 1u]))
      {
        fill_char = value[index];
        index += 1u;
        direction = fmt_align_from_char(value[index]);
        index += 1u;
      }       
      //////////////////////
      // Parse just algiment
      //////////////////////
      else if (index < value.size() && is_direction(value[index]))
      {
        direction = fmt_align_from_char(value[index]);
        index += 1u;
      }
      
      /////////////
      // Parse sign
      /////////////
      if (index < value.size() && is_sign(value[index])) 
      { 
        plus_sign = (value[index] == '+'); 
        index += 1u; 
      }
      
      ////////////////////////////
      // Parse prefix (pound sign)
      ////////////////////////////
      if (index < value.size() && value[index] == '#') 
      {
        index += 1u; 
        prefix_base = true; 
      }
      
      /////////////////
      // Parse zero pad
      /////////////////
      if (index < value.size() && value[index] == '0') 
      { 
        index += 1u; 
        pad_zeros = true; 
      }
      
      ////////////////////////////
      // Parse width of align/pad
      ////////////////////////////
      if (index < value.size() && is_digit(value[index]))
      {
        size_t index0 = index;
        while (index < value.size() && is_digit(value[index])) index += 1u;
        size_t numeric_value = 0;
        if (index - index0) {
          for(auto i = index0; i < index; ++i) {
            numeric_value = numeric_value * 10u + (value[i] - '0');
          }         
        }
        width = numeric_value;
      }
        
      //////////////////////////
      // Parse dot and precision
      //////////////////////////
      if (index + 1u < value.size() && value[index] == '.' && is_digit(value[index + 1u])) 
      {
        index += 1u; 
        size_t index0 = index;
        while (index < value.size() && is_digit(value[index])) index += 1u;
        size_t numeric_value = 0;
        if (index - index0) {
          for(auto i = index0; i < index; ++i) {
            numeric_value = numeric_value * 10u + (value[i] - '0');
          }         
        }
        precision = numeric_value;
      }

      ////////////////////
      // Parse format type
      ////////////////////
      if (index < value.size() && is_format_char(value[index]))
      {
        format_type = fmt_type_from_char(value[index]);
        index += 1u;
      }

      /////////////////////////////
      // Shouldn't be anything left
      /////////////////////////////
      if (index < value.size() && value[index] != 0) {
        ::textio::detail::throw_invalid_argument("Invalid format string: garbage at the end of options string");
      }
    }   
    inline constexpr auto base() const  
      -> std::size_t
    { 
      switch (format_type)
      {
      case fmt_type::lower_pointer: return 16u;
      case fmt_type::upper_pointer: return 16u;
      case fmt_type::lower_hexadecimal: return 16u;
      case fmt_type::upper_hexadecimal: return 16u;
      case fmt_type::lower_decimal: return 10u;
      case fmt_type::upper_decimal: return 10u;
      case fmt_type::lower_octal: return 8u;
      case fmt_type::upper_octal: return 8u;        
      case fmt_type::lower_binary: return 2u;
      case fmt_type::upper_binary: return 2u;       
      case fmt_type::none:;
      default: return 10u;
      }
    }

    inline constexpr auto prefix_string() const  
      -> std::basic_string_view<char_type>
    {
      switch (format_type)
      {
      case fmt_type::lower_pointer: return "0x";
      case fmt_type::upper_pointer: return "0X";
      case fmt_type::lower_hexadecimal: return "0x";
      case fmt_type::upper_hexadecimal: return "0X";
      case fmt_type::lower_decimal: return "0d";
      case fmt_type::upper_decimal: return "0D";
      case fmt_type::lower_octal: return "0o";
      case fmt_type::upper_octal: return "0O";
      case fmt_type::lower_binary: return "0b";
      case fmt_type::upper_binary: return "0B";
      case fmt_type::none: return "0d";
      default: return "0d";
      }     
    }

    inline constexpr auto is_upper() const 
    {
      return format_type == fmt_type::upper_hexadecimal 
          || format_type == fmt_type::upper_pointer ;
    }
    inline constexpr auto is_lower() const 
    {
      return format_type == fmt_type::lower_hexadecimal 
          || format_type == fmt_type::lower_pointer ;
    }
  };
  
}