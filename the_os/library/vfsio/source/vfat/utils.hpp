#pragma once

#include <type_traits>

namespace vfsio::vfat
{	
	template <typename Arg0, typename... ArgN>	
	static inline auto first_true_value(Arg0&& arg0, ArgN&&... argn) 
		-> std::common_type_t<Arg0, ArgN...>
	{		
		if (arg0 || !sizeof...(ArgN)) {
			return std::forward<Arg0>(arg0);
		}
		if constexpr (sizeof...(ArgN)) {
			return first_true_value(std::forward<ArgN>(argn)...);
		}		
	}

	template <typename Arg0, typename... ArgN>	
	requires (std::is_same_v<Arg0, ArgN> && ...)
	static inline auto&& first_true_value(Arg0&& arg0, ArgN&&... argn) 		
	{		
		if (arg0 || !sizeof...(ArgN)) {
			return std::forward<Arg0>(arg0);
		}
		if constexpr (sizeof...(ArgN)) {
			return first_true_value(std::forward<ArgN>(argn)...);
		}		
	}
}