#include <vfsio/Ibase.hpp>

#include <utility>

using vfsio::Ibase;

auto Ibase::last_error() const -> error
{
  return m_last_error;
}

auto Ibase::set_error(error error_v) const -> error
{
  return std::exchange(m_last_error, error_v);
}

auto Ibase::clear_error() const -> error
{
  return set_error(error::success);
}