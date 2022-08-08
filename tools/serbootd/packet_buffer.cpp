#include "packet_buffer.hpp"
#include "packet_buffer.hpp"
#include "packet_buffer.hpp"
#include "packet_buffer.hpp"
#include <new>
#include <stdexcept>
#include <algorithm>

#include "packet_buffer.hpp"
#include "common/crc32.hpp"

template<typename T>
packet_buffer<T>::packet_buffer()
:	m_databits { },
	m_deleter { [](auto...){} }
{
}

template <typename T>
packet_buffer<T>::packet_buffer(packet_data databits, deleter_type deleter)
:	m_databits	{ std::move(databits) },
	m_deleter		{ std::move(deleter) }
{}

template<typename T>
packet_buffer<T>::packet_buffer(std::size_t size)
: m_databits	{ new T[size], size},
	m_deleter		{ [this](auto what){ delete [] what.data(); }}
{}

template<typename T>
packet_buffer<T>::packet_buffer(clone_from_t, ct_packet_data databits)
:	packet_buffer<T>{ databits.size() }
{
	copy_from(databits);
}

template <typename T>
packet_buffer<T>::packet_buffer(packet_buffer&& other)
:	m_databits	{ std::move(other.m_databits) },
	m_deleter		{ std::move(other.m_deleter) }
{}

template <typename T>
packet_buffer<T>::~packet_buffer()
{
	if (m_deleter)
		m_deleter(m_databits);
}

template <typename T>
auto packet_buffer<T>::operator=(packet_buffer&& other) 
	-> packet_buffer<T>&
{
	this->~packet_buffer();
	new (this) packet_buffer(std::move(other));
	return *this;
}

template <typename T>
auto packet_buffer<T>::bytes() const noexcept -> ct_packet_data
{
	return m_databits;
}

template <typename T>
auto packet_buffer<T>::bytes() noexcept -> packet_data
{
	return m_databits;
}

template <typename T>
auto packet_buffer<T>::size() const noexcept -> std::size_t
{
	return m_databits.size();
}

template<typename T>
auto packet_buffer<T>::data() const noexcept -> const T*
{
	return m_databits.data();
}

template<typename T>
auto packet_buffer<T>::data() noexcept -> T*
{
	return m_databits.data();
}

template<typename T>
auto packet_buffer<T>::deleter() const noexcept -> deleter_type
{
	return m_deleter;
}

template<typename T>
auto packet_buffer<T>::deleter(deleter_type deleter) noexcept -> deleter_type
{
	return std::exchange(m_deleter, std::move(deleter));
}

template<typename T>
auto packet_buffer<T>::crc32() const noexcept -> std::uint32_t
{
  return ::crc32(m_databits, 0u);
}

template<typename T>
void packet_buffer<T>::copy_from(std::span<const T> bits)
{
	std::copy(bits.begin(), bits.begin() + std::min(m_databits.size(), bits.size()), m_databits.begin());
}

template <typename T>
auto packet_buffer<T>::copy_from(std::string_view bits) -> void
{
	std::transform(bits.begin(), bits.begin() + std::min(size(), bits.size()), m_databits.begin(), [] (auto ch) { return T(ch); });
}

template<typename T>
void packet_buffer<T>::copy_into(packet_buffer& other)
{
	if (other.size() > size())
	{
		throw std::logic_error("Can't clone into smaller packet then the original packet.");
	}

	std::copy(data(), data() + size(), other.data());
	if (other.size() > size())
		std::fill(other.data() + size(), other.data() + other.size(), 0);
}

template<typename T>
bool packet_buffer<T>::copy_into(packet_buffer& other, std::error_code& ec) noexcept
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

template<typename T>
auto packet_buffer<T>::clone() -> packet_buffer
{
	packet_buffer pbuf{ size() };
	copy_into(pbuf);
	return pbuf;
}

template<typename T>
auto packet_buffer<T>::fill(T value) -> void
{
	std::fill(m_databits.begin(), m_databits.end(), value);
}


template<typename T>
auto packet_buffer<T>::split(std::size_t chunk_size) const -> std::vector<packet_buffer>
{
	const auto number_of_chunks = ((*this).size() + chunk_size - 1u) / chunk_size;
	
	std::vector<packet_buffer> list;	
	list.reserve(number_of_chunks);
	
	auto tmp = m_databits;
	while (!tmp.empty())
	{
		auto subsp = tmp.subspan(0, chunk_size);
		list.emplace_back(subsp, [](auto...) {});
		tmp = tmp.subspan(chunk_size);
	}

	return list;
}

template struct packet_buffer<byte>;
