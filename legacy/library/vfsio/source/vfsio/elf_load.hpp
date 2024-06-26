#pragma once

#include <cstddef>
#include <cstdint>
#include <span>
#include <functional>
#include <string_view>
#include <string>

#include <memory/block_list.hpp>
#include <vfsio/vfsio.hpp>

namespace vfsio::elf
{

	struct image
	{
		void operator () ();
		void* m_image_base { nullptr } ;
	};

	auto load(vfsio::error& error_v, memory::block_list& block_list_v, std::span<std::byte const> bytes_v) -> image;

}


