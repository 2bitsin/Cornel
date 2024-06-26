#pragma once

#include <concepts>

#include <textio/format/vconvert.hpp>

namespace textio::fmt::helpers::detail
{ 
  using ::textio::fmt::detail::vconvert_base;

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
    
    template <typename char_type>
    inline auto format(vconvert_base<char_type>& vconv_r) const -> convert_error
    {
      using namespace ::textio::fmt;
      if constexpr (Count > 0)
      {
        convert_error error_v { convert_error::none };
        error_v = format_to<"{}">(vconv_r, value);
        if (error_v != convert_error::none)
          return error_v;
        for (auto i = 1u; i < Count; ++i) {
          error_v = format_to<"{}{}">(vconv_r, glue, value);
          if (error_v != convert_error::none)
            return error_v;
        }
      }
      return convert_error::none;
    }
  };

  template <auto Count, typename Value_type>
  struct repeat_value_impl<Count, Value_type, void>
  {
    const Value_type& value;

    repeat_value_impl(const Value_type& value)
    : value(value)
    { }
    
    template <typename char_type>
    inline auto format(vconvert_base<char_type>& vconv_r) const -> convert_error
    {
      using namespace ::textio::fmt;
      if constexpr (Count > 0)
      {     
        convert_error error_v { convert_error::none };
        for (auto i = 0u; i < Count; ++i) {
          error_v = format_to<"{}">(vconv_r, value);
          if (error_v != convert_error::none)
            return error_v;
        }
      }
      return convert_error::none;
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

    template <typename char_type>
    inline auto format(vconvert_base<char_type>& vconv_r) const -> convert_error
    {
      using namespace ::textio::fmt;
      if (count > 0)
      {
        convert_error error_v { convert_error::none };
        error_v = format_to<"{}">(vconv_r, value);
        if (error_v != convert_error::none)
          return error_v;
        for (auto i = 1u; i < count; ++i) {
          error_v = format_to<"{}{}">(vconv_r, glue, value);
          if (error_v != convert_error::none)
            return error_v;
        }
      }
      return convert_error::none;
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
    
    template <typename char_type>
    inline auto format(vconvert_base<char_type>& vconv_r) const -> convert_error
    {
      using namespace ::textio::fmt;
      if (count > 0)
      {     
        convert_error error_v { convert_error::none };
        for (auto i = 0u; i < count; ++i) {
          error_v = format_to<"{}">(vconv_r, value);
          if (error_v != convert_error::none)
            return error_v;
        }
      }
      return convert_error::none;
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