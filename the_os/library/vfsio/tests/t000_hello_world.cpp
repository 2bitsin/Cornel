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
/*
	error	error_v { error::none };
	cstdfile output_v { error_v, "output.v"s, "w+b"s } ;	
	check_error(error_v);
		
	archive archive_v { error_v, &output_v };
	check_error(error_v);	

	archive_v.open(error_v, archive::directory, "this_is_a_very_long_directory_name"s);
	check_error(error_v);	
	archive_v.open(error_v, archive::file, "test.txt"s);
	check_error(error_v);	

	char const text[] = "Hello, world!";
	char const txet[] = "!dlrow ,olleH";

	for (auto i = 0; i < 20; ++i)
	{
		archive_v.write(error_v, utils::as_bytes(text));
		check_error(error_v);
	}	

	archive_v.close(error_v);
	check_error(error_v);

	archive_v.open(error_v, archive::file, "test2.txt"s);
	check_error(error_v);

	for (auto i = 0; i < 30; ++i)
	{
		archive_v.write(error_v, utils::as_bytes(txet));
		check_error(error_v);
	}	
*/

	::memory::block_list bl;
	bl.insert_range(std::span<std::byte>(new std::byte[16*1024*1024], 16*1024*1024));
	::memory::pretty_print(bl, stdout);

	
	vfsio::error error_v{ vfsio::error::none };
	
	heapfile hfl(error_v, bl, 16 * 1024 * 1024);
	check_error(error_v);

	for (auto i = 0u; i < 1000; ++i)
	{
		hfl.write(error_v, utils::as_bytes("Hello World!!!!"));
		check_error(error_v);
	}

	::memory::pretty_print(bl, stdout);
	
	__debugbreak();
	
	return 0;
}  
catch (std::exception const& ex)
{
	std::cerr<<ex.what()<<"\n";
	return 0;
}


