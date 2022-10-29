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

	logger_module<"TEST", std::back_insert_iterator<std::string>> Glog{ std::back_inserter(buffer) };

	logger_base::level(logger_base::level_type::all);

	Glog.fatal	<"Hello world!"> ();
	Glog.error	<"Hello world!"> ();
	Glog.warn		<"Hello world!"> ();
	Glog.debug	<"Hello world!"> ();
	Glog.info		<"Hello world!"> ();
	Glog.trace	<"Hello world!"> ();
	

	logger_base::level(logger_base::level_type::warning);
	
	Glog.fatal	<"Goodbye world!"> ();
	Glog.error	<"Goodbye world!"> ();
	Glog.warn		<"Goodbye world!"> ();
	Glog.debug	<"Goodbye world!"> ();
	Glog.info		<"Goodbye world!"> ();
	Glog.trace	<"Goodbye world!"> ();

  expect_eq(buffer, expected);
}  

