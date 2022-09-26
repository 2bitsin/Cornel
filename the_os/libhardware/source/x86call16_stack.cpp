#include <utility>

#include <hardware/x86call16_stack.hpp>
#include <memory/allocate_buffer.hpp>

x86arch::call16_stack::call16_stack(call16_context& context, std::size_t size) noexcept
: m_context(context)
, m_bytes(allocate_buffer_of<std::byte>(memory::default_allocator(), size))
{
  const auto stack_address = real_address::from_pointer(m_bytes.data());
  m_context.ss = stack_address.seg;
  m_context.esp = stack_address.off + size;      
}

x86arch::call16_stack::call16_stack(call16_stack&& other) noexcept
: m_context(other.m_context)
, m_bytes(std::exchange(other.m_bytes, std::span<std::byte>{}))    
{
  if (&other.m_context != &m_context)
  {
    m_context.ss  = std::exchange(other.m_context.ss,   0u);
    m_context.esp = std::exchange(other.m_context.esp,  0u);
  }
}

auto x86arch::call16_stack::operator = (call16_stack&& other) noexcept -> call16_stack&
{
  if (&other.m_context != &m_context)
  {
    m_context.ss  = std::exchange(other.m_context.ss,   0u);
    m_context.esp = std::exchange(other.m_context.esp,  0u);
  }
  m_bytes = std::exchange(other.m_bytes, std::span<std::byte>{});
  return *this;
}
    
x86arch::call16_stack::~call16_stack() noexcept
{
  if (!m_bytes.empty())
  { deallocate_buffer(memory::default_allocator(), m_bytes); }      
}
