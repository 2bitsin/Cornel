#include <functional>
#include <memory>
#include <ranges>
#include <algorithm>

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

auto block_list::is_block_allocated(block_type const& block) -> bool
{
  return block.meta == allocated(&block);
}

auto block_list::is_block_available(block_type const& block) -> bool
{
  return block.meta == available(&block);
}

auto block_list::block_status(block_type& block) -> block_status_type
{
  if(is_block_allocated(block))
    return block_allocated;
  if(is_block_available(block))
    return block_available;
  return block_invalid;
}

auto block_list::block_status(block_type const& block) -> block_status_type
{
  if(is_block_allocated(block))
    return block_allocated;
  if(is_block_available(block))
    return block_available;
  return block_invalid;
}

auto block_list::initialize(range_type init) noexcept -> bool
{
  using std::byte;
  using std::uintptr_t;

  auto size = init.size();
  auto addr = init.data();

  static constexpr auto Q = sizeof(block_type);

  if ((uintptr_t)addr % Q) 
  {
    addr = (byte*)(((uintptr_t)addr + Q) & ~(Q - 1));
    size -= (uintptr_t)addr - (uintptr_t)init.data();
  }

  if (init.size() < sizeof(block_type) * 2u) 
    return false;

  auto block = (block_type*)addr;  
  block->size = size;
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
  size = ((size + Q - 1) / Q) * Q + Q;
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

auto block_list::size(void const* what) const noexcept -> std::size_t
{
  auto curr = block_list::block_from_pointer(what);

  if (!block_list::is_block_allocated(*curr))
    return 0u;

  if ((curr->size <= sizeof(block_type)))
    return 0u;

  return curr->size - sizeof(block_type);
}

auto block_list::deallocate(void* what,[[maybe_unused]] std::size_t size) noexcept -> bool
{
  return deallocate(what);
}

auto block_list::reallocate([[maybe_unused]] void* what, [[maybe_unused]] std::size_t size) noexcept -> void*
{
  // TODO : implement and test
  /*
  if (nullptr == what)
    return allocate(size);
  auto curr = block_from_pointer(what);
  if (!is_block_allocated(*curr))
    return nullptr;
  if (curr->size >= size)
    return what;
  auto next = curr->next;
  if (next && is_block_available(*next) && (curr->size + next->size) >= size)
  {
    try_mege_blocks(curr, next);
    try_split_block(curr, size);
    return what;
  }
  auto new_what = allocate(size);
  if (nullptr == new_what)
    return nullptr;
  std::ranges::copy_n((std::byte*)what, curr->size, (std::byte*)new_what);
  deallocate(what);
  return new_what;
  */
  return nullptr;
}
