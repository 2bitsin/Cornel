#pragma once

#include <algorithm>

#ifdef _MSC_VER
	#include <intrin.h>
#endif

namespace details
{
	inline void reverse_bytes_inplace(void* bytes, std::size_t len)
	{
		std::reverse((std::byte*)(bytes), (std::byte*)(bytes) + len);
	}

	template <typename T>
	requires(std::is_trivial_v<T>)
	inline void reverse_bytes_inplace(T& value)
	{
		if constexpr (std::is_integral_v<T>)
		{
		#ifdef _MSC_VER

			if constexpr (sizeof(T) == 2u)
				value = (T &&)_byteswap_ushort((unsigned short const&)value);
			else if constexpr (sizeof(T) == 4u)
				value = (T &&)_byteswap_ulong((unsigned long const&)value);
			else if constexpr (sizeof(T) == 8u)
				value = (T &&)_byteswap_uint64((unsigned long long const&)value);
			else 
				reverse_bytes_inplace(&value, sizeof(value));
			
		#elif defined(__GNUC__) || defined(__clang__)

			if constexpr (sizeof(T) == 2u)
				value = (T &&)__builtin_bswap16(value);
			else if constexpr (sizeof(T) == 4u)
				value = (T &&)__builtin_bswap32(value);
			else if constexpr (sizeof(T) == 8u)
				value = (T &&)__builtin_bswap64(value);
			else 
				reverse_bytes_inplace(&value, sizeof(value));
			
		#else

			if constexpr (sizeof (T) > 1u) 
			{
				reverse_bytes_inplace(&value, sizeof(value));
			}
			
		#endif
		}
		else
		{
			if constexpr (sizeof (T) > 1u) 
			{
				reverse_bytes_inplace(&value, sizeof(value));
			}
		}
	}
}

template <typename T>
requires (std::is_integral_v<T>)
inline auto host_to_net(T value) -> T
{
	details::reverse_bytes_inplace(value);
	return value;
}

template <typename T>
requires (std::is_integral_v<T>)
inline auto net_to_host(T value) -> T
{
	details::reverse_bytes_inplace(value);
	return value;
}

template <typename T>
requires (std::is_integral_v<T>)
inline auto host_to_net_inplace(T& value) -> void
{
	details::reverse_bytes_inplace(value);
}

template <typename T>
requires (std::is_integral_v<T>)
inline auto net_to_host_inplace(T& value) -> void
{
	details::reverse_bytes_inplace(value);
}

