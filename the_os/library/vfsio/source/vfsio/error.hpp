#pragma once

#include <cstddef>
#include <cstdint>
#include <cerrno>
#include <textio/format.hpp>

namespace vfsio
{
  enum class error: int
  {
    success           =  0,
		unknown						= -1,
		other							= -2,
    not_implemented   = -3,
    not_supported     = -4,
    path_not_found    = -5,
    access_denied     = -6,
    invalid_argument  = -7,    
    invalid_mode      = -8,
    out_of_bounds     = -9,
		is_not_open				= -10,
		bad_file_handle		= -11,
		name_too_long			= -12,
		already_exists		= -13,
		too_large					= -14,	
		bad_volume				= -15
		
  }; 

	static inline error from_errno(int errno_v = errno)
	{
		switch (errno_v)
		{
		case 0:            return error::success;
		case ENOENT:       return error::path_not_found;
		case EACCES:       return error::access_denied;
		case EINVAL:       return error::invalid_argument;
		case EBADF:        return error::bad_file_handle;
		case ENAMETOOLONG: return error::name_too_long;
		case EEXIST:       return error::already_exists;
		case EFBIG:        return error::too_large;
		default:           return error::other;
		}
	}
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
			case vfsio::error::unknown:           return format_to<"{}({})">(out_i, "error::unknown",          (int)value_v);
			case vfsio::error::other:             return format_to<"{}({})">(out_i, "error::other",            (int)value_v);
      case vfsio::error::not_implemented:   return format_to<"{}({})">(out_i, "error::not_implemented",  (int)value_v);
      case vfsio::error::not_supported:     return format_to<"{}({})">(out_i, "error::not_supported",    (int)value_v);
      case vfsio::error::path_not_found:    return format_to<"{}({})">(out_i, "error::path_not_found",   (int)value_v);
      case vfsio::error::access_denied:     return format_to<"{}({})">(out_i, "error::access_denied",    (int)value_v);
      case vfsio::error::invalid_argument:  return format_to<"{}({})">(out_i, "error::invalid_argument", (int)value_v);
      case vfsio::error::invalid_mode:      return format_to<"{}({})">(out_i, "error::invalid_mode",     (int)value_v);
			case vfsio::error::out_of_bounds:     return format_to<"{}({})">(out_i, "error::out_of_bounds",    (int)value_v);
			case vfsio::error::is_not_open:       return format_to<"{}({})">(out_i, "error::is_not_open",      (int)value_v);
			case vfsio::error::bad_volume:        return format_to<"{}({})">(out_i, "error::bad_volume",       (int)value_v);
      default:                              return format_to<"error({})">(out_i, (int)value_v);
      }      
    }

  }; 
}