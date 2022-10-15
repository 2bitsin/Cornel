#pragma once

namespace textio::fmt::detail
{
	template <meta::string String, size_t DefaultIndex>
	struct format_variable
	{
		static inline constexpr auto colon_offset	= String.template find<':'>() != String.npos ? String.template find<':'>() : String.size();		
		static inline constexpr auto index_string = String.template substr<0, colon_offset>();		
		static inline constexpr auto uses_default	= index_string.empty();
		static inline constexpr auto argument_index = ([]() constexpr { if constexpr (uses_default) return DefaultIndex; else return string_convert_base_v<size_t, index_string>; })();
		static inline constexpr auto options_string = String.template substr<colon_offset + 1u>();
		static inline constexpr auto next_default_index = DefaultIndex + !!uses_default;
	};	
}