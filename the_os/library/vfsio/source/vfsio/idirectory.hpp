#pragma once

#include <vfsio/Iblock.hpp>

namespace vfsio
{
	struct Idirectory: public Iblock
	{
    auto type() const -> device_type override;

	};
}