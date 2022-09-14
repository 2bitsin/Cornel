#include <functional>
#include <memory>
#include <ranges>
#include <algorithm>
#include <iostream>

#include <memory/block_list.hpp>

static const constexpr std::hash<void const*> ptr_hash{ };

auto allocated(void const* ptr) -> std::size_t
{
  return ~ptr_hash(ptr);
}

auto available(void const* ptr) -> std::size_t
{
  return ptr_hash(ptr);
}

block_list::block_list()
: m_head{ nullptr },
  m_tail{ nullptr }
{}

block_list::~block_list()
{}

auto block_list::block_from_pointer(void* pointer) -> block_type*
{
  return (((block_type*)pointer) - 1u);
}

auto block_list::block_from_pointer(void const* pointer) -> block_type const*
{
  return (((block_type const*)pointer) - 1u);
}

auto block_list::pointer_from_block(block_type* block) -> void*
{
  return block + 1u;
}

auto block_list::pointer_from_block(block_type const* block) -> void const*
{
  return block + 1u;
}

auto block_list::set_block_allocated(block_type& block) -> void
{
  block.meta = allocated(&block);
}

auto block_list::set_block_available(block_type& block) -> void
{
  block.meta = available(&block);
}

auto block_list::is_block_allocated(block_type const& block) const -> bool
{
  return block.meta == allocated(&block);
}

auto block_list::is_block_available(block_type const& block) const -> bool
{
  return block.meta == available(&block);
}

auto block_list::block_status(block_type& block) const -> block_status_type
{
  if(is_block_allocated(block))
    return block_allocated;
  if(is_block_available(block))
    return block_available;
  return block_invalid;
}

auto block_list::block_status(block_type const& block) const -> block_status_type
{
  if(is_block_allocated(block))
    return block_allocated;
  if(is_block_available(block))
    return block_available;
  return block_invalid;
}

auto block_list::initialize(range_type init) noexcept -> bool
{
  if (init.size() < sizeof(block_type) * 2u) 
    return false;

  auto block = (block_type*)init.data();
  
  block->size = init.size();
  block->next = nullptr;
  block->prev = nullptr;
  
  set_block_available(*block);
  
  m_head = block;
  m_tail = block;

  return true;
}

auto block_list::try_split_block(block_type* head, std::size_t size) -> bool
{
  if((head->size - size) < sizeof(block_type) * 2u)
    return false;
  
  auto next = (block_type*)((std::byte*)head + size);
  
  next->size = head->size - size; 
  next->prev = head;
  
  if (is_block_allocated(*head)) 
    set_block_allocated(*next);
  else 
    set_block_available(*next);
    
  head->next = next;
  head->size = size;

  if (head == m_tail)
    m_tail = next;
  
  return true;
}

auto block_list::try_mege_blocks(block_type* lower, block_type* upper) -> block_type*
{
  if (lower == upper)
    return nullptr;
  if (lower > upper)
    std::swap(upper, lower);
  if (block_status(*lower) != block_status(*upper))
    return nullptr; 
  lower->size += upper->size;
  lower->next  = upper->next;
  if (nullptr != upper->next) 
    upper->next->prev = lower;  
  if (m_tail == upper)
    m_tail = lower;
  return lower;
}

auto block_list::allocate(std::size_t size) noexcept -> void*
{ 
  static constexpr const auto Q = sizeof(block_type);
  size = ((size + Q - 1) / Q) * Q;
  for (auto head = m_head; nullptr != head; head = head->next) 
  {   
    if (is_block_allocated(*head))
      continue;
    if (head->size - sizeof(block_type) < size)
      continue;
    try_split_block(head, size);
    set_block_allocated(*head);   
    return pointer_from_block(head);
  }
  return nullptr;
}

auto block_list::deallocate(void* what) noexcept -> bool
{
  auto curr = block_from_pointer(what);
  if (!is_block_allocated(*curr))
    return false;
  set_block_available(*curr);
  while(curr->next && try_mege_blocks(curr, curr->next));
  while((curr = curr->prev) && try_mege_blocks(curr, curr->next));  
  return true;
}

auto block_list::pretty_print(std::ostream& oss) const noexcept -> void
{
  auto status = [this](block_type const& head) -> std::string_view    
  {
    switch (block_status(head))
    {
    case block_allocated: 
      return "allocated"; 
    case block_available: 
      return "available"; 
    default: 
      return "invalid"; 
    }
  };

  auto byte_diff = [](auto* lhs, auto* rhs)
  {
    if (lhs > rhs) std::swap(lhs, rhs);
    return (std::byte const*)rhs - (std::byte const*)lhs;
  };

  char buffer[2048];
  std::ranges::fill(buffer, 0);

  snprintf(buffer, sizeof(buffer)-1, "head = %08x, tail = %08x", 0u, byte_diff(m_head, m_tail));
  oss << buffer << "\n";

  std::uintptr_t total_size{ 0 }, allocated{ 0 }, available{ 0 }; 
  for (auto head = m_head; head; head = head->next)
  {

    snprintf(buffer, sizeof(buffer)-1, "%08x :", byte_diff(head, m_head));
    oss << buffer << " ";

    snprintf(buffer, sizeof(buffer)-1, head->next ? "next=%08x" : "next=( null )", head->next ? byte_diff(head->next, m_head) : 0);
    oss << buffer << " ";
    
    snprintf(buffer, sizeof(buffer)-1, head->prev ? "prev=%08x" : "prev=( null )", head->prev ? byte_diff(head->prev, m_head) : 0);
    oss << buffer << " ";

    snprintf(buffer, sizeof(buffer)-1, "status=%s size=%08x", status(*head).data(), head->size);
    oss << buffer << "\n";

    
    total_size += head->size;   
    if (is_block_available(*head))
      available += head->size;
    if (is_block_allocated(*head))
      allocated += head->size;
  }
  snprintf(buffer, sizeof(buffer)-1, "total_size=%08x, allocated=%08x, available=%08x", total_size, allocated, available);
  oss << buffer << "\n";
  
  
}

auto block_list::deallocate(void* what,[[maybe_unused]] std::size_t size) noexcept -> bool
{
  return deallocate(what);
}
