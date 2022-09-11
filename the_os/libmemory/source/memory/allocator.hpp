#pragma once

#include <cstddef>
#include <cstdint>
#include <span>
#include <system_error>
#include <initializer_list>
#include <variant>

//#include <type_traits>

struct allocator
{
  using range_type = std::span<std::byte>;

#pragma pack(push, 1)
  struct block_type 
  {
    std::uintptr_t  size;
    std::uintptr_t  meta;
		static_assert(sizeof(meta) >= sizeof(std::uint32_t));
    block_type*     next;
    block_type*     prev;
  };
#pragma pack(pop)

  allocator();
	
	auto initialize(range_type) noexcept -> std::errc;

	auto allocate(std::size_t) noexcept -> std::variant<void*, std::errc> ;
	auto deallocate(void*, std::size_t) noexcept -> std::errc;
	auto deallocate(void*) noexcept -> std::errc;

  ~allocator();

private:
  block_type*     m_head;
  block_type*     m_tail;
//std::uintptr_t  m_size;
//std::uintptr_t  m_used;
};