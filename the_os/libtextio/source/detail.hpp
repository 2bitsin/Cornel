#pragma once

#include <type_traits>

namespace textio::detail
{
	template<typename... T>
	struct one_of_type_t;

	template<typename T>
	struct one_of_type_t<T>: 
		public std::false_type
	{};

	template<typename T, typename ... Q>
	struct one_of_type_t<T, T, Q...>:
		public std::true_type
	{};
	
	template<typename T, typename U, typename... Q>
	struct one_of_type_t<T, U, Q...>: 
		public one_of_type_t<T, Q...> 
	{};
	
	template <typename... T>
	static inline constexpr bool one_of_type_v = one_of_type_t<T...>::value;
	
	template <typename... T>
	concept one_of_type = one_of_type_v<T...>;
}