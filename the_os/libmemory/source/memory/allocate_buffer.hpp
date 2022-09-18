#pragma once

#include <cstddef>
#include <cstdint>
#include <span>
#include <algorithm>

template <typename T>
static inline auto allocate_buffer_of(std::size_t size) -> std::span<T>
{
  return std::span<T>{ new T[size], size };
}

template <typename T>
static inline auto allocate_buffer_of(std::size_t size, T const& value) -> std::span<T>
{
  auto buffer = allocate_buffer_of<T>(size);
  for (auto&& single_element : buffer)
    single_element = value;
  return buffer;
}

template <typename T, typename F>
requires (std::is_invocable_v<F, std::size_t>)
static inline auto allocate_buffer_of(std::size_t size, F&& fill) -> std::span<T>
{
  auto buffer = allocate_buffer_of<T>(size);
  for (std::size_t i = 0u; i < size; ++i)
    buffer[i] = fill(i);
  return buffer;
}

template <typename T>
static inline auto deallocate_buffer(std::span<T> const& buffer) -> void
{
  delete[] buffer.data();
}

static constexpr auto reallocate_force_copy_flag = 0x1u;
static constexpr auto reallocate_dont_release_prev_flag = 0x2u;

template <typename T>
static inline auto reallocate_buffer(std::span<T> buffer, std::size_t new_size, T const& defval = T(), std::uint32_t flags = 0u) -> std::span<T>
{
  if (new_size <= buffer.size() && !(flags & reallocate_force_copy_flag))
    return buffer.subspan(new_size);
  auto new_buffer = allocate_buffer_of<T>(new_size, defval);
  const auto min_size = std::min(buffer.size(), new_size);
  std::copy_n(buffer.begin(), min_size, new_buffer.begin());
  if(!(flags & reallocate_dont_release_prev_flag))
    deallocate_buffer(buffer);
  return new_buffer;
}