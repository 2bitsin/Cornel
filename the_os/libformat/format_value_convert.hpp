#pragma once

#include <concepts>
#include <algorithm>
#include <charconv>
#include <string>
#include <string_view>

#include "format_options.hpp"

namespace textio::fmt::detail
{

	template <typename Value_type, typename Char_type, meta::string Options>
	struct format_value_convert
	{
		static_assert(sizeof(Value_type*) == 0, "Unimplemented");
	}; 

	template <typename Value_type, typename Char_type, meta::string Options>
	requires (std::is_integral_v<Value_type>)
	struct format_value_convert<Value_type, Char_type, Options>
	{		
		using value_type	= Value_type;
		using char_type		= Char_type;
		
		static inline constexpr auto options = format_options<value_type, char_type>{ Options };				
		static inline constexpr auto minimum_buffer_size = std::max(options.pad_zeros * options.width + options.prefix_base * 2u + 1u, 0x80u);

		template <std::output_iterator<char> OIterator>
		static inline auto apply(OIterator o_iterator, value_type const& value) -> OIterator
		{		
			using format_sv = format_value_convert<std::basic_string_view<char_type>, char_type, Options>;
			
			char_type buffer[minimum_buffer_size];
			constexpr auto prefix = options.prefix_string();
			
			auto buff_i = std::begin(buffer);						

			if constexpr (options.prefix_base) {
				
				buff_i = std::copy_n(prefix.begin(), prefix.size(), buff_i);
			}

			if constexpr (options.pad_zeros) {
				buff_i = std::fill_n(buff_i, options.width - prefix.size(), char_type('0'));
			}

			auto [end, errc] = std::to_chars(buffer, buffer + minimum_buffer_size, value, options.base());
			
			

			return o_iterator;
		}
	}; 
	

	template <typename Char_type, meta::string Options, typename... Q>	
	struct format_value_convert<std::basic_string_view<Char_type, Q...>, Char_type, Options>
	{		
		using char_type		= Char_type;
		using value_type	= std::basic_string_view<char_type, Q...>;
				
		static inline constexpr auto options = format_options<value_type, char_type>{ Options };
		
		template <std::output_iterator<char> OIterator>
		static inline auto apply(OIterator o_iterator, value_type const& value) -> OIterator
		{
			if (options.width < value.size()) {
				return std::copy(value.begin(), value.end(), o_iterator);
			}

			const auto total_padding = options.width - value.size();
			switch (options.direction)
			{
			case fmt_align::left:
				o_iterator = std::copy(value.begin(), value.end(), o_iterator);
				return std::fill_n(o_iterator, total_padding, options.fill_char);
			case fmt_align::right:
				o_iterator = std::fill_n(o_iterator, total_padding, options.fill_char);
				return std::copy(value.begin(), value.end(), o_iterator);
			case fmt_align::center:
				{
					const auto left_padding = total_padding / 2;
					const auto right_padding = total_padding - left_padding;
					o_iterator = std::fill_n(o_iterator, left_padding, options.fill_char);
					o_iterator = std::copy(value.begin(), value.end(), o_iterator);
					return std::fill_n(o_iterator, right_padding, options.fill_char);
				}
			case fmt_align::none:
				return std::copy(value.begin(), value.end(), o_iterator);
			}
		}
			
	}; 
}