#pragma once

#include <vfsio/vfsio.hpp>

#include <memory_resource>
#include <limits>

namespace vfsio
{
	
	//////////////////////////////////
	// Write Once Memory File System
	//////////////////////////////////

	using allocator_type = std::pmr::memory_resource;

	auto WomfsCreate (error& error_v, std::size_t size_init_v = 0u, std::size_t size_limit_v = std::numeric_limits<std::size_t>::max(), allocator_type* allocator_v = nullptr) -> IVolume*;
}