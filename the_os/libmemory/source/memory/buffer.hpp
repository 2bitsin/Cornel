#pragma once

#include <cstddef>
#include <cstdint>
#include <span>
#include <functional>
#include <concepts>

#include <memory/allocate_buffer.hpp>

namespace memory
{
  
  template <typename T, allocator_like Allocator>
  struct buffer
  {
    using allocator_type = Allocator;
    using value_type = T;

    

  private:
    std::span<value_type>   m_buffer_sp;
    allocator_type&         m_allocator;
  }; 

}