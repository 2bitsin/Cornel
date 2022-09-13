#pragma once

#include <cstddef>
#include <cstdint>
#include <span>
#include <system_error>
#include <initializer_list>
#include <variant>

//#include <type_traits>

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
	
	auto initialize(range_type) noexcept -> std::errc;

	auto allocate(std::size_t) noexcept -> std::variant<void*, std::errc> ;
	auto deallocate(void*, std::size_t) noexcept -> std::errc;
	auto deallocate(void*) noexcept -> std::errc;

  ~block_list();
	
protected:
	auto block_from_pointer(void* pointer) -> block_type*;
	auto block_from_pointer(void const* pointer) -> block_type const*;
	auto pointer_from_block(block_type* block) -> void*;
	auto pointer_from_block(block_type const* block) -> void const*;
	
	auto set_block_allocated(block_type& block) -> void;
	auto set_block_available(block_type& block) -> void;
	auto is_block_allocated(block_type const& block) -> bool;
	auto is_block_available(block_type const& block) -> bool;	
	auto block_status(block_type& block) -> block_status_type;
	auto block_status(block_type const& block) -> block_status_type;
	
  auto try_split_block(block_type* head, std::size_t size) -> bool;
	auto try_mege_blocks(block_type* lower, block_type* upper) -> block_type*;
	
private:
  block_type*     m_head;
  block_type*     m_tail;
};