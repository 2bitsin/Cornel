#pragma once

struct runtime
{

  static auto initialize(bool first_time) -> void;
  static auto finalize(bool last_time) -> void;
  static auto panick() -> void;

  // Allocate below 1 MiB
  static auto allocate(std::size_t size) -> void*;
  static auto deallocate(void* pointer, std::size_t size) -> bool;
};