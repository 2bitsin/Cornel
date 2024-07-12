#include <cstddef>
#include <cstdint>

#include <iostream>
#include <fstream>
#include <vector>
#include <string_view>
#include <format>
#include <filesystem>
#include <span>
#include <iterator>

#include "helpers.hpp"

#pragma pack(push, 1)
struct header
{ 
  uint32_t size;
  uint8_t  type;
  uint8_t  name_len;
  char     name[250];
};
static_assert(sizeof(header) == 256);
#pragma pack(pop)

static auto header_size(header const& header_v) -> std::size_t
{
  return offsetof(header, name) + header_v.name_len;
}



auto header_from_bytes(std::span<const std::byte> bytes_v) 
	-> header const&
{
	if (bytes_v.size() < offsetof(header, name))
		throw_fmt("Not enough bytes to form a header");
	header const& header_v = *std::launder((header const*)bytes_v.data());
	if (bytes_v.size() < header_size(header_v))
		throw_fmt("Not enough bytes to form a header");
	return header_v;
}

static constexpr inline auto align(auto offset_v) 
{
	return (offset_v + 0xfu) & ~(decltype(offset_v))0xfu;
}

static auto extract_from_bytes(std::span<const std::byte> bytes_v, std::filesystem::path outdir_v = "./tmp")
	-> void
{
	static std::ostream_iterator<char> stdout_i (std::cout);
	std::size_t offset_v = 0;
	std::size_t ending_v = bytes_v.size();	
	while(offset_v < bytes_v.size())
	{
		auto const& header_v = header_from_bytes(bytes_v.subspan(offset_v));	
		
		ending_v = align(offset_v + header_v.size);
		offset_v += header_size(header_v);
		const auto sub_size_v = std::min(ending_v - offset_v, bytes_v.size() - offset_v);
		
		std::string_view name_v(header_v.name, header_v.name_len);		
		if (const auto i = name_v.find_last_not_of('\0'); i != name_v.npos)
			name_v = name_v.substr(0, i+1);
		const auto new_full_path_v = outdir_v / name_v;				

		const auto sub_bytes_v = bytes_v.subspan(offset_v, sub_size_v);

		if (header_v.type == 0)
		{
			std::format_to(stdout_i, "Creating directory '{}'...\n", new_full_path_v.generic_string());
			std::filesystem::create_directories(new_full_path_v);
			save (new_full_path_v.string() + ".dir", sub_bytes_v);
			extract_from_bytes(sub_bytes_v, new_full_path_v);
		}
		else 
		{
			std::format_to(stdout_i, "Writing file '{}'...\n", new_full_path_v.generic_string());
			save (new_full_path_v.string(), sub_bytes_v);
		}		

		offset_v = ending_v;
	}	
}


int main (int argc, char** argv)
{
  std::vector<std::filesystem::path> args(argv, argv + argc);
#ifndef _DEBUG
	auto buffer_v = load(args.at(1));
#else
	std::filesystem::current_path(args.at(0).parent_path());
	auto buffer_v = load("output.v");
#endif

	extract_from_bytes(buffer_v);

  return 0;
}