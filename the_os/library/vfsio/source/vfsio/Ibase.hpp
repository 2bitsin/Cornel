#pragma once

#include <cstddef>
#include <cstdint>

#include <vfsio/error.hpp>

namespace vfsio
{
  struct Ibase
  {
    virtual ~Ibase() = default;
    virtual auto last_error() const -> error;
    virtual auto set_error(error error_v) const -> error;
    virtual auto clear_error() const -> error;
	private:
		mutable error m_last_error{ error::success };
  };
}