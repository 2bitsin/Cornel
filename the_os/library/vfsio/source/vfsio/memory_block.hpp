#pragma once

#include <vfsio/Iblock.hpp>

namespace vfsio
{
	struct memory_block
	: public Iblock
	{
		memory_block(std::span<std::byte> block_bytes_v, bool read_only_v = false)
		: m_block_bytes(block_bytes_v),
			m_read_only(read_only_v)
		{}

		auto read(std::span<std::byte> buffer_v, std::uintmax_t offset_v) -> std::size_t override;
		auto write(std::span<const std::byte> buffer_v, std::uintmax_t offset_v) -> std::size_t override;		

	private:
		std::span<std::byte> m_block_bytes;		
		bool m_read_only = false;
	};
}
