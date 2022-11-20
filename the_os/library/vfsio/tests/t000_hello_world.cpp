#include <iterator>
#include <vector>
#include <fstream>
#include <filesystem>
#include <cassert>
#include <bit>
#include <span>
#include <source_location>
#include <format>

#include "expect.hpp"

#include <textio/logger.hpp>
#include <utils/utils.hpp>
#include <memory/block_list.hpp>

#include <vfsio/vfsio.hpp>
#include <vfsio/archive.hpp>
#include <vfsio/cstdfile.hpp>
#include <vfsio/heapfile.hpp>

#include <utils/utils.hpp>

using namespace std::string_literals;

void check_error(vfsio::error const& error_v, std::source_location const& loc = std::source_location::current())
{
	if (error_v != vfsio::error::none)
	{		
		throw std::runtime_error(std::format("Error: {} at {}:{}:{}", std::to_underlying(error_v), loc.file_name(), loc.line(), loc.column()));
	}
}

int main(int argc,char** const argv) 
try
{
	using namespace vfsio;

	using namespace std::filesystem;

	current_path(path(argv[0]).parent_path());

	error	error_v { error::none };
	cstdfile output_v { error_v, "output.v"s, "w+b"s } ;	
	check_error(error_v);
		
	archive archive_v { error_v, &output_v };
	check_error(error_v);	

	archive_v.open(error_v, archive::directory, "dir0"s);
	check_error(error_v);	
	archive_v.open(error_v, archive::file, "test1.txt"s);
	check_error(error_v);	

	char const text[] = "Hello, world!";
	char const txet[] = "!dlrow ,olleH";
	char const ends[] = "EOF";

	for (auto i = 0; i < 20; ++i)
	{
		archive_v.write(error_v, utils::as_bytes(text));
		check_error(error_v);
	}	
	archive_v.write(error_v, utils::as_bytes(ends));
	check_error(error_v);

	archive_v.close(error_v);
	check_error(error_v);

	archive_v.open(error_v, archive::file, "test2.txt"s);
	check_error(error_v);

	for (auto i = 0; i < 30; ++i)
	{
		archive_v.write(error_v, utils::as_bytes(txet));
		check_error(error_v);
	}	
	archive_v.write(error_v, utils::as_bytes(ends));
	check_error(error_v);

	archive_v.close(error_v);
	check_error(error_v);
	archive_v.close(error_v);
	check_error(error_v);

	memory::buffer<std::byte> test1_txt(16);
	archive_v.load(error_v, "dir0/test1.txt", test1_txt);		
	check_error(error_v);

	archive_v.open(error_v, archive::directory, "dir1");
	check_error(error_v);
	archive_v.open(error_v, archive::directory, "test1.txt");
	check_error(error_v);

	for (auto i = 0; i < 30; ++i)
	{
		archive_v.write(error_v, utils::as_bytes(txet));
		check_error(error_v);
		archive_v.write(error_v, utils::as_bytes(text));
		check_error(error_v);
	}	
	archive_v.write(error_v, utils::as_bytes(ends));
	check_error(error_v);

	archive_v.close(error_v);
	check_error(error_v);
	
	archive_v.open(error_v, archive::directory, "test2.txt");
	check_error(error_v);
	
	for (auto i = 0; i < 30; ++i)
	{
		archive_v.write(error_v, utils::as_bytes(text));
		check_error(error_v);
		archive_v.write(error_v, utils::as_bytes(txet));
		check_error(error_v);
	}	
	archive_v.write(error_v, utils::as_bytes(ends));
	check_error(error_v);

	archive_v.close(error_v);
	check_error(error_v);
	
	archive_v.close(error_v);
	check_error(error_v);

	test1_txt = memory::buffer<std::byte>(16);
	archive_v.load(error_v, "dir0/test2.txt", test1_txt);
	check_error(error_v);
	archive_v.load(error_v, "dir1/test1.txt", test1_txt);
	check_error(error_v);

	
		
	return 0;
}  
catch (std::exception const& ex)
{
	std::cerr<<ex.what()<<"\n";
	return 0;
}


