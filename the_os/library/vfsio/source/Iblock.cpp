#include <vfsio/iblock.hpp>

using vfsio::Iblock;

auto Iblock::read (std::span<std::byte> buffer_v) -> std::size_t
{
  clear_error();
  return read (buffer_v, m_offset);
}

auto Iblock::write (std::span<const std::byte> buffer_v) -> std::size_t
{
  clear_error();
  return write (buffer_v, m_offset);
}

auto Iblock::seek (std::uintmax_t offset_v, relative_to relative_to_v) -> std::uintmax_t
{
  clear_error();
  switch (relative_to_v)
  {
  case relative_to::start:
    m_offset = offset_v;
    break;
  case relative_to::current:
    m_offset += offset_v;
    break;
  case relative_to::end:
    m_offset = size() - offset_v;
    break;
  }
  return m_offset;
}

auto Iblock::tell () const -> std::uintmax_t
{
  clear_error();
  return m_offset;
}

auto Iblock::flush() -> bool
{  
  clear_error();
  // No-op
  return true;
}

auto Iblock::type() const -> vfsio::device_type
{
  set_error(error::not_implemented);
  return device_type::undefined;
}

auto Iblock::aligment() const -> std::size_t
{ 
  clear_error();
  // Assume 1 byte aligment
  return 1u;
}


auto Iblock::read(std::span<std::byte>, std::uintmax_t) -> std::size_t
{
  set_error(error::not_implemented);
  return 0u;
}

auto Iblock::write(std::span<const std::byte>, std::uintmax_t) -> std::size_t
{
  set_error(error::not_implemented);
  return 0u;
}

auto Iblock::size() const -> std::uintmax_t
{
  switch (type ())
  {
  case device_type::stream:
    set_error(error::not_supported);
    return ~std::uintmax_t(0u);
  case device_type::undefined:
  case device_type::block:
  case device_type::directory:
  default:
    set_error(error::not_implemented);
    return 0u;
  }
  return 0u;
}

auto Iblock::resize(std::size_t) -> bool
{
	set_error(error::not_supported);
	return false;
}
