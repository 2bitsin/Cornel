#include "vfsio/heapfile.hpp"

#include <algorithm>

using vfsio::heapfile;

auto heapfile::read(error& error_v, std::span<std::byte> buffer_v, std::uintmax_t offset_v) -> std::span<std::byte> 
{
	error_v = error::none;
	if (offset_v > m_data_s.size())
		return buffer_v.subspan(0, 0);
	const auto size_v = std::min<std::size_t>(buffer_v.size(), m_data_s.size() - offset_v);
	std::copy_n(m_data_s.data() + offset_v, size_v, buffer_v.data());
	return buffer_v.subspan(0, size_v);
}

auto heapfile::write(error& error_v, std::span<std::byte const> buffer_v, std::uintmax_t offset_v) -> std::span<std::byte const> 
{
	error_v = error::none;
	if (offset_v + buffer_v.size() > m_data_s.size())
	{
		resize(error_v, offset_v + buffer_v.size());
		if (error_v != error::none) 
		{
			if (offset_v >= m_data_s.size())
				return buffer_v;			
			const auto available_bytes_v = m_data_s.size() - offset_v;
			std::copy_n(buffer_v.data(), available_bytes_v, m_data_s.data() + offset_v);
			return buffer_v.subspan(buffer_v.size() - available_bytes_v);
		}
	}
	std::copy_n(buffer_v.data(), buffer_v.size(), m_data_s.data() + offset_v);
	return buffer_v.subspan(buffer_v.size());
}

auto heapfile::resize(error& error_v, std::uintmax_t new_size_v) -> std::uintmax_t
{
	error_v = error::none;
	new_size_v = std::min<std::uintmax_t>(new_size_v, m_max_size);
	const auto new_data_p = m_heap_w.reallocate(error_v, m_data_s.data(), new_size_v);
	if (nullptr == new_data_p)
		return m_data_s.size();	
	m_data_s = std::span<std::byte>((std::byte*)new_data_p, new_size_v);
	return m_data_s.size();
}

auto vfsio::heapfile::size(error& error_v) const -> std::uintmax_t 
{
	error_v = error::none;
	return m_data_s.size();
}
