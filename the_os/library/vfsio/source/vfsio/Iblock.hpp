#pragma once

#include <cstddef>
#include <cstdint>
#include <span>

#include <vfsio/ibase.hpp>

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
    block, // non resizeable block
		file, // a resizeable block
    stream,
    directory
  };

  struct Iblock: public Ibase
  {		
    virtual auto type() const -> device_type;
    virtual auto aligment () const -> std::size_t;
		virtual auto resize(std::size_t) -> bool;

    virtual auto load (std::span<std::byte> buffer_v, std::uintmax_t offset_v) -> std::size_t;
    virtual auto write (std::span<const std::byte> buffer_v, std::uintmax_t offset_v) -> std::size_t;
    virtual auto size () const -> std::uintmax_t;

    virtual auto flush () -> bool;    
    virtual auto load (std::span<std::byte> buffer) -> std::size_t;
    virtual auto write (std::span<const std::byte> buffer) -> std::size_t;
    virtual auto seek (std::uintmax_t offset_v, relative_to relative_to_v = relative_to::start) -> std::uintmax_t;
    virtual auto tell () const -> std::uintmax_t;
  private:
    std::uintmax_t m_offset { 0u };
  };
}