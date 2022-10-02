#pragma once

#include <cstddef>
#include <cstdint>
#include <memory>
#include <span>
#include <concepts>
#include <algorithm>

#include <memory/detail.hpp>

namespace memory
{
  template <typename T, detail::allocator_like A>
  static inline auto allocate_buffer_of(A&& a, std::size_t size) -> std::span<T>
  {
    return std::span<T>{ (T*)a.allocate(size*sizeof(T)), size };
  }   

  template <typename T, detail::allocator_like A>
  static inline auto allocate_buffer_of(A&& a, std::size_t size, T const& value) -> std::span<T>
  {
    auto buffer = allocate_buffer_of<T>(a, size);
    for (auto&& single_element : buffer)
      std::construct_at(&single_element, value);
    return buffer;
  }

  template <typename T, typename F, detail::allocator_like A>
  requires (std::is_invocable_v<F, std::size_t>)
  static inline auto allocate_buffer_of(A&& a, std::size_t size, F&& fill) -> std::span<T>
  {
    auto buffer = allocate_buffer_of<T>(a, size);
    for (std::size_t i = 0u; i < size; ++i)
      std::construct_at(&buffer[i], fill(i));
    return buffer;
  }

  template <typename T, detail::allocator_like A>
  static inline auto deallocate_buffer(A&& a, std::span<T> const& buffer) -> void
  {
    std::destroy(buffer.begin(), buffer.end());
    a.deallocate(buffer.data(), buffer.size()*sizeof(T));
  }

  static constexpr auto reallocate_force_copy_flag = 0x1u;
  static constexpr auto reallocate_dont_release_prev_flag = 0x2u;

  template <typename T, detail::allocator_like A>
  static inline auto reallocate_buffer(A&& a, std::span<T> buffer, std::size_t new_size, T const& defval = T(), std::uint32_t flags = 0u) -> std::span<T>
  {    
    if (new_size <= buffer.size() && !(flags & reallocate_force_copy_flag))
      return buffer.subspan(0, new_size);
    auto new_buffer = allocate_buffer_of<T>(a, new_size, defval);
    const auto min_size = std::min(buffer.size(), new_size);
    std::copy_n(buffer.begin(), min_size, new_buffer.begin());
    if(!(flags & reallocate_dont_release_prev_flag))
      deallocate_buffer(a, buffer);
    return new_buffer;
  }

  struct default_allocator_type
  {
    static inline void* allocate(std::size_t size) noexcept { return std::malloc(size); }
    static inline void deallocate(void* ptr) noexcept { std::free(ptr); }
  };

  static inline auto default_allocator() noexcept -> default_allocator_type { return {}; }
}