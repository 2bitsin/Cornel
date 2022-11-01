#pragma once

#include <cstddef>
#include <cstdint>
#include <span>

#include <vfsio/Ibase.hpp>

namespace vfsio
{
  enum class relative_to
  {
    start,
    current,
    end
  };


	enum class device_type
	{
		undefined,
		block,
		stream,
		directory
	};

  struct Iblock: public Ibase
  {    
		virtual auto type() const -> device_type;
		virtual auto sector_size () const -> std::size_t;

    virtual auto read (std::span<std::byte> buffer_v, std::uintmax_t offset_v = 0) -> std::size_t;
    virtual auto write (std::span<const std::byte> buffer_v, std::uintmax_t offset_v = 0) -> std::size_t;
    virtual auto size () -> std::uintmax_t;
		virtual auto flush () -> void;
		
    virtual auto read (std::span<std::byte> buffer) -> std::size_t;
    virtual auto write (std::span<const std::byte> buffer) -> std::size_t;
    virtual auto seek (std::uintmax_t offset_v, relative_to relative_to_v = relative_to::start) -> std::uintmax_t;
    virtual auto tell () -> std::uintmax_t;
  private:
    std::uintmax_t m_offset { 0u };
  };
}