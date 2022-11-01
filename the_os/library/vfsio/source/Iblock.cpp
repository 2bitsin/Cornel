#include <vfsio/Iblock.hpp>

using vfsio::Iblock;

auto Iblock::read (std::span<std::byte> buffer_v) -> std::size_t
{
  return read (buffer_v, m_offset);
}

auto Iblock::write (std::span<const std::byte> buffer_v) -> std::size_t
{
  return write (buffer_v, m_offset);
}

auto Iblock::seek (std::uintmax_t offset_v, relative_to relative_to_v) -> std::uintmax_t
{
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

auto Iblock::tell () -> std::uintmax_t
{
  return m_offset;
}

auto Iblock::flush() -> void
{
	set_error(error::not_implemented);	
}

auto Iblock::type() const -> vfsio::device_type
{
	set_error(error::not_implemented);
  return device_type::undefined;
}

auto Iblock::sector_size() const -> std::size_t
{
	set_error(error::not_implemented);
	return 0u;
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

auto Iblock::size() -> std::uintmax_t
{
	set_error(error::not_implemented);
	return 0u;
}
