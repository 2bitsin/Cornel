#include <memory/allocator.hpp>
#include <functional>

static const constexpr std::hash<void*> ptr_hash{ };

auto occupied(void* ptr) -> std::size_t
{
	return ~ptr_hash(ptr);
}

auto unoccupied(void* ptr) -> std::size_t
{
	return ptr_hash(ptr);
}




allocator::allocator()
: m_head{ nullptr },
	m_tail{ nullptr }
{}

allocator::~allocator()
{}

auto allocator::initialize(range_type init) noexcept -> std::errc
{
	

	if (init.size() < sizeof(block_type) * 2u) 
	{
		return std::errc::not_enough_memory;	
	}

	auto& curr_block = *(block_type*)init.data();
	
	curr_block.size = init.size() - sizeof(block_type);
	curr_block.next = nullptr;
	curr_block.prev = nullptr;
	curr_block.meta = unoccupied(&curr_block);
	
	m_head = &curr_block;
	m_tail = &curr_block;

	return std::errc();
}

auto allocator::allocate(std::size_t size) noexcept -> std::variant<void*, std::errc>
{
	block_type* head{ nullptr };
	block_type* next{ nullptr };

	size = (size + 15) & ~0xfu;
	
	for (head = m_head; head; head = head->next) 
	{
		if (head->meta || head->size < size)
			continue;	

		if ((head->size - size) >= sizeof(block_type) * 2u) 
		{
			next = (block_type*)((std::byte*)(head + 1u) + size);
						
			next->meta = occupied(next);
			next->size = head->size - (size + sizeof(block_type));
			
			next->prev = head;
			head->next = next;

			head->size = head->size - next->size;
		}
		
		head->meta = unoccupied(head);
		
		return (void*)(head + 1u);
	}

	return std::errc::not_enough_memory;	
}

auto allocator::deallocate(void* what) noexcept -> std::errc
{
	block_type* curr{ nullptr };	
	curr = ((block_type*)what) - 1u;

	return std::errc();
}

auto allocator::deallocate(void* what, std::size_t size) noexcept -> std::errc
{
	return deallocate(what);
}
