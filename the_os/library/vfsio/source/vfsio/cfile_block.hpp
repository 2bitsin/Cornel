#pragma once

#include <cstddef>
#include <cstdint>
#include <cstdio>

#include <memory>

#include <vfsio/error.hpp>
#include <vfsio/ibase.hpp>
#include <vfsio/iblock.hpp>
#include <vfsio/iblock_seekable.hpp>

namespace vfsio
{
	namespace detail
	{
		struct cfile_close { 
			auto operator()(std::FILE* file_v) const -> void; 
		};
	}

	struct cfile_block final: 
		public vfsio::Iblock_seekable
	{
		cfile_block(std::string_view path_v, std::string_view mode_v = "r+b");

	private:
    auto type() const -> device_type override;
    auto aligment () const -> std::size_t override;
		auto resize(std::size_t) -> bool override;

    auto read (std::span<std::byte> buffer_v, std::uintmax_t offset_v = 0) -> std::size_t override;
    auto write (std::span<const std::byte> buffer_v, std::uintmax_t offset_v = 0) -> std::size_t override;
    auto size () const -> std::uintmax_t override;

    auto flush () -> bool override;   
    auto read (std::span<std::byte> buffer) -> std::size_t override;
    auto write (std::span<const std::byte> buffer) -> std::size_t override;
    auto seek (std::intmax_t offset_v, relative_to relative_to_v = relative_to::start) -> std::uintmax_t override;
    auto tell () const -> std::uintmax_t override;
		
		std::unique_ptr<FILE, detail::cfile_close> m_file;
	};
}