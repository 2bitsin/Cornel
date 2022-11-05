#include <vfsio/iblock_seekable.hpp>

using vfsio::Iblock_seekable;

auto Iblock_seekable::seek (std::intmax_t offset_v, relative_to relative_to_v) -> std::uintmax_t
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
		{
			const auto size_v = size();
			if (last_error () == error::success)
				m_offset = size_v - offset_v;				
			break;
		}
  }
  return m_offset;
}

auto Iblock_seekable::tell () const -> std::uintmax_t
{
  clear_error();
  return m_offset;
}

