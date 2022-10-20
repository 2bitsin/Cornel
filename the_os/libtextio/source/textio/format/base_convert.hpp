#pragma once

#include "../meta/value_list.hpp"

namespace textio::fmt::detail
{ 
  template <typename T, auto Value>
  struct digit_to_integer
  {
    static inline constexpr auto value = ([] (auto v) constexpr 
    {
           if (v == '-') return T(-1);
      else if (v == '+') return T(+1);
      else if (v >= '0' && v <= '9') return T(v - '0');
      else if (v >= 'a' && v <= 'z') return T(v - 'a' + 10);
      else if (v >= 'A' && v <= 'Z') return T(v - 'A' + 10);
      else throw std::invalid_argument("Not a valid digit");
    })(Value);
  };
  
  template <typename T, auto Value>
  inline constexpr auto digit_to_integer_v = digit_to_integer<T, Value>::value;
  
  template <typename T>
  struct digit_to_integer_bind
  {
    template <auto Value>
    struct type
    {
      static inline constexpr auto value = digit_to_integer_v<T, Value>;
    };
  };
  
  template <typename T, typename DigitList, auto Base = 10>
  struct digit_convert_base_impl;
  
  
  template <typename T, auto Base, auto Digit0>
  struct digit_convert_base_impl<T, meta::value_list<Digit0>, Base>
  {
    static_assert(Digit0 < Base, "Digit is out of range");
    static inline constexpr T value = T(Digit0);
  };
  
  template <typename T, auto Base, auto Digit0, auto... DigitN>
  struct digit_convert_base_impl<T, meta::value_list<Digit0, DigitN...>, Base>
  {
    static_assert(Digit0 < Base, "Digit is out of range");
    static inline constexpr T value = Base * digit_convert_base_impl<T, meta::value_list<DigitN...>, Base>::value + Digit0;
  };
  
  
  template <typename T, typename CharList, auto Base = 10>  
  static inline constexpr T char_convert_base_v = ([]() constexpr
  {
    if constexpr (std::is_signed_v<T> && meta::value_list_front<CharList> == '-')
    {
      return -T (digit_convert_base_impl<
        std::make_unsigned_t<T>,    
        meta::value_list_reverse<
          meta::value_list_transform<
            meta::value_list_pop_front<CharList>, digit_to_integer_bind<T>::template type>>, 
        Base
      >::value);
    }
    else
    {
      return T (digit_convert_base_impl<T, 
        meta::value_list_reverse<
          meta::value_list_transform<CharList, digit_to_integer_bind<T>::template type>>, 
        Base
      >::value);
    }
  }) ();
     
  template <typename T, meta::string String, auto Base = 10>  
  static inline constexpr T string_convert_base_v = char_convert_base_v<T, meta::value_list_from_array<meta::string_truncate_v<String>>, Base>;
  
}