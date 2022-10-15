#include <type_traits>
#include <iostream>
#include <string_view>
#include <string>

#include <concepts>

#include "meta_string.hpp"
#include "meta_type_list.hpp"
#include "meta_value_list.hpp"
#include "format_string.hpp"

template <typename T, auto Value>
struct digit_to_integer
{
	static inline constexpr auto value = ([] (auto v) constexpr 
	{
		     if (v >= '0' && v <= '9') return T(v - '0');
		else if (v >= 'a' && v <= 'z') return T(v - 'a' + 10);
		else if (v >= 'A' && v <= 'Z') return T(v - 'A' + 10);
		else throw std::invalid_argument("Not a valid digit");
	})(Value);
};

template <typename T, auto Value>
inline constexpr auto digit_to_integer_v = digit_to_integer<T, Value>::value;

template <typename T>
struct digit_to_integer_bind
{
	template <auto Value>
	struct type
	{
		static inline constexpr auto value = digit_to_integer_v<T, Value>;
	};
};

template <typename T>
using digit_to_integer_bind_t = typename digit_to_integer_bind<T>::template type;

template <typename T, typename DigitList, auto Base = 10>
struct digit_convert_base_impl;


template <typename T, auto Base, auto Digit0>
struct digit_convert_base_impl<T, meta::value_list<Digit0>, Base>
{
	static_assert(Digit0 < Base, "Digit is out of range");
	static inline constexpr T value = T(Digit0);
};

template <typename T, auto Base, auto Digit0, auto... DigitN>
struct digit_convert_base_impl<T, meta::value_list<Digit0, DigitN...>, Base>
{
	static_assert(Digit0 < Base, "Digit is out of range");
	static inline constexpr T value = Base * digit_convert_base_impl<T, meta::value_list<DigitN...>, Base>::value + Digit0;
};

template <typename T, typename CharList, auto Base = 10>
static inline constexpr T base_convert_v = digit_convert_base_impl<T, meta::value_list_map<CharList, digit_to_integer_bind_t<T>, Base>>::value;


template <typename T, meta::string String, auto Base = 10>
static inline constexpr T string_base_convert_v = digit_convert_base_impl<T, meta::value_list_map<meta::value_list_from_array<meta::string_truncate_v<String>>, digit_to_integer_bind_t<T>>, Base>::value;




int main(int, char**)
{
  using namespace std::string_view_literals;
  using namespace std::string_literals; 

  //const auto s = ::textio::fmt::format<"Hello {{o{{ {1:x} {0:x}">();
	//std::cout << s << std::endl;

	constexpr auto v = string_base_convert_v<int, "1234">;

	std::cout << v << "\n";

  return 0;
}