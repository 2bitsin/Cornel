#include <new>
#include <stdexcept>
#include <algorithm>

#include "packet_buffer.hpp"
#include "common/crc32.hpp"

template <typename T, typename Allocator>
packet_buffer<T, Allocator>::packet_buffer(packet_data databits, deleter_type deleter, allocator_type alloc)
:	m_allocator { std::move(alloc) },
 	m_databits { std::move(databits) },
	m_deleter { std::move(deleter) },
{}

template<typename T, typename Allocator>
packet_buffer<T, Allocator>::packet_buffer(std::size_t size, allocator_type alloc)
: m_allocator { std::move(alloc) },
 	m_databits { m_allocator.allocate(size), size },
	m_deleter { [this, size](auto what){ m_allocator.deallocate(what), size }},
{}

template <typename T, typename Allocator>
packet_buffer<T, Allocator>::packet_buffer(packet_buffer&& other)
:	m_allocator { std::move(other.m_allocator) },
 	m_databits { std::move(other.m_databits) },
	m_deleter { std::move(other.m_deleter) },
{}

template <typename T, typename Allocator>
packet_buffer<T, Allocator>::~packet_buffer()
{
	if (m_deleter)
	{
		m_deleter(m_databits);
	}
}

template <typename T, typename Allocator>
auto packet_buffer<T, Allocator>::operator=(packet_buffer&& other) 
	-> packet_buffer<T, Allocator>&
{
	~packet_data();
	new (this) packet_buffer(std::move(other));
	return *this;
}

template <typename T, typename Allocator>
auto packet_buffer<T, Allocator>::data() const noexcept -> ct_packet_data
{
	return m_databits;
}

template <typename T, typename Allocator>
auto packet_buffer<T, Allocator>::data() noexcept -> packet_data
{
	return m_databits;
}

template <typename T, typename Allocator>
auto packet_buffer<T, Allocator>::size() const noexcept -> std::size_t
{
	return m_databits.size();
}

template<typename T, typename Allocator>
auto packet_buffer<T, Allocator>::deleter() const noexcept -> deleter_type
{
	return m_deleter;
}

template<typename T, typename Allocator>
auto packet_buffer<T, Allocator>::deleter(deleter_type deleter) noexcept -> deleter_type
{
	return std::exchange(m_deleter, std::move(deleter));
}

template<typename T, typename Allocator>
auto packet_buffer<T, Allocator>::crc32() const noexcept -> std::uint32_t
{
  return ::crc32(m_databits, 0u);
}

template<typename T, typename Allocator>
void packet_buffer<T, Allocator>::clone_into(packet_buffer& other)
{
	if (other.size() > size())
	{
		throw std::logic_error("Can't clone into smaller packet then the original packet.");
	}

	std::copy(data(), data() + size(), other.data());
	if (other.size() > size())
		std::fill(other.data() + size(), other.data() + other.size(), 0);
}

template<typename T, typename Allocator>
bool packet_buffer<T, Allocator>::clone_into(packet_buffer& other, std::error_code& ec) noexcept
{	
	if (other.size() > size())
	{
		ec = std::make_error_code(std::errc::not_enough_memory);
		return false;
	}
	std::copy(data(), data() + other.size(), other.data());
	if (other.size() > size())
		std::fill(other.data() + size(), other.data() + other.size(), 0);
	return true;
}

template<typename T, typename Allocator>
auto packet_buffer<T, Allocator>::clone() -> packet_buffer
{
	packet_buffer pbuf{ size() };
	clone_into(pbuf);
	return pbuf;
}


template<typename T, typename Allocator>
auto packet_buffer<T, Allocator>::split(std::size_t chunk_size) const -> std::vector<packet_buffer>
{
	const auto number_of_chunks = ((*this).size() + chunk_size - 1u) / chunk_size;
	
	std::vector<packet_buffer> list;	
	list.reserve(number_of_chunks);
	
	auto tmp = (*this).data();
	while (!tmp.empty())
	{
		list.emplace_back(packet_buffer<T, Allocator>{ tmp.subspan(0, chunk_size), [] (auto...) {} });
		tmp = tmp.subspan(chunk_size);
	}

	return list;
}
