#pragma once

#include <cstddef>
#include <cstdint>
#include <span>
#include <functional>
#include <concepts>

#include <memory/allocate_buffer.hpp>

namespace memory
{
  template <typename T, std::size_t E = std::dynamic_extent>
  struct unique_buffer: std::span<T, E>
  {
    using self = std::span<T, E>;
    
    template <typename Allo>
    requires (detail::allocator_like<Allo>)
    static auto allocate(Allo& alloc, std::size_t N) -> unique_buffer<T, E>
    {
      return unique_buffer<T, E> { 
        allocate_buffer_of<T>(alloc, N), 
        [&alloc] (auto&& what) { 
          deallocate_buffer(alloc, what); 
        } 
      }; 
    }

    ~unique_buffer()
    {
      m_release();
    }

    unique_buffer(unique_buffer const&) = delete;
    unique_buffer& operator=(unique_buffer const&) = delete;
    unique_buffer(unique_buffer&& what)
    : self      { std::move (what) }
    , m_release { std::move (what.m_release) }
    {

    }

  protected:
    template <std::invocable<unique_buffer<T, E>&> Release>
    unique_buffer(Release&& release, std::span<T, E> buffer) noexcept
    : self      { buffer }
    , m_release { std::forward<Release>(release) }
    {}

  private:
    std::function<void(unique_buffer<T, E>&)> m_release;
  }

}