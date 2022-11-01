#pragma once

#include <cstddef>
#include <cstdint>

#include <textio/format.hpp>

namespace vfsio
{
  enum class error: int
  {
    success           =  0,
    not_implemented   = -1,
    not_supported     = -2,
    path_not_found    = -3,
    access_denied     = -4,
    invalid_argument  = -5,    
    invalid_mode      = -6,
    out_of_bounds     = -7    
  }; 
}

namespace textio::fmt
{
  template <typename Char_type, meta::string Options>
  struct user_convert<vfsio::error, Char_type, Options>
  {
    template <std::output_iterator<Char_type> OIterator>
    static inline auto apply(OIterator out_i, vfsio::error value_v) -> OIterator
    {
      switch (value_v) {
      case vfsio::error::success:           return format_to<"{}({})">(out_i, "error::success",          (int)value_v);
      case vfsio::error::not_implemented:   return format_to<"{}({})">(out_i, "error::not_implemented",  (int)value_v);
      case vfsio::error::not_supported:     return format_to<"{}({})">(out_i, "error::not_supported",    (int)value_v);
      case vfsio::error::path_not_found:    return format_to<"{}({})">(out_i, "error::path_not_found",   (int)value_v);
      case vfsio::error::access_denied:     return format_to<"{}({})">(out_i, "error::access_denied",    (int)value_v);
      case vfsio::error::invalid_argument:  return format_to<"{}({})">(out_i, "error::invalid_argument", (int)value_v);
      case vfsio::error::invalid_mode:      return format_to<"{}({})">(out_i, "error::invalid_mode",     (int)value_v);
      default:                              return format_to<"error({})">(out_i, (int)value_v);
      }      
    }

  }; 
}