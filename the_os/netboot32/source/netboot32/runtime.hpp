#pragma once

struct runtime
{

  static auto initialize() -> void;
  static auto finalize() -> void;
  static auto panick() -> void;

  static auto allocate_lower(std::size_t size) -> void*;
  static auto allocate_upper(std::size_t size) -> void*;
  static auto deallocate_lower(void* pointer, std::size_t size) -> bool;
  static auto deallocate_upper(void* pointer, std::size_t size) -> bool;
};