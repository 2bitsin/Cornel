#pragma once

namespace memory
{
  auto initialize(bool first_time) -> void;
  auto finalize(bool last_time) -> void;
  
  auto ext_allocate(std::size_t size) -> void*;
  auto ext_deallocate(void* pointer) -> void;

  auto allocate(std::size_t size) -> void*;
  auto deallocate(void* pointer) -> void;


  struct ext_allocator_type
  {
    static inline void* allocate(std::size_t size) noexcept { return ext_allocate(size); }
    static inline void deallocate(void* ptr) noexcept { ext_deallocate(ptr); }
  };

  static inline auto ext_allocator() noexcept -> ext_allocator_type { return {}; }

};