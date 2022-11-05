#pragma once

#include <vfsio/ibase.hpp>
#include <vfsio/iblock.hpp>
#include <vfsio/iblock_seekable.hpp>

namespace vfsio
{
  struct buffer_block final
  : public Iblock_seekable
  {
    buffer_block(std::span<std::byte> block_bytes_v, bool read_only_v = false);

    auto read(std::span<std::byte> buffer_v, std::uintmax_t offset_v) -> std::size_t override;
    auto write(std::span<const std::byte> buffer_v, std::uintmax_t offset_v) -> std::size_t override;   
    auto size() const -> std::uintmax_t override;

  private:
    std::span<std::byte> m_block_bytes;   
    bool m_read_only = false;
  };
}
