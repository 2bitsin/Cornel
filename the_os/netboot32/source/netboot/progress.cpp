#include <netboot/progress.hpp>
#include <textio/logger.hpp>
#include <textio/format/helpers/repeat_value.hpp>

declare_module(Netboot);

bool progress_notify::initialize (std::string_view& name, ::pxenv::tftp::params&)
{
  m_file_name = name;
  return true;
}

bool progress_notify::update_sizes (std::size_t total_size, std::size_t)
{
  m_final_size = total_size;
  return true;
}

bool progress_notify::progress (std::span<const std::byte> buffer, std::size_t, std::size_t offset)
{
  using namespace textio::fmt;
  using namespace textio::fmt::helpers;
  offset += buffer.size();
  const auto length = offset*25/m_final_size;
  Gmod.info<"{:s} [{:_<25}] {:>3d}%\r", false> (m_file_name, repeat_value(length, char('X')), length*4);
  return true;
}

bool progress_notify::finalize (std::size_t)
{
  using namespace textio::fmt;
  using namespace textio::fmt::helpers;
  Gmod.info<"{:s} [{:_<25}] {:>3d}%"> (m_file_name, repeat_value(25, char('X')), 100);
  return true;
}

auto progress_notify::failure (::pxenv::pxenv_status status) 
  -> ::pxenv::pxenv_status
{
  using namespace textio::fmt;
  Gmod.error<"\nDownload of {:s} failed with status {:x}"> (m_file_name, (std::uint16_t)status);
  return status;
}