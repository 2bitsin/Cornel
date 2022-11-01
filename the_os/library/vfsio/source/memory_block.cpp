#include <vfsio/memory_block.hpp>

using vfsio::memory_block;


auto memory_block::read(std::span<std::byte> buffer_v, std::uintmax_t offset_v) -> std::size_t
{
	if (offset_v > m_block_bytes.size())
	{
		set_error(error::out_of_bounds);
		return 0u;
	}
	
	const auto bytes_left = m_block_bytes.size() - offset_v;
	const auto bytes_to_read = std::min(buffer_v.size(), bytes_left);
	const auto start_from = m_block_bytes.begin() + offset_v;	
	std::copy(start_from, start_from + bytes_to_read, buffer_v.begin());
	return bytes_to_read;	
}

auto memory_block::write(std::span<const std::byte> buffer_v, std::uintmax_t offset_v) -> std::size_t
{
	if (m_read_only)
	{
		set_error(error::access_denied);
		return 0u;
	}

	if (offset_v > m_block_bytes.size())
	{
		set_error(error::out_of_bounds);
		return 0u;
	}

	const auto bytes_left = m_block_bytes.size() - offset_v;
	const auto bytes_to_write = std::min(buffer_v.size(), bytes_left);
	const auto start_from = m_block_bytes.begin() + offset_v;
	std::copy(buffer_v.begin(), buffer_v.begin() + bytes_to_write, start_from);
	return bytes_to_write;
}


memory_block::memory_block(std::span<std::byte> block_bytes_v, bool read_only_v)
: m_block_bytes(block_bytes_v),
	m_read_only(read_only_v)
{}
