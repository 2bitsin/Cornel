#pragma once

#include <cstddef>
#include <cstdint>
#include <span>

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
static inline auto allocate_buffer_of(std::size_t size, F&& fill) -> std::span<T>
{
  auto buffer = allocate_buffer_of<T>(size);
  for (std::size_t i = 0u; i < size; ++i)
    buffer[i] = fill(i);
  return buffer;
}

template <typename T>
static inline auto deallocate_buffer(std::span<T> buffer) -> void
{
  delete[] buffer.data();
}