#pragma once

#include <algorithm>

namespace details
{
	template <typename T>
	inline void reverse_bytes_inplace(T* bytes, std::size_t len)
	{
		std::reverse(reinterpret_cast<uint8_t*>(bytes),
			           reinterpret_cast<uint8_t*>(bytes) + len);
	}
}

template <typename T>
requires (std::is_integral_v<T>)
inline auto host_to_net(T value) -> T
{
	details::reverse_bytes_inplace(&value, sizeof (T));
	return value;
}

template <typename T>
requires (std::is_integral_v<T>)
inline auto net_to_host(T value) -> T
{
	return host_to_net(value);
}