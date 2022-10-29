#include <iterator>
#include <textio/logger.hpp>

#include "expect.hpp"

#ifdef TESTING
#define main t005_logger_basic
#endif

int main(int,char** const) 
{
  using namespace std::string_literals;
  using namespace std::string_view_literals;

	auto const expected = ""s;
	auto buffer = ""s;
	
	using textio::logger_base;
	using textio::logger_module_base;
	using textio::logger_module;

	logger_module<"TEST", std::back_insert_iterator<std::string>> zout{ std::back_inserter(buffer) };

	zout.level(logger_base::level_type::all);

	zout.fatal<"Hello world!">();
	zout.error<"Hello world!">();
	zout.warn<"Hello world!">();
	zout.debug<"Hello world!">();
	zout.info<"Hello world!">();
	zout.trace<"Hello world!">();
	
	
  expect_eq(buffer, expected);
}  

