#pragma once

#include <memory/block_list.hpp>
#include <vfsio/vfsio.hpp>

#include <cstdint>
#include <cstddef>
#include <span>
#include <tuple>


namespace vfsio
{
	struct peloader
	{
		using entrypoint_t = void();

		peloader(memory::block_list& heap_v): m_heap {heap_v} {};

		auto load(error& error_v, std::span<std::byte const> raw_bytes_v) -> std::tuple<void*, entrypoint_t*>;
	
	private:
		memory::block_list& m_heap;
	};
}