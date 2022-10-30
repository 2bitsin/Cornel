#pragma once

#include <concepts>
#include <algorithm>
#include <charconv>
#include <string>
#include <string_view>

#include "options.hpp"
#include "to_chars.hpp"
#include "../general/error.hpp"

namespace textio::fmt
{
  
  template <typename User_type, typename Char_type, meta::string Options>
  struct user_convert;
  
}

namespace textio::fmt::detail
{ 
  template <typename Value_type, typename Char_type, meta::string Options>
  struct format_convert
  {
    static_assert(sizeof(Value_type*) == 0, "Unimplemented");
  };
  
  
  template <typename Value_type, typename Char_type, meta::string Options>
  requires requires (Value_type&& value, Char_type* o_i) { 
    { user_convert<std::decay_t<Value_type>, Char_type, Options>::apply(o_i, value) } -> std::convertible_to<Char_type*>;
  }
  struct format_convert<Value_type, Char_type, Options>
  {
    using value_type = Value_type;
    using char_type = Char_type;
    static inline constexpr auto options = format_options<Value_type, Char_type>(Options);
    
    template <std::output_iterator<char> OIterator>
    static inline auto apply(OIterator o_iterator, value_type const& value) -> OIterator
    {
      using user_value_convert_t = user_convert<value_type, char_type, Options>;
      return user_value_convert_t::apply(o_iterator, value);
    }       
  };
  
  template <typename Value_type, typename Char_type, meta::string Options>
  requires requires (Value_type const& value, Char_type* o_i) { 
    { value.template format<Options> (o_i) } -> std::convertible_to<Char_type*>;
  }
  struct format_convert<Value_type, Char_type, Options>
  {
    using value_type = Value_type;
    using char_type = Char_type;
    static inline constexpr auto options = format_options<Value_type, Char_type>(Options);
    
    template <std::output_iterator<char> OIterator>
    static inline auto apply(OIterator o_iterator, value_type const& value) -> OIterator
    {     
      return value.template format<Options>(o_iterator);
    }       
  };

  template <typename Value_type, typename Char_type, meta::string Options>
  requires requires (Value_type const& value, Char_type* o_i) { 
    { value.format (o_i) } -> std::convertible_to<Char_type*>;
  }
  struct format_convert<Value_type, Char_type, Options>
  {
    using value_type = Value_type;
    using char_type = Char_type;
    static inline constexpr auto options = format_options<Value_type, Char_type>(Options);
    
    template <std::output_iterator<char> OIterator>
    static inline auto apply(OIterator o_iterator, value_type const& value) -> OIterator
    {     
      return value.format(o_iterator);
    }       
  };

  template <typename Value_type, typename Char_type, meta::string Options>
  requires requires (Value_type const& value, Char_type* o_i) { 
    { value.format (o_i, Options) } -> std::convertible_to<Char_type*>;
  }
  struct format_convert<Value_type, Char_type, Options>
  {
    using value_type = Value_type;
    using char_type = Char_type;
    static inline constexpr auto options = format_options<Value_type, Char_type>(Options);
    
    template <std::output_iterator<char> OIterator>
    static inline auto apply(OIterator o_iterator, value_type const& value) -> OIterator
    {     
      return value.format(o_iterator, Options);
    }       
  };   

  template <typename Char_type, meta::string Options, typename... Q>  
  struct format_convert<std::basic_string_view<Char_type, Q...>, Char_type, Options>
  {   
    using char_type   = Char_type;
    using value_type  = std::basic_string_view<char_type, Q...>;
        
    static inline constexpr auto options = format_options<value_type, char_type>{ Options };
    
    template <std::output_iterator<char> OIterator>
    static inline auto apply(OIterator o_iterator, value_type const& value) -> OIterator
    {
      //////////////////////////////////////
      // String larger then aligment width ?
      //////////////////////////////////////
      if (options.width < value.size()) {
        return std::copy(value.begin(), value.end(), o_iterator);
      }

      /////////////////////////////////////////
      // Output an aligned string by padding it
      /////////////////////////////////////////
      const auto total_padding = options.width - value.size();
      switch (options.direction)
      {
      case fmt_align::left:
        o_iterator = std::copy(value.begin(), value.end(), o_iterator);
        return std::fill_n(o_iterator, total_padding, options.fill_char);
      case fmt_align::right:
        o_iterator = std::fill_n(o_iterator, total_padding, options.fill_char);
        return std::copy(value.begin(), value.end(), o_iterator);
      case fmt_align::center:
        {
          const auto left_padding = total_padding / 2;
          const auto right_padding = total_padding - left_padding;
          o_iterator = std::fill_n(o_iterator, left_padding, options.fill_char);
          o_iterator = std::copy(value.begin(), value.end(), o_iterator);
          return std::fill_n(o_iterator, right_padding, options.fill_char);
        }
      case fmt_align::none:
      default:
        return std::copy(value.begin(), value.end(), o_iterator);
      }
    }     
  };   

