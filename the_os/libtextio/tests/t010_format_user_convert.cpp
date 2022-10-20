#include <iterator>
#include <textio/format.hpp>
#include "expect.hpp"

#ifdef TESTING
#define main t010_format_convert_value
#endif

struct vec3i
{
	int x, y, z;
	
	inline auto format(std::output_iterator<char> auto o_i, auto const& options) const
	{
		return textio::fmt::format_to<"vec3i({0}, {1}, {2})">(o_i, x, y, z);
	}
};

struct box2u
{
	unsigned x0,y0;
	unsigned x1,y1;
};

template <typename Char_type, meta::string Options>
struct textio::fmt::user_convert<box2u, Char_type, Options>
{	
	static inline auto apply(std::output_iterator<char> auto o_iterator, box2u const& value)
	{
		return textio::fmt::format_to<"box2u[({0}, {1}), ({2}, {3})]">(o_iterator, value.x0, value.y0, value.x1, value.y1);
	}
};


int main(int,char** const) 
{
  using namespace std::string_literals;
  using namespace std::string_view_literals;
	using namespace textio::fmt;
	using namespace textio::fmt::literals;


	auto const expected = "vec3i(1, 2, 3)\nbox2u[(1, 2), (3, 4)]\n"s;
			
	constexpr auto fmt_s0 = "{}\n{}\n"_fmt;
	
	fmt_s0.to(stderr, vec3i{1,2,3}, box2u{1,2,3,4});
	const auto buffer = fmt_s0.as<std::string>(vec3i{1,2,3}, box2u{1,2,3,4});
	
	// TODO : WRITE MORE TESTS
  expect_eq(buffer, expected);
}  

