#pragma once

#include <cstdint>
#include <cstddef>
#include <type_traits>
#include <cstring>
#include <memory_resource>

#include <hardware/x86/call16.hpp>
#include <hardware/x86/address16.hpp>

#include <memory/buffer.hpp>  

namespace x86arch
{
  struct call16_stack
  {
    call16_stack (std::pmr::memory_resource& allocator, call16_context& context, std::size_t size) ;
    call16_stack (call16_context& context, std::size_t size) ;
    call16_stack (call16_stack&& other) ;
    ~call16_stack() ;
    auto operator = (call16_stack&& other)  -> call16_stack&;

    call16_stack(call16_stack const&) = delete;
    auto operator=(call16_stack const&) -> call16_stack& = delete;

    template <typename T>
    requires (std::is_trivially_copyable_v<T>)
    inline bool push(T const& value) 
    {
      if (m_context.esp < sizeof(T))
      { return false; }
      m_context.esp -= sizeof(T);
      std::memcpy(m_buffer.data() + m_context.esp, &value, sizeof(T));
      return true; 
    }

    template <typename... T>
    requires (sizeof...(T) > 1)
    inline bool push(T const&... args) 
    {
      return (push(args) && ...);
    }

    auto&& bytes() const 
    { return m_buffer; }
    auto&& bytes() 
    { return m_buffer; }

  private:
    call16_context& m_context;
    memory::buffer<std::byte> m_buffer;
  };
}