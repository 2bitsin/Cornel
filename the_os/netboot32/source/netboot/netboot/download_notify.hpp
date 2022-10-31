#pragma once

#include <pxenv/tftp_notify.hpp>
#include <textio/format.hpp>

template <typename Target>
struct DownloadNotify: 
  public ::pxenv::tftp::Inotify
{
  DownloadNotify(Target& target_v)
  : m_target(target_v) 
  {};

  bool initialize (std::string_view& file_name_v, ::pxenv::tftp::params& params_v) override
  {
    return m_target.download_initialize(file_name_v, params_v);
  }

  bool update_sizes (std::size_t total_size_v, std::size_t packet_size_v) override
  {
    return m_target.download_update_sizes(total_size_v, packet_size_v);  
  }

  bool progress (std::span<const std::byte> buffer_s, std::size_t packet_number_v, std::size_t offset_v) override
  {
    return m_target.download_progress(buffer_s, packet_number_v, offset_v);
  }

  bool finalize (std::size_t total_bytes_v) override
  {
    return m_target.download_finalize(total_bytes_v);
  }

  auto failure (::pxenv::pxenv_status status) -> ::pxenv::pxenv_status override
  {
    return m_target.download_failure(status);
  }

private:
  Target& m_target;
};

