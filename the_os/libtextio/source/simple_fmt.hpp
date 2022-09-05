#pragma once

#include <cstddef>
#include <cstdint>
#include <concepts>
#include <string_view>
#include <limits>
#include <bit>

#include "detail.hpp"

namespace textio::simple::fmt
{
	namespace detail
	{
		using namespace std::string_view_literals;

		template <typename T, auto Base>
		constexpr auto get_number_of_digits() -> std::size_t
		{
			if (std::has_single_bit(Base)) 
			{
				constexpr auto nbits = std::bit_width(Base - 1);
				return (sizeof(T) * 8 + nbits - 1) / nbits;				
			}

			if (Base == 10) 
			{
				return std::numeric_limits<T>::digits10 + std::is_signed_v<T> + 1;
			}

			if (Base >= 32) return get_number_of_digits<T, 32>();
			if (Base >= 16) return get_number_of_digits<T, 16>();
			if (Base >=  8) return get_number_of_digits<T,  8>();
			if (Base >=  4) return get_number_of_digits<T,  4>();
			if (Base >=  2) return get_number_of_digits<T,  2>();			
		}


		template <auto Base> 
		struct base_traits { static inline constexpr const auto prefix = ""sv; };

		template <> struct base_traits<2>  { static inline constexpr const auto prefix = "0b"sv; };
		template <> struct base_traits<4>  { static inline constexpr const auto prefix = "0q"sv; };
		template <> struct base_traits<8>  { static inline constexpr const auto prefix = "0o"sv; };
		template <> struct base_traits<10> { static inline constexpr const auto prefix = "0d"sv; };
		template <> struct base_traits<16> { static inline constexpr const auto prefix = "0x"sv; };

		template <typename T, auto Base>
		struct format_traits: base_traits<Base>
		{
			static inline constexpr const auto number_of_digits = get_number_of_digits<T, Base>();
		};

		template <std::integral T, auto Base, auto... Flags>
		struct format_base
		{
			static inline const constexpr auto base	= Base; // chosen base

			static inline const constexpr auto prefix_flag = ::textio::detail::one_of_v<'P', Flags...>;	// add prefix
			static inline const constexpr auto suffix_flag = ::textio::detail::one_of_v<'S', Flags...>;	// add suffix
			static inline const constexpr auto fixedw_flag = ::textio::detail::one_of_v<'W', Flags...>;	// fixed width
		
			T const& value;
		
			format_base(T const& value): value(value) {}
		};

		template <typename T, auto N>
		requires(N >= 0)
		struct const_repeat_impl
		{
			static inline constexpr const auto times = N; 
			T const& value;
		};
	}

	template <auto... Flags, std::integral T> auto bin(T const& value) -> detail::format_base<T, 2,  Flags...> { return { value }; }
	template <auto... Flags, std::integral T> auto qua(T const& value) -> detail::format_base<T, 4,  Flags...> { return { value }; }
	template <auto... Flags, std::integral T> auto oct(T const& value) -> detail::format_base<T, 8,  Flags...> { return { value }; }
	template <auto... Flags, std::integral T> auto dec(T const& value) -> detail::format_base<T, 10, Flags...> { return { value }; }
	template <auto... Flags, std::integral T> auto hex(T const& value) -> detail::format_base<T, 16, Flags...> { return { value }; }
	template <auto N, typename T> auto repeat(T const& value) -> detail::const_repeat_impl<T, N> { return { value }; }
}