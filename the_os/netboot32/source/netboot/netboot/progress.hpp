#pragma once

#include <pxenv/tftp_notify.hpp>
#include <textio/format.hpp>

struct progress_notify: ::pxenv::tftp::Inotify
{
  std::string_view m_file_name;
  std::size_t m_final_size;
  progress_notify() = default;
  bool initialize (std::string_view& name, ::pxenv::tftp::params&) override;
  bool update_sizes (std::size_t total_size, std::size_t) override;
  bool progress (std::span<const std::byte> buffer, std::size_t, std::size_t offset) override;
  bool finalize (std::size_t) override;
  auto failure (::pxenv::pxenv_status status) -> ::pxenv::pxenv_status override;
};

