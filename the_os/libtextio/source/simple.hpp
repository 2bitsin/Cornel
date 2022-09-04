#pragma once

#include <algorithm>
#include <ranges>
#include <iterator>
#include <string_view>
#include <functional>

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

	template <std::output_iterator<char> I, typename Arg0>
	static inline auto write(I out_i, Arg0&& arg0) -> I
	{
		static_assert(!sizeof(Arg0*), "This shouldn't happen");
		return out_i;
	}

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