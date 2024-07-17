
#include <cstring>

#include <memory/block_list.hpp>

using memory::block_list;

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

auto block_list::range_from_block(block_type& block) -> range_type
{
  if (block_status(block) == block_invalid)
    return {};
  return std::span((std::byte*)&block, block.size);
}

auto block_list::block_from_range(range_type range) -> block_type*
{
  range = align_range(range);
  if (range.empty() || range.size () < block_threshold)
    return nullptr;
  return (block_type*)range.data(); 
}

auto block_list::initialize_block(range_type range) -> block_type*
{
  auto block = block_from_range(range);
  if (nullptr == block)
    return nullptr;
  set_block_available(*block);
  block->size = range.size();
  block->next = nullptr;
  block->prev = nullptr;
  return block;
}

auto block_list::align_range(range_type range) -> range_type
{
  const auto address = (std::uintptr_t)range.data();
  if (0u == address % sizeof(block_type))
    return range;   
  const auto offset = sizeof(block_type) - address % sizeof(block_type);
  if (offset >= range.size())
    return {};
  return range.subspan(offset);
}

auto block_list::ranges_overlap(range_type a, range_type b) -> bool
{
  if (a.data () > b.data())
    std::swap(a, b);
  if ((std::size_t)(b.data() - a.data ()) < a.size())
    return true;
  return false;
}

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

auto block_list::insert_range(range_type initialize)  -> bool
{
  initialize = block_list::align_range(initialize);
  
  if (initialize.empty() || initialize.size() < block_threshold)    
    return false;

  if (nullptr == m_head)
  {
    m_head = m_tail = initialize_block(initialize);
    return true;
  }

  auto node = initialize_block(initialize);
  auto insert_here = m_head;
  
  while (node > insert_here && insert_here->next)
    insert_here = insert_here->next;
  
  if (ranges_overlap(range_from_block(*insert_here), range_from_block(*node)))
  {
    return false;
  }
  
  if (node < insert_here)
  {   
    node->next = insert_here;
    node->prev = insert_here->prev;
    insert_here->prev = node;
    if (node->prev)
      node->prev->next = node;
    else if (m_head == insert_here)
      m_head = node;
  }
  else
  {
    node->prev = insert_here;
    node->next = insert_here->next;
    insert_here->next = node;
    if (node->next)
      node->next->prev = node;
    else if (m_tail == insert_here)
      m_tail = node;    
  }
  
  defragmentate(node);  
  return true;
}

auto block_list::try_split_block(block_type* head, std::size_t size) -> bool
{
  if((head->size - size) < block_threshold)
    return false;
  
  auto next = (block_type*)((std::byte*)head + size); 

  next->size = head->size - size;
  head->size = size;
  
  next->next = head->next;
  head->next = next;
  
  next->prev = head;
  if (nullptr != next->next) {
    next->next->prev = next;
  }
  
  set_block_available(*next);
   
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
  if ((std::uintptr_t)lower + lower->size < (std::uintptr_t)upper)
    return nullptr;
  if (!is_block_available(*lower) || !is_block_available(*upper))
    return nullptr;
  lower->size += upper->size;
  lower->next  = upper->next;
  if (nullptr != upper->next) 
    upper->next->prev = lower;  
  if (m_tail == upper)
    m_tail = lower;
  return lower;
}

auto block_list::allocate(std::size_t size)  -> void*
{ 
  static constexpr const auto Q = sizeof(block_type);
  size = ((size + Q - 1) / Q) * Q + Q;
  for (auto head = m_head; nullptr != head; head = head->next) 
  {   
    if (is_block_allocated(*head))
      continue;
    if (head->size < size)
      continue;
    try_split_block(head, size);
    set_block_allocated(*head);   
    return pointer_from_block(head);
  }
  return nullptr;
}

auto block_list::defragmentate(block_type* curr) -> void
{
  while(curr->next && try_mege_blocks(curr, curr->next));
  while((curr = curr->prev) && try_mege_blocks(curr, curr->next));  
}

auto block_list::deallocate(void* what)  -> bool
{
  auto curr = block_from_pointer(what);
  if (!is_block_allocated(*curr))
    return false;
  set_block_available(*curr);
  defragmentate(curr);
  return true;
}

auto block_list::size(void const* what) const  -> std::size_t
{
  auto curr = block_list::block_from_pointer(what);

  if (!block_list::is_block_allocated(*curr))
    return 0u;

  if ((curr->size <= sizeof(block_type)))
    return 0u;

  return curr->size - sizeof(block_type);
}

auto block_list::deallocate(void* what,[[maybe_unused]] std::size_t size)  -> bool
{
  return deallocate(what);
}

auto block_list::contains(void const* ptr) const  -> bool
{
  auto const b_ptr = (std::byte const*)ptr;
  return m_bits.data() <= b_ptr && b_ptr < (m_bits.data() + m_bits.size());
}

auto block_list::is_valid(void const* ptr) const  -> bool
{
  auto const b_ptr = block_list::block_from_pointer(ptr);    
  if (nullptr == b_ptr || !block_list::contains(b_ptr))
    return false;
  return block_invalid != block_list::block_status(*b_ptr);
}

auto block_list::probe_available(block_type const* node) -> std::size_t
{
  std::size_t available{ 0u };  
  while (node && is_block_available(*node)) 
  {
    available += node->size;
    node = node->next;
  } 
  return available;
}

auto block_list::reallocate(void* what, std::size_t a_size)  -> void*
{
  static constexpr const auto Q = sizeof(block_type);
  auto size = ((a_size + Q - 1) / Q) * Q + Q;
  // If the pointer is null, then this is equivalent to a call to allocate(size).
  if (nullptr == what)
    return allocate(a_size);  
  // Get the block from pointer
  auto curr = block_list::block_from_pointer(what);
  // If it's an invalid block, we can't reallocate it
  if (!block_list::is_block_allocated(*curr))
    return nullptr; 
  // We need to adjust size for simplicity, see allocate
  
  // If requested size is same as current size, do nothing
  if (size == curr->size)
    return what;
  // If requested less then current size, just split the block
  if (size < curr->size) {
    // If we can split the block, do it
    try_split_block(curr, size);
    return what;
  }       
  // Check if we have enough headroom 
  const auto size_ahead = probe_available(curr->next);
  if (curr->size + size_ahead >= size)
  {
    // Try deallocate the node, and that should merge nodes ahead
    // while not stomping on this node
    deallocate(what);
    // Let's double check our assumption    
    // If we have enough room
    if (size <= curr->size) {
      // Allocate the same block again
      set_block_allocated(*curr);
      // Try spliting off the remainder
      try_split_block(curr, size);
      // Return the pointer
      return what;
    }
  }
  // If all else fails, allocate a new block
  auto new_block = allocate(a_size);  
  // Did we get a new block ?
  if (nullptr == new_block)
    // No
    return nullptr;
  // Yes, Copy the data
  std::memcpy(new_block, what, curr->size);
  // Deallocate the old block
  deallocate(what);
  // Return the new block
  return new_block;
}
