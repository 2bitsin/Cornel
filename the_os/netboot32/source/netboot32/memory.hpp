#pragma once

namespace memory
{
  auto initialize(bool first_time) -> void;
  auto finalize(bool last_time) -> void;
  
  auto ext_allocate(std::size_t size) -> void*;
  auto ext_deallocate(void* pointer) -> void;

  auto allocate(std::size_t size) -> void*;
  auto deallocate(void* pointer) -> void;
};