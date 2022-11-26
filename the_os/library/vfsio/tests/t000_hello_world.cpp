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
#include <vfsio/archive_view.hpp>
#include <vfsio/cstdfile.hpp>
#include <vfsio/heapfile.hpp>

#include <utils/utils.hpp>

#include "helpers.hpp"

int main(int argc,char** const argv) 
try
{
	using namespace vfsio;

	using namespace std::filesystem;

	error	error_v { error::none };
	current_path(path(argv[0]).parent_path());
	
	const std::size_t N = 16*1024*1024;
	memory::block_list bl;
	bl.insert_range(std::span{new std::byte[N], N});

	heapfile heap_v{ error_v, bl, N };
	check_error(error_v);
	archive archive_v { error_v, &heap_v };
	check_error(error_v);
	archive_v.open(error_v, archive_v.file, "test.txt");
	check_error(error_v);
	archive_v.resize(error_v, 161);
	check_error(error_v);
	
	const auto test_v = utils::as_bytes("\x01\x02\x03\x04\x05\x06\x07\x08\x09\x0A\x0B\x0C\x0D\x0E\x0F");
	
	for (auto i = 0; i < 10; ++i) {		
		auto unwritten_bytes_v = archive_v.write(error_v, test_v);				
		check_error(error_v);
	}

	archive_v.close(error_v);
	check_error(error_v);

	archive_v.open(error_v, archive_v.file, "test2.txt");
	check_error(error_v);
	archive_v.resize(error_v, 321);
	check_error(error_v);
	
	for (auto i = 0; i < 20; ++i) {
		auto unwritten_bytes_v = archive_v.write(error_v, test_v);		
		check_error(error_v);
	}
	
	archive_v.close(error_v);
	check_error(error_v);

	save("./test.v", heap_v.view());
	pretty_print(bl, stdout);

#if 0
	{
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

		archive_v.open(error_v, archive::directory, "dir1");
		check_error(error_v);
		archive_v.open(error_v, archive::file, "test1.txt");
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
		
		archive_v.open(error_v, archive::file, "test2.txt");
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
	}	

	auto buffer_v = load("output.v");
	vfsio::archive_view archive_view_v { buffer_v };
	
	auto[bytes_v, type_v] = archive_view_v.find(error_v, "dir1/test1.txt");
	check_error(error_v);
#endif	

	return 0;
}  
catch (std::exception const& ex)
{
	std::cerr<<ex.what()<<"\n";
	return 0;
}


