#include <netboot/progress.hpp>

#include <textio/format.hpp>
#include <textio/format/helpers/repeat_value.hpp>

using netboot::progress_notify;

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
  const auto length = offset*50/m_final_size;
  format_to<"{:s} [{:_<50}] {:>3d}%\r"> (stdout, m_file_name, repeat_value(length, char('=')), length*2);
  return true;
}

bool progress_notify::finalize (std::size_t)
{
  using namespace textio::fmt;
  using namespace textio::fmt::helpers;
  format_to<"\r{:s} [{:_<50}] {:>3d}%\n"> (stdout, m_file_name, repeat_value(50, char('=')), 100);
  return true;
}
