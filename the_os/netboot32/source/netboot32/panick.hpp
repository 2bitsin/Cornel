#pragma once

#include <cstdint>
#include <cstddef>

struct block_list;

namespace panick
{
  [[noreturn]] void cant_enable_atwenty() noexcept;
  [[noreturn]] void invalid_free(void* ptr) noexcept;
  [[noreturn]] void out_of_memory(std::size_t size) noexcept;

  [[noreturn]] void invalid_free(void* ptr, block_list const& blist) noexcept;
  [[noreturn]] void out_of_memory(std::size_t size, block_list const& blist) noexcept;

  [[noreturn]] void invalid_pxenvplus() noexcept;
  [[noreturn]] void invalid_bangpxe() noexcept;  

  [[noreturn]] void pxe_failed(const char* what) noexcept;
};