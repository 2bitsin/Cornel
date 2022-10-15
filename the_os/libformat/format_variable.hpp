#pragma once

namespace textio::fmt::detail
{
	template <meta::string String, size_t DefaultIndex>
	struct format_variable_parse
	{
		static inline constexpr auto colon_offset		= String.template find<':'>() != String.npos ? String.template find<':'>() : String.size();		
		static inline constexpr bool uses_default		= colon_offset == 0;		
		static inline constexpr auto index_string   = String.template substr<0, colon_offset>();
		
		static inline constexpr auto argument_index = !uses_default ? string_convert_base_v<unsigned, index_string> : DefaultIndex;
		static inline constexpr auto options_string = String.template substr<colon_offset + 1u>();		
	};	
}