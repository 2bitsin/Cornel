#include <memory/block_list.hpp>

#include <iostream>
#include <ranges>
#include <algorithm>

auto pretty_print(block_list const& blist, std::ostream& oss) noexcept -> void
{
  auto status = [](block_list::block_type const& head) -> std::string_view    
  {
    switch (block_list::block_status(head))
    {
		case block_list::block_allocated: 
      return "allocated"; 
    case block_list::block_available: 
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

  snprintf(buffer, sizeof(buffer)-1, "head = %08x, tail = %08x", 0u, byte_diff(blist.m_head, blist.m_tail));
  oss << buffer << "\n";

  std::uintptr_t total_size{ 0 }, allocated{ 0 }, available{ 0 }; 
  for (auto head = blist.m_head; head; head = head->next)
  {

    snprintf(buffer, sizeof(buffer)-1, "%08x :", byte_diff(head, blist.m_head));
    oss << buffer << " ";

    snprintf(buffer, sizeof(buffer)-1, head->next ? "next=%08x" : "next=( null )", head->next ? byte_diff(head->next, blist.m_head) : 0);
    oss << buffer << " ";
    
    snprintf(buffer, sizeof(buffer)-1, head->prev ? "prev=%08x" : "prev=( null )", head->prev ? byte_diff(head->prev, blist.m_head) : 0);
    oss << buffer << " ";

    snprintf(buffer, sizeof(buffer)-1, "status=%s size=%08x", status(*head).data(), head->size);
    oss << buffer << "\n";

    
    total_size += head->size;   
    if (block_list::is_block_available(*head))
      available += head->size;
    if (block_list::is_block_allocated(*head))
      allocated += head->size;
  }
  snprintf(buffer, sizeof(buffer)-1, "total_size=%08x, allocated=%08x, available=%08x", total_size, allocated, available);
  oss << buffer << "\n";    
}