  template <typename Value_type, typename Char_type, meta::string Options>  
  struct format_convert<Value_type*, Char_type, Options>
  {   
    using char_type   = Char_type;
    using value_type  = Value_type*;

    static inline constexpr auto options = format_options<value_type, char_type>{ Options };

    template <std::output_iterator<char> OIterator>
    static inline auto apply(OIterator o_iterator, value_type const& value) -> OIterator
    {
      static constexpr bool is_integral_format = (
          options.format_type == fmt_type::lower_pointer 
        ||options.format_type == fmt_type::upper_pointer
        ||options.format_type == fmt_type::lower_binary
        ||options.format_type == fmt_type::upper_binary
        ||options.format_type == fmt_type::lower_octal
        ||options.format_type == fmt_type::upper_octal
        ||options.format_type == fmt_type::lower_decimal
        ||options.format_type == fmt_type::upper_decimal
        ||options.format_type == fmt_type::lower_hexadecimal
        ||options.format_type == fmt_type::upper_hexadecimal);

      static constexpr bool is_string_format = 
        std::is_same_v<std::remove_const_t<Value_type>, char_type> && 
          (options.format_type == fmt_type::string 
         ||options.format_type == fmt_type::none);

      if constexpr (is_string_format)
      {
        using convert = format_convert<std::basic_string_view<char_type>, char_type, Options>;
        if (nullptr == value) 
        { return convert::apply(o_iterator, { "null" }); }
        else
        { return convert::apply(o_iterator, { value }); }
      }
      else if constexpr (is_integral_format)
      {
        using convert = format_convert<std::uintptr_t, char_type, Options>;
        return convert::apply(o_iterator, (std::uintptr_t)value);
      }      
      else 
      {
        static_assert(sizeof(value_type)==0, "Not a suitable format type for pointer");
      }
      return o_iterator;
    } 
  }; 

  template <typename Char_type, meta::string Options, typename... Q>  
  struct format_convert<std::basic_string<Char_type, Q...>, Char_type, Options>
  {   
    using char_type   = Char_type;
    using value_type  = std::basic_string<char_type, Q...>;
        
    template <std::output_iterator<char> OIterator>
    static inline auto apply(OIterator o_iterator, value_type const& value) -> OIterator
    {
      using format_sv = format_convert<std::basic_string_view<char_type>, char_type, Options>;
      return format_sv::apply(o_iterator, value);
    }     
  }; 

  template <typename Char_type, meta::string Options, size_t N>  
  struct format_convert<Char_type[N], Char_type, Options>
  {   
    using char_type   = Char_type;
    using value_type  = Char_type[N];
        
    template <std::output_iterator<char> OIterator>
    static inline auto apply(OIterator o_iterator, value_type const& value) -> OIterator
    {
      using format_sv = format_convert<std::basic_string_view<char_type>, char_type, Options>;
      return format_sv::apply(o_iterator, value);
    }     
  }; 

  template <typename Char_type, meta::string Options, size_t N>  
  struct format_convert<meta::string<N, Char_type>, Char_type, Options>
  {   
    using char_type   = Char_type;
    using value_type  = Char_type[N];
        
    template <std::output_iterator<char> OIterator>
    static inline auto apply(OIterator o_iterator, value_type const& value) -> OIterator
    {
      using format_sv = format_convert<std::basic_string_view<char_type>, char_type, Options>;
      return format_sv::apply(o_iterator, value);
    }     
  }; 
  
  template <typename Value_type, typename Char_type, meta::string Options>
  requires (std::is_integral_v<Value_type>)
  struct format_convert<Value_type, Char_type, Options>
  {   
    using value_type  = Value_type;
    using char_type   = Char_type;
    
    static inline constexpr auto options = format_options<value_type, char_type>{ Options };        
    static inline constexpr auto min_number_buffer_size = sizeof(value_type) * 8u * 2u;
    static inline constexpr auto min_string_buffer_size = std::max<std::size_t>(options.pad_zeros * options.width + options.prefix_base * 2u + 1u, min_number_buffer_size);

