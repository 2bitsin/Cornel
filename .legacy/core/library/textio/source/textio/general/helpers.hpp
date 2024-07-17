#pragma once

#include <type_traits>

namespace textio::detail
{
	 /*************************
		* 
		*		one_of_type<T, Q...>
		* 
		************************* 
		*/

	template<typename... T>
	struct one_of_type_impl;

	template<typename T>
	struct one_of_type_impl<T>: 
		public std::false_type
	{};

	template<typename T, typename ... Q>
	struct one_of_type_impl<T, T, Q...>:
		public std::true_type
	{};
	
	template<typename T, typename U, typename... Q>
	struct one_of_type_impl<T, U, Q...>: 
		public one_of_type_impl<T, Q...> 
	{};
	
	template <typename... T>
	static inline constexpr bool one_of_type_v = one_of_type_impl<T...>::value;
	
	template <typename... T>
	concept one_of_type = one_of_type_v<T...>;

	 /*************************
		* 
		*		one_of_value
		* 
		************************* 
		*/

	template <auto... T>
	struct one_of_value_impl;

	template <auto T>
	struct one_of_value_impl<T>: 
		public std::false_type 
	{};

	template<auto T, auto ... Q>
	struct one_of_value_impl<T, T, Q...>:
		public std::true_type
	{};

	template<auto T, auto U, auto... Q>
	struct one_of_value_impl<T, U, Q...>: 
		public one_of_value_impl<T, Q...> 
	{};

	template <auto T, auto... Q>
	static inline constexpr auto one_of_v = one_of_value_impl<T, Q...>::value;

	template <typename T, typename O>
	concept is_writable = requires(T && t, O && o) {
		{ t.write(o) } -> std::convertible_to<O>;
	};

}