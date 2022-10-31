#pragma once

#include <cstdint>
#include <cstddef>

struct block_list;

namespace panick
{
  [[noreturn]] void cant_enable_atwenty() ;
  [[noreturn]] void invalid_free(void* ptr) ;
  [[noreturn]] void out_of_memory(std::size_t size) ;

  [[noreturn]] void invalid_free(void* ptr, block_list const& blist) ;
  [[noreturn]] void out_of_memory(std::size_t size, block_list const& blist) ;
  [[noreturn]] void invalid_free_ext(void* ptr, block_list const& blist) ;
  [[noreturn]] void out_of_memory_ext(std::size_t size, block_list const& blist) ;

  [[noreturn]] void pxenv_error(const char* what) ;
  [[noreturn]] void unable_to_download(const char* what) ;
};