    template <std::output_iterator<char> OIterator>
    static inline auto apply(OIterator o_iterator, value_type const& value) -> OIterator
    {   
      using format_sv = format_convert<std::basic_string_view<char_type>, char_type, Options>;

      ///////////////////////////////
      // Take care of 'c' format type
      ///////////////////////////////
      if constexpr (std::is_same_v<value_type, char_type> || options.format_type == fmt_type::character)
      {       
        char_type buff [1u];
        if constexpr (!std::is_same_v<value_type, char_type>)
          buff[0u] = static_cast<char_type>(value);
        else
          buff[0u] = value;       
        return format_sv::apply(o_iterator, typename format_sv::value_type{ buff, std::size(buff) });
      }
      
      ////////////////////////////////////
      // Take care of 'bodxX' format types
      ////////////////////////////////////
      
      std::size_t output_count { 0 };
      char_type buffer [min_string_buffer_size] { 0 };
      auto buffer_i { std::begin(buffer) };
      char_type number [min_number_buffer_size] { 0 };

      //////////////////////////////////////////////////////////
      // Sign sign is already taken care of, make value unsigned
      //////////////////////////////////////////////////////////      
      using value_u_type = std::make_unsigned_t<value_type>;      
      const auto value_u = ([](auto value) constexpr -> value_u_type {
        if constexpr (std::is_signed_v<value_type>)
          return static_cast<value_u_type>(value < 0 ? -value : value);
        else
          return value;       
      })(value);
            
      /////////////////////////////////////////
      // Print the number into temporary buffer
      /////////////////////////////////////////
    #ifdef USE_STD_TO_CHARS
      auto [number_e, errc] = std::to_chars(std::begin(number), std::end(number), value_u, options.base());
    #else
      auto [errc, number_e] = detail::to_chars<options.is_lower(), options.base()>(std::begin(number), std::end(number), value_u);
    #endif  
      if (errc != std::errc{}) {
        using ::textio::detail::throw_conversion_error;
        throw_conversion_error("to_chars failed to convert value to string");
      }     
      output_count += (number_e - std::begin(number));
                  
      ////////////////////
      // Take care of sign 
      ////////////////////            
      if constexpr (options.plus_sign) {
        if (value >= 0) {
          ++output_count;
          *buffer_i++ = '+';          
        }
      }
      
      if (value < 0) {
        ++output_count;
        *buffer_i++ = '-';
      }         
      
      /////////////////////////////
      // Take care of adding prefix
      /////////////////////////////     
      if constexpr (options.prefix_base 
        || options.format_type == fmt_type::lower_pointer 
        || options.format_type == fmt_type::upper_pointer) 
      {
        constexpr auto prefix = options.prefix_string();
        buffer_i = std::copy(prefix.begin(), prefix.end(), buffer_i);
        output_count += prefix.size();
      }

      ////////////////////////////
      // Take care of zero padding
      ////////////////////////////
      if constexpr (options.pad_zeros && options.direction == fmt_align::none) {
        if (output_count < options.width) {
          buffer_i = std::fill_n(buffer_i, options.width - output_count, char_type('0'));
        }
      }

      /////////////////////////////////////////////
      // Lastly, copy the prepared number to buffer
      /////////////////////////////////////////////
      buffer_i = std::copy(std::begin(number), number_e, buffer_i);
      
      ////////////////////////////////////////////////////////////////
      // Now pass it along to string_view formatter to adjust aligment
      ////////////////////////////////////////////////////////////////
      return format_sv::apply(o_iterator, typename format_sv::value_type{ std::begin(buffer), buffer_i });
    }
  }; 

  template <typename Char_type, meta::string Options>
  struct format_convert<bool, Char_type, Options>
  {   
    using value_type  = bool;
    using char_type   = Char_type;
    
    static inline constexpr auto options = format_options<value_type, char_type>{ Options };        
    static inline constexpr auto min_number_buffer_size = sizeof(value_type) * 8u * 2u;
    static inline constexpr auto min_string_buffer_size = std::max<std::size_t>(options.pad_zeros * options.width + options.prefix_base * 2u + 1u, min_number_buffer_size);
    
    template <std::output_iterator<char> OIterator>
    static inline auto apply(OIterator o_iterator, value_type const& value) -> OIterator
    {
      ///////////////////////////////////////////////
      // Take care of 's' format in the case of bool
      ///////////////////////////////////////////////
      if constexpr (options.format_type == fmt_type::string)
      {                 
        using format_sv = format_convert<std::basic_string_view<char_type>, char_type, Options>;
        if (value == true) {
          constexpr auto true_s = meta::string{ "true"  };
          return format_sv::apply(o_iterator, true_s);
        }

        constexpr auto false_s = meta::string{ "false" };
        return format_sv::apply(o_iterator, false_s);
      }
      else
      {
        using format_i = format_convert<unsigned, char_type, Options>;
        return format_i::apply(o_iterator, value);
      }     
    }
  };
}