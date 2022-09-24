#pragma once

#include <cstdint>
#include <cstddef>
#include <type_traits>
#include <cstring>

#include <hardware/x86call16.hpp>
#include <hardware/x86real_addr.hpp>
#include <memory/allocate_buffer.hpp>
  
namespace x86arch
{
  struct call16_stack
  {
    call16_stack (call16_context& context, std::size_t size) noexcept;
    call16_stack (call16_stack&& other) noexcept;
    ~call16_stack() noexcept;
    auto operator = (call16_stack&& other) noexcept -> call16_stack&;

    call16_stack(call16_stack const&) = delete;
    auto operator=(call16_stack const&) -> call16_stack& = delete;

    template <typename T>
    requires (std::is_trivially_copyable_v<T>)
    inline bool push(T const& value) noexcept
    {
      if (m_context.esp < sizeof(T))
      { return false; }
      m_context.esp -= sizeof(T);
      std::memcpy(m_bytes.data() + m_context.esp, &value, sizeof(T));
      return true;
    }

    template <typename... T>
    requires (sizeof...(T) > 1)
    inline bool push(T const&... args) noexcept
    {
      return (push(args) && ...);
    }

    auto&& bytes() const noexcept
    { return m_bytes; }
    auto&& bytes() noexcept
    { return m_bytes; }

  private:
    call16_context& m_context;
    std::span<std::byte> m_bytes;
  };
}