#pragma once

#include <vfsio/iblock.hpp>

namespace vfsio
{
	struct Iblock_seekable : public Iblock
	{		
    auto seek (std::intmax_t offset_v, relative_to relative_to_v = relative_to::start) -> std::uintmax_t;
    auto tell () const -> std::uintmax_t;
  private:
    std::uintmax_t m_offset { 0u };
	};
}