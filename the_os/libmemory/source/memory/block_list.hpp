#pragma once

#include <cstddef>
#include <cstdint>
#include <span>
#include <textio/simple.hpp>

struct block_list
{
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

  block_list();
  
  auto initialize(range_type) noexcept -> bool;
  auto allocate(std::size_t) noexcept -> void*;

  auto deallocate(void*, std::size_t) noexcept -> bool;
  auto deallocate(void*) noexcept -> bool;

  auto size(void const* ptr) const noexcept -> std::size_t;

  auto reallocate(void*, std::size_t) noexcept -> void*;

  auto contains(void const* ptr) const noexcept -> bool;
  auto is_valid(void const* ptr) const noexcept -> bool;

  template <std::output_iterator<char> O>
  friend auto pretty_print(block_list const& blist, O oi) noexcept -> void;    

  ~block_list();
  
protected:
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


template <std::output_iterator<char> O>
auto pretty_print(block_list const& blist, O oss) noexcept -> void
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

  using namespace textio::simple;
  using namespace textio::simple::fmt;

  writeln(oss, ">>> head = 00000000, tail = ", hex<'&'>(byte_diff(blist.m_head, blist.m_tail)));


  std::uintptr_t total_size{ 0 }, allocated{ 0 }, available{ 0 }; 
  for (auto head = blist.m_head; head; head = head->next)
  {

    write(oss, hex<'&'>(byte_diff(head, blist.m_head)), ": ");

    if (head->next) write(oss, "next=", hex<'&'>(byte_diff(head->next, blist.m_head)), ", "); else write(oss, "next=( null ), ");
    if (head->prev) write(oss, "prev=", hex<'&'>(byte_diff(head->prev, blist.m_head)), ", "); else write(oss, "prev=( null ), ");

    writeln(oss, "size=", hex<'&'>(head->size), " status=", status(*head));
     
    total_size += head->size;   
    if (block_list::is_block_available(*head))
      available += head->size;
    if (block_list::is_block_allocated(*head))
      allocated += head->size;
  }    
}
