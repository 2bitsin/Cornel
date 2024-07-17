#pragma once 

#include <vfsio/vfsio.hpp>

#include <source_location>
#include <filesystem>
#include <fstream>

using namespace std::string_literals;

static inline void check_error(vfsio::error const& error_v, std::source_location const& loc = std::source_location::current())
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
inline static auto throw_fmt(std::_Basic_format_string<char, typename identity<K>::type...> fmt, K&&... args)
{
  throw Q(std::format(fmt, std::forward<K>(args)...));
}

inline static auto load (std::filesystem::path const& path_v) -> std::vector<std::byte>
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

inline static auto save (std::filesystem::path const& path_v, std::span<const std::byte> bytes_v) 
{	
	std::ofstream output_file_v(path_v, std::ios::binary);
	if (!output_file_v)
		throw_fmt("Failed to open file '{}' for writing.", path_v.string());
	output_file_v.write((char*)bytes_v.data(), bytes_v.size());
	if (!output_file_v)
		throw_fmt("Failed to write file '{}' from memory.", path_v.string());
}
