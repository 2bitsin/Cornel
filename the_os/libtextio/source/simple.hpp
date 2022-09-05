#pragma once

#include <cassert>

#include <algorithm>
#include <ranges>
#include <iterator>
#include <string_view>
#include <functional>
#include <limits>
#include <charconv>

#include "detail.hpp"
#include "consts.hpp"
#include "simple_fmt.hpp"

namespace textio::simple
{
	template <std::output_iterator<char> I, typename... Args>
	requires (sizeof...(Args) > 1)
	static inline auto write(I out_i, Args&&... args) -> I;
	
	/******************************/
	/*														*/	
	/* Individual write overloads */
	/*														*/	
	/******************************/

	template <std::output_iterator<char> I, std::integral Arg0>	
	static inline auto write(I out_i, Arg0 const& arg0) -> I;

	template <std::output_iterator<char> I>
	static inline auto write(I out_i, std::string_view value) -> I
	{
		for(auto subv: value)
			out_i = write(out_i, subv);
		return out_i;
	}

	template <std::output_iterator<char> I, std::size_t N>
	static inline auto write(I out_i, char const (&value) [N]) -> I
	{		
		using std::string_view;
		return write(out_i, string_view(value));
	}

	template <std::output_iterator<char> I, std::integral Arg0>	
	static inline auto write(I out_i, Arg0 const& arg0) -> I
	{
		if constexpr (std::is_same_v<Arg0, char>)
		{
			*out_i++ = arg0;
			return out_i;
		}
		else if constexpr (std::is_same_v<Arg0, bool>)
		{
			return write(out_i, arg0 ? "true" : "false");
		}
		else
		{
			static const constexpr auto length = std::numeric_limits<Arg0>::digits10 + 2;
			char buffer [length];
			static_assert(length > 1);
			std::ranges::fill(buffer, '\0');
			std::to_chars_result result;
			result = std::to_chars(std::begin(buffer), std::end(buffer), arg0);
			assert(result.ec == std::errc());
			return write(out_i, std::string_view(std::begin(buffer), result.ptr));
		}		
	}	

	template <std::output_iterator<char> I, typename T, auto N>
	static inline auto write(I out_i, fmt::detail::const_repeat_impl<T, N> what) -> I
	{		
		for(auto i = 0u; i < what.times; ++i) {
			out_i = write(out_i, what.value);
		}
		return out_i;
	}

	template <std::output_iterator<char> I, typename T>
	static inline auto write(I out_i, fmt::detail::repeat_impl<T> what) -> I
	{		
		for(auto i = 0u; i < what.times; ++i) {
			out_i = write(out_i, what.value);
		}
		return out_i;
	}


	template <std::output_iterator<char> I, typename T, auto Base, auto... Flags>
	static inline auto write(I out_i, fmt::detail::format_base<T, Base, Flags...> what) -> I
	{		
		char buffer [what.number_of_digits];

		if (what.prefix_flag) {
			out_i = write(out_i, what.prefx);
		}

		std::to_chars_result result;
		result = std::to_chars(std::begin(buffer), std::end(buffer), what.value, what.base);
		assert(result.ec == std::errc());

		const std::string_view value_s{ std::begin(buffer), result.ptr };

		//TODO : fix way signed values are handled
		if (what.fixedw_flag) {
			out_i = write(out_i, fmt::repeat('0', what.number_of_digits - value_s.size()));
		}

		out_i = write(out_i, value_s);
		return out_i;
	}

	
	/*****************************/
	/*													 */	
	/* Catch-all write overloads */
	/*													 */	
	/*****************************/
	

	template <std::output_iterator<char> I, typename... Args>
	requires (sizeof...(Args) > 1)
	static inline auto write(I out_i, Args&&... args) -> I
	{				
		((out_i = write(out_i, std::forward<Args>(args))),...);
		return out_i;
	}

	template <std::output_iterator<char> I, typename... Args>
	requires (sizeof...(Args) >= 1)
	static inline auto writeln(I out_i, Args&&... args) -> I
	{		
		return write(out_i, std::forward<Args>(args)..., consts::crlf_s);
	}

}