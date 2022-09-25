#pragma once

#include <cstddef>
#include <cstdint>

#include <concepts>

#include <textio/simple.hpp>

namespace textio::simple::fmt::detail
{
	template <typename T, auto... Flags> 
	struct data_size_impl
	{
		T const& value;

		data_size_impl(T const& value): value(value) { }
		
		template <std::output_iterator<char> O>
		inline auto write(O out_i) const noexcept -> O
		{
			static constexpr auto sz_K = 1024u;
			static constexpr auto sz_M = 1024u * sz_K;
			static constexpr auto sz_G = 1024u * sz_M;
			static constexpr auto sz_T = 1024u * sz_M;
			
			auto temp_value = value;			
			
			if (temp_value >= sz_T) {
				out_i = textio::simple::write((temp_value / sz_T), "T ");
				temp_value %= sz_T;
			}
			
			if (temp_value >= sz_G) {
				out_i = textio::simple::write((temp_value / sz_G), "G ");
				temp_value %= sz_G;
			}

			if (temp_value >= sz_M) {
				out_i = textio::simple::write((temp_value / sz_M), "M ");
				temp_value %= sz_M;
			}

			if (temp_value >= sz_K) {
				out_i = textio::simple::write((temp_value / sz_K), "K ");
				temp_value %= sz_K;
			}
			
			if (temp_value > 0 && value == 0) {
				out_i = textio::simple::write(temp_value);
			}
			
			return out_i;
		}
	};
}

namespace textio::simple::fmt
{
	template <typename T, auto... Flags>
	inline auto data_size(T const& value) -> detail::data_size_impl<T, Flags...>
	{
		return detail::data_size_impl<T, Flags...>(value);
	}
}