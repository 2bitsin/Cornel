#include <type_traits>
#include <iostream>
#include <string_view>
#include <string>

#include <concepts>

#include "meta_string.hpp"
#include "meta_type_list.hpp"
#include "meta_value_list.hpp"
#include "format_string.hpp"
#include "format_base_convert.hpp"
#include "format_variable.hpp"


int main(int, char**)
{
  using namespace std::string_view_literals;
  using namespace std::string_literals; 
	using namespace textio::fmt::detail;
	
	//using variable_info = format_variable_parse<"1:2", 0u>;
	
	//std::cout << typeid(variable_info::index_string).name() << "\n";
	//std::cout << variable_info::index_string.as_string_view() << "\n";
	//std::cout << typeid(variable_info::argument_index).name() << "\n";
	//std::cout << variable_info::argument_index << "\n";

  const auto s = ::textio::fmt::format<"Hello {{o{{ {1:x} {0:x}">();
	std::cout << s << std::endl;

	//constexpr auto v_u = string_convert_base_v<unsigned, "1234ABCD", 16>;
	//std::cout << std::hex << v_u << "\n";	
	//constexpr auto v_i = string_convert_base_v<int, "-1234">;
	//std::cout << std::dec << v_i << "\n";
	
  return 0;
}