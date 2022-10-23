#pragma once

#include <cstddef>
#include <cstdint>
#include <span>
#include <string_view>

namespace pxenv::tftp
{
  struct params;

  struct Inotify
  {
    virtual ~Inotify() = default;    
    virtual bool initialize (std::string_view& name, params& par);
    virtual bool update_sizes (std::size_t byte_to_download, std::size_t packet_size);    
    virtual bool progress (std::span<const std::byte> buffer, std::size_t packet_number, std::size_t offset);
    virtual bool finalize (std::size_t bytes_downloaded);
    static auto null() -> Inotify&;
  protected:
    Inotify() = default;
  };

 
}