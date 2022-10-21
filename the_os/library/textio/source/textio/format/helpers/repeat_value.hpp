#pragma once

#include <concepts>

#include "../../format.hpp"

namespace textio::fmt::helpers::detail
{ 
  struct repeat_value_dynamic_t {};

  static inline constexpr auto repeat_value_dynamic = repeat_value_dynamic_t{};
  
  template <auto Count, typename Value_type, typename Glue_type>  
  struct repeat_value_impl
  {
    const Value_type& value;
    const Glue_type& glue;

    repeat_value_impl(const Value_type& value, const Glue_type& glue)
    : value(value)
    , glue(glue)
    { }
    
    template <std::output_iterator<char> O>
    inline auto format(O out_i) const -> O
    {
      using namespace ::textio::fmt;
      if constexpr (Count > 0)
      {
        out_i = format_to<"{}">(out_i, value);
        for (auto i = 1u; i < Count; ++i) {
          out_i = format_to<"{}{}">(out_i, glue, value);
        }
      }
      return out_i;
    }
  };

  template <auto Count, typename Value_type>
  struct repeat_value_impl<Count, Value_type, void>
  {
    const Value_type& value;

    repeat_value_impl(const Value_type& value)
    : value(value)
    { }
    
    template <std::output_iterator<char> O>
    inline auto format(O out_i) const -> O
    {
      using namespace ::textio::fmt;
      if constexpr (Count > 0)
      {     
        for (auto i = 0u; i < Count; ++i) {
          out_i = format_to<"{}">(out_i, value);
        }
      }
      return out_i;
    }
  };
  
  template <typename Value_type, typename Glue_type>
  struct repeat_value_impl<repeat_value_dynamic, Value_type, Glue_type>
  {
    const std::size_t count;
    const Value_type& value;
    const Glue_type& glue;

    repeat_value_impl(const std::size_t count, const Value_type& value, const Glue_type& glue)
    : count(count)
    , value(value)
    , glue(glue)
    { }

    template <std::output_iterator<char> O>
    inline auto format(O out_i) const -> O
    {
      using namespace ::textio::fmt;
      if (count > 0)
      {
        out_i = format_to<"{}">(out_i, value);
        for (auto i = 1u; i < count; ++i) {
          out_i = format_to<"{}{}">(out_i, glue, value);
        }
      }
      return out_i;
    }   
  };

  template <typename Value_type>
  struct repeat_value_impl<repeat_value_dynamic, Value_type, void>
  {
    const std::size_t count;
    const Value_type& value;

    repeat_value_impl(const std::size_t count, const Value_type& value)
    : count(count)
    , value(value)
    { }
    
    template <std::output_iterator<char> O>
    inline auto format(O out_i) const -> O
    {
      using namespace ::textio::fmt;
      for (auto i = 0u; i < count; ++i) {
        out_i = format_to<"{}">(out_i, value);
      }
      return out_i;
    } 
  };
}

namespace textio::fmt::helpers
{
  template <auto Count, typename Value_type, typename Glue_type>
  static inline auto repeat_value(const Value_type& value, const Glue_type& glue)
    -> detail::repeat_value_impl<Count, Value_type, Glue_type>
  {
    return { value, glue };
  }

  template <auto Count, typename Value_type>
  static inline auto repeat_value(const Value_type& value)
    -> detail::repeat_value_impl<Count, Value_type, void>
  {
    return { value };
  }

  template <typename Value_type, typename Glue_type>
  static inline auto repeat_value(const std::size_t count, const Value_type& value, const Glue_type& glue)
    -> detail::repeat_value_impl<detail::repeat_value_dynamic, Value_type, Glue_type>
  {
    return { count, value, glue };
  }

  template <typename Value_type>
  static inline auto repeat_value(const std::size_t count, const Value_type& value)
    -> detail::repeat_value_impl<detail::repeat_value_dynamic, Value_type, void>
  {
    return { count, value };
  }
}