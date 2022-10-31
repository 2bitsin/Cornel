#pragma once

#include <memory_resource>

namespace memory
{
  auto initialize(bool first_time) -> void;
  auto finalize(bool last_time) -> void;

  auto get_base_heap()  -> std::pmr::memory_resource&;
  auto get_extended_heap()  -> std::pmr::memory_resource&;

  template <typename T>
  static inline auto get_base_allocator()  
    -> std::pmr::polymorphic_allocator<T>
  { return { &get_base_heap() }; }

  template <typename T>
  static inline auto get_extended_allocator()  
    -> std::pmr::polymorphic_allocator<T>
  { return { &get_extended_heap() }; }
   
};