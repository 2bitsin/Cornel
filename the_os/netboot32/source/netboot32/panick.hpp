#pragma once

#include <cstdint>
#include <cstddef>

struct panick
{
  [[noreturn]] static void invalid_free(void* ptr) noexcept;
  [[noreturn]] static void out_of_memory(std::size_t size) noexcept;

  [[noreturn]] static void invalid_free(void* ptr, struct block_list const& blist) noexcept;
  [[noreturn]] static void out_of_memory(std::size_t size, struct block_list const& blist) noexcept;
};