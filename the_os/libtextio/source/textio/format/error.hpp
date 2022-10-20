#pragma  once

#include <string_view>
#include <stdexcept>

#ifndef _MSC_VER
  #include <bits/functexcept.h>
#endif


namespace textio::fmt::detail
{
  [[noreturn]] 
  static inline auto throw_conversion_error(std::string_view error) -> void
  {
  #ifdef _MSC_VER
    throw std::runtime_error(std::string(error));
  #else
    std::__throw_runtime_error(error.data());
  #endif
  }
}