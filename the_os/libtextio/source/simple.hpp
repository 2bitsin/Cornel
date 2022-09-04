#pragma once

#include <algorithm>
#include <ranges>
#include <iterator>
#include <string_view>
#include <functional>
#include <limits>
#include <charconv>

#include "detail.hpp"
#include "consts.hpp"

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


	template <std::output_iterator<char> I>
	static inline auto write(I out_i, char value) -> I
	{		
		*out_i++ = value;
		return out_i;
	}	

	template <std::output_iterator<char> I>
	static inline auto write(I out_i, std::string_view value) -> I
	{
		for(auto subv: value)
			out_i = write(out_i, subv);
		return out_i;
	}

	template <std::output_iterator<char> I, std::integral Arg0, typename Q = std::decay_t<Arg0>>
	requires (!std::is_same_v<Q, char>)
	static inline auto write(I out_i, Arg0&& arg0) -> I
	{
		static const constexpr auto length = std::numeric_limits<Q>::digits10 + 1;
		char buffer [length];
		static_assert(length > 1);

		std::ranges::fill(buffer, '\0');
		auto result = std::to_chars(std::begin(buffer), std::end(buffer), std::forward<Arg0>(arg0));

		return write(out_i, std::string_view(std::begin(buffer), result.ptr));
	}	

	template <std::output_iterator<char> I, std::size_t N>
	static inline auto write(I out_i, char const (&value) [N]) -> I
	{		
		using std::string_view;
		return write(out_i, string_view(value));
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