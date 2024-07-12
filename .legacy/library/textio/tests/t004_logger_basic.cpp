#include <iterator>
#include <textio/logger.hpp>

#include "expect.hpp"

declare_module(Memory);

int main(int,char** const) 
{
  using namespace std::string_literals;
  using namespace std::string_view_literals;

	auto const expected = ""s;
	auto buffer = ""s;
	
	using textio::logger_base;
	using textio::logger_module_base;
	using textio::logger_module;

	logger_module<meta::empty_string_v<char>, std::back_insert_iterator<std::string>> Glog{ std::back_inserter(buffer) };
	logger_module<meta::string_truncate_v<"T000">, std::back_insert_iterator<std::string>> Gmod{std::back_inserter(buffer)};
	
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

	
	logger_base::level(logger_base::level_type::all);
	
	Gmod.fatal	<"Hello world!"> ();
	Gmod.error	<"Hello world!"> ();
	Gmod.warn		<"Hello world!"> ();
	Gmod.debug	<"Hello world!"> ();
	Gmod.info		<"Hello world!"> ();
	Gmod.trace	<"Hello world!"> ();
	
	logger_base::level(logger_base::level_type::warning);
	
	Gmod.fatal	<"Goodbye world!"> ();
	Gmod.error	<"Goodbye world!"> ();
	Gmod.warn		<"Goodbye world!"> ();
	Gmod.debug	<"Goodbye world!"> ();
	Gmod.info		<"Goodbye world!"> ();
	Gmod.trace	<"Goodbye world!"> ();
	
  expect_eq(buffer, expected);

}  

