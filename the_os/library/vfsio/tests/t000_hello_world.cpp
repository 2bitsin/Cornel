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

using namespace std::string_literals;

void check_error(vfsio::error const& error_v, std::source_location const& loc = std::source_location::current())
{
	if (error_v == vfsio::error::none)
		return ;
	const auto s = std::format("Error: {} at {}:{}:{}", (int)error_v, loc.file_name(), loc.line(), loc.column());
	throw std::runtime_error(s);
}

template <typename I>
struct identity {
	using type = I;
};

template <typename Q = std::runtime_error, typename ... K>
static auto throw_fmt(std::_Basic_format_string<char, typename identity<K>::type...> fmt, K&&... args)
{
  throw Q(std::format(fmt, std::forward<K>(args)...));
}

static auto load (std::filesystem::path const& path_v) -> std::vector<std::byte>
{
	if (!std::filesystem::exists(path_v))
		throw_fmt("File '{}' does not exist.", path_v.string());
	if (!std::filesystem::is_regular_file(path_v))
		throw_fmt("File '{}' is not a regular file.", path_v.string());
	const auto length_v = std::filesystem::file_size(path_v);
	if (length_v < 1u)
		return {};
	std::vector<std::byte> buffer_v (length_v);
	std::ifstream input_file_v (path_v, std::ios::binary);
	if (!input_file_v)
		throw_fmt("Failed to open file '{}' for reading.", path_v.string());
	input_file_v.read((char*)buffer_v.data(), length_v);
	if (!input_file_v || input_file_v.gcount() != length_v)
		throw_fmt("Failed to read file '{}' into memory.", path_v.string());
	return buffer_v;
}


static auto save (std::filesystem::path const& path_v, std::span<const std::byte> bytes_v) 
{	
	std::ofstream output_file_v(path_v, std::ios::binary);
	if (!output_file_v)
		throw_fmt("Failed to open file '{}' for writing.", path_v.string());
	output_file_v.write((char*)bytes_v.data(), bytes_v.size());
	if (!output_file_v)
		throw_fmt("Failed to write file '{}' from memory.", path_v.string());
}

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


