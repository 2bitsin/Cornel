#pragma once

#include <vfsio/iblock.hpp>
#include <type_traits>

namespace vfsio
{

	template <typename What>
	requires (std::is_trivially_copyable_v<std::remove_cvref_t<What>>)
	static inline auto block_write(Iblock& block, What const& what) -> bool
	{
		return sizeof(What) == block.write({(std::byte*)&what, sizeof(what)});
	}

	template <typename What, auto Count>
	requires (std::is_trivially_copyable_v<std::remove_cvref_t<What>>)
	static inline auto block_read(Iblock& block, What const (&what)[Count]) -> bool
	{
		return sizeof(What) == block.read({(std::byte*)&what, sizeof(what)});
	}
	
}