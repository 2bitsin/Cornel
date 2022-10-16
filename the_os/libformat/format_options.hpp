#pragma once

#include "meta_string.hpp"
#include "format_helper.hpp"

namespace textio::fmt::detail
{
	template <typename ValueT, typename CharT>
	struct format_options
	{
		using value_type	= ValueT;
		using char_type		= CharT;

		enum class fmt_type
		{
			none,
			binary,
			octal,
			decimal,
			lower_hex, 
			upper_hex,
			character,
			string,
			pointer,
			boolean,
			floating			
		};

		enum class fmt_align
		{
			left,
			right,
			center
		};
		
		fmt_type		format_type { fmt_type::none	};
		fmt_align		direction		{ fmt_align::left };
		char_type		fill_char		{ ' '							};		
		int32_t			width				{ 0								};
		int32_t			precision		{ 0								};		
		bool				prefix_base	{ false						};
		bool				pad_zeros		{ false						};
		bool				plus_sign		{ false						};
		
		template <size_t Size>
		constexpr format_options(meta::string<Size, char_type> const& value)
		{
			using namespace meta::literals;
			size_t index = 0;
			if (value.size() > 1)
			{
				if (is_any_of<"<^>"_Ts>(value[0]))
				{
					
				}
				
					
			}
		}		
	};
	
}