#pragma once

namespace memory
{
  auto initialize(bool first_time) -> void;
  auto finalize(bool last_time) -> void;
  auto is_valid(void* ptr) -> bool;

#ifdef __cpp_lib_span
  template <typename T>
  static inline auto is_valid(std::span<T> const& ptr) -> bool 
  {
    return is_valid(ptr.data());
  }
#endif
};