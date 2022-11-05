#include <vfsio/iblock.hpp>

using vfsio::Iblock;


auto Iblock::type() const -> device_type 
{ 
	set_error(error::not_implemented);
	return device_type::undefined;
};

auto Iblock::aligment () const -> std::size_t 
{
	clear_error();
	return 1u;
};

auto Iblock::resize(std::size_t) -> bool
{
	set_error(error::not_implemented);
	return false;
}

auto Iblock::read(std::span<std::byte> buffer_v, std::uintmax_t offset_v) -> std::size_t
{
	set_error(error::not_implemented);
	return 0u;
}

auto Iblock::write(std::span<const std::byte> buffer_v, std::uintmax_t offset_v) -> std::size_t
{
	set_error(error::not_implemented);
	return 0u;
}

auto Iblock::read (std::span<std::byte> buffer) -> std::size_t
{	
	const auto size_v = read(buffer, tell());
	seek(size_v, relative_to::current);
	return size_v;
}

auto Iblock::write (std::span<const std::byte> buffer) -> std::size_t
{
	using enum device_type;
	using enum relative_to;
	const auto size_v = write(buffer, tell());
	seek(type() != directory ? std::min<std::intmax_t>(size_v, 1u) : 1u, current);
	return size_v;
}

auto Iblock::flush () -> bool
{
	clear_error();
	return true;
}

auto Iblock::seek (std::intmax_t offset_v, relative_to relative_to_v) -> std::uintmax_t 
{
	set_error(error::not_implemented);
	return 0u;
}

auto Iblock::tell () const -> std::uintmax_t 
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
