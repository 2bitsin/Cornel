#pragma once

#include <cstddef>
#include <cstdint>
#include <span>
#include <textio/format.hpp>

struct block_list
{
#ifdef _TESTING_
  friend int main(int, char**);
#endif

  using range_type = std::span<std::byte>;

  enum block_status_type
  {
    block_invalid,
    block_available,
    block_allocated
  };
    
#pragma pack(push, 1)
  struct block_type 
  {
    std::uintptr_t  size;
    std::uintptr_t  meta;
    block_type*     next;
    block_type*     prev;
    
    static_assert(sizeof(meta) >= sizeof(std::uint32_t));
  };
  static_assert(sizeof(block_type) == sizeof(void*)*4u);
#pragma pack(pop)
  
  static inline constexpr const auto block_threshold = sizeof(block_type) * 2u;

  block_list();
  
  auto insert_range(range_type) noexcept -> bool;
  
  auto allocate(std::size_t) noexcept -> void*;

  auto deallocate(void*, std::size_t) noexcept -> bool;
  auto deallocate(void*) noexcept -> bool;

  auto size(void const* ptr) const noexcept -> std::size_t;

  auto reallocate(void*, std::size_t) noexcept -> void*;

  auto contains(void const* ptr) const noexcept -> bool;
  auto is_valid(void const* ptr) const noexcept -> bool;

  template <typename Anything>   
  friend auto pretty_print(block_list const& blist, Anything&& oi) noexcept -> void;

  template <std::output_iterator<char> OIterator>
  auto format (OIterator out_i) const noexcept -> OIterator
  {
    pretty_print(*this, out_i);
    return out_i;
  }

  ~block_list();

protected:
  auto defragmentate(block_type* node) -> void;
  auto probe_available(block_type const* node) -> std::size_t;
  static auto range_from_block(block_type& block) -> range_type;
  static auto block_from_range(range_type range) -> block_type*;
  static auto initialize_block(range_type range) -> block_type*;
  static auto align_range(range_type) -> range_type;
  static auto ranges_overlap(range_type a, range_type b) -> bool;

  static auto block_from_pointer(void* pointer) -> block_type*;
  static auto block_from_pointer(void const* pointer) -> block_type const*;
  static auto pointer_from_block(block_type* block) -> void*;
  static auto pointer_from_block(block_type const* block) -> void const*;
  
  static auto set_block_allocated(block_type& block) -> void;
  static auto set_block_available(block_type& block) -> void;
  static auto is_block_allocated(block_type const& block) -> bool;
  static auto is_block_available(block_type const& block) -> bool; 
  static auto block_status(block_type& block) -> block_status_type;
  static auto block_status(block_type const& block) -> block_status_type;
  
  auto try_split_block(block_type* head, std::size_t size) -> bool;
  auto try_mege_blocks(block_type* lower, block_type* upper) -> block_type*;
  
private:
  
  block_type*     m_head;
  block_type*     m_tail;
  range_type      m_bits;
};


template <typename Anything>
auto pretty_print(block_list const& blist, Anything&& oss) noexcept -> void
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

  using namespace textio::fmt;

  format_to<"{}head={:08x}, tail={:08x}\n">(oss, ">>> ", 0, byte_diff(blist.m_head, blist.m_tail));

  std::uintptr_t total_size{ 0 }, allocated{ 0 }, available{ 0 }; 
  for (auto head = blist.m_head; head; head = head->next)
  {

    format_to<"{:#08x}: ">(oss, byte_diff(head, blist.m_head));

    if (head->next) format_to<"next={:08x}, ">(oss, byte_diff(head->next, blist.m_head)); else format_to<"next={:*^8s}, ">(oss, "null") ;
    if (head->prev) format_to<"prev={:08x}, ">(oss, byte_diff(head->prev, blist.m_head)); else format_to<"prev={:*^8s}, ">(oss, "null") ;

    format_to<"size={:08x} status={:<10s}\n">(oss, head->size, status(*head));
     
    total_size += head->size;   
    if (block_list::is_block_available(*head))
      available += head->size;
    if (block_list::is_block_allocated(*head))
      allocated += head->size;
  }    
}
