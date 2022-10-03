#pragma once

#include <cstddef>
#include <cstdint>
#include <span>
#include <concepts>
#include <memory_resource>
#include <algorithm>

#include <memory/allocate_buffer.hpp>

namespace memory
{
  
  template <typename T, allocator_like Allocator = std::pmr::memory_resource>
  struct buffer
  {
    using allocator_type = Allocator;
    using value_type = T;

    buffer(std::size_t size, allocator_type& allocator)
    : m_allocator { allocator }
    , m_buffer_sp { allocate_buffer_of<T>(m_allocator, size) }
    {}

    buffer(const buffer& prev)
    : buffer(prev.size(), prev.m_allocator)
    {
      std::copy(prev.begin(), prev.end(), begin());
    }

    buffer(buffer&& prev) noexcept
    : m_allocator { prev.m_allocator }
    , m_buffer_sp { std::exchance (prev.m_buffer_sp, std::span<T>{ }) }
    {}

    auto operator = (buffer&& prev) -> buffer& 
    {
      if (this != &prev)
      {
        buffer tmp { std::move (prev) };
        std::swap(*this, tmp);
      }
      return *this;
    }

    auto operator = (const buffer& prev) -> buffer& 
    {
      std::destroy_at(this);
      std::construct_at(this, prev);
      return *this;
    }

    auto size () const noexcept -> std::size_t { return m_buffer_sp.size(); }
    
    auto data () const noexcept -> const T* { return m_buffer_sp.data(); }
    auto begin() const noexcept -> const T* { return m_buffer_sp.begin(); }
    auto end  () const noexcept -> const T* { return m_buffer_sp.end(); }

    auto data () noexcept -> T* { return m_buffer_sp.data(); }
    auto begin() noexcept -> T* { return m_buffer_sp.begin(); }
    auto end  () noexcept -> T* { return m_buffer_sp.end(); }

    operator std::span<const value_type> () const noexcept
    {
      return m_buffer_sp;
    }

    operator std::span<value_type> () noexcept
    {
      return m_buffer_sp;
    }

  private:
    std::span<value_type>   m_buffer_sp;
    allocator_type&         m_allocator;
  }; 

}