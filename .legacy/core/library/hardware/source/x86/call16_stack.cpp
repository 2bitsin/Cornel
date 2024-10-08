#include <utility>

#include <hardware/x86/call16_stack.hpp>


x86arch::call16_stack::call16_stack(call16_context& context, std::size_t size) 
: call16_stack(*std::pmr::new_delete_resource(), context, size)
{}

x86arch::call16_stack::call16_stack(std::pmr::memory_resource& allocator, call16_context& context, std::size_t size) 
: m_context { context }
, m_buffer  { allocator, size }
{
  const auto stack_address = address16::from(m_buffer);
  m_context.ss = stack_address.seg;
  m_context.esp = stack_address.off + size;      
}

x86arch::call16_stack::call16_stack(call16_stack&& other) 
: m_context { other.m_context }
, m_buffer  { std::exchange(other.m_buffer, memory::buffer<std::byte>{}) }
{
  if (&other.m_context != &m_context)
  {
    m_context.ss  = std::exchange(other.m_context.ss,   0u);
    m_context.esp = std::exchange(other.m_context.esp,  0u);
  }
}

auto x86arch::call16_stack::operator = (call16_stack&& other)  -> call16_stack&
{
  call16_stack tmp { std::move(other) };
  std::swap(*this, tmp);
  return *this;
}
    
x86arch::call16_stack::~call16_stack() 
{}
