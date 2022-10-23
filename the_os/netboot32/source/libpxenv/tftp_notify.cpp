#include <pxenv/tftp_notify.hpp>

using pxenv::tftp::Inotify;

bool Inotify::initialize (std::string_view&, params&)
{
  return true;
}

bool Inotify::update_sizes (std::size_t, std::size_t)
{
  return true;
}

bool Inotify::progress (std::span<const std::byte>, std::size_t, std::size_t)
{
  return true;
}

bool Inotify::finalize (std::size_t)
{
  return true;
}

auto Inotify::null() -> Inotify&
{
  static Inotify null_instance;
  return null_instance;  
}
