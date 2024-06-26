#pragma  once

#include <string_view>
#include <stdexcept>

#ifndef _MSC_VER
  #include <bits/functexcept.h>
#endif


namespace textio::detail
{
  [[noreturn]] 
  static inline auto throw_conversion_error(std::string_view error) -> void
  {
  #ifdef __cpp_exceptions
    throw std::runtime_error(std::string(error));
  #else
    std::__throw_runtime_error(error.data());
  #endif
  }

  [[noreturn]] 
  static inline auto throw_invalid_argument(std::string_view error) -> void
  {
  #ifdef __cpp_exceptions
    throw std::invalid_argument(std::string(error));
  #else
    std::__throw_invalid_argument(error.data());
  #endif
  }

}