#include <vfsio/cfile_block.hpp>

using vfsio::cfile_block;
using vfsio::detail::cfile_close;

// TODO: Implement 64bit file size/seek

auto cfile_close::operator()(std::FILE* file_v) const -> void
{
  if (nullptr != file_v)
  {
    std::fclose(file_v);
  }
}

cfile_block::cfile_block(std::string_view path_v, std::string_view mode_v)
: m_file { std::fopen (path_v.data(), mode_v.data()) }
{ 
  clear_error();
  if (nullptr == m_file)
  {
    set_error(from_errno(errno));
  }
}

auto cfile_block::size() const -> std::uintmax_t
{
  clear_error();
  if (nullptr == m_file) {
    set_error(error::bad_file_handle);
    return 0;
  }

  auto const position_save_v = std::ftell(m_file.get());
  if (-1L == position_save_v) {
    set_error(from_errno(errno));
    return 0u;
  }

  if (0L != std::fseek(m_file.get(), 0, SEEK_END)) {
    set_error(from_errno(errno));
    return 0u;
  }

  auto const file_size_v = std::ftell(m_file.get());
  if (-1L == file_size_v) {
    set_error(from_errno(errno));
    return 0;
  }

  if (0L != std::fseek(m_file.get(), position_save_v, SEEK_SET)) {
    set_error(from_errno(errno));
    return 0;
  }

  return file_size_v;
}

auto cfile_block::resize(std::size_t size_v) -> bool
{
  clear_error();
  if (nullptr == m_file) {
    set_error(error::bad_file_handle);
    return false;
  }
  
  if (size_v < size()) {
    set_error(error::not_supported);
    return false;
  }

  size_v -= sizeof(unsigned char);
  if (0u != std::fseek(m_file.get(), size_v, SEEK_SET)) {
    set_error(from_errno(errno));
    return false;
  }

  if (0u != std::fputc(0, m_file.get())) {
    set_error(from_errno(errno));
    return false;
  }

  return true;
}

auto cfile_block::read(std::span<std::byte> buffer_v, std::uintmax_t offset_v) -> std::size_t
{
  clear_error();
  if (nullptr == m_file) {
    set_error(error::bad_file_handle);
    return 0u;
  }
  
  auto const position_save_v = std::ftell(m_file.get());
  if (-1L == position_save_v) {
    set_error(from_errno(errno));
    return 0u;
  }

  if (0u != std::fseek(m_file.get(), offset_v, SEEK_SET)) {
    set_error(from_errno(errno));
    return 0u;
  }

	std::size_t bytes_read_v = 0u;
	while (bytes_read_v < buffer_v.size())
	{
		const auto result_v = std::fread(buffer_v.data() + bytes_read_v, sizeof(std::byte), buffer_v.size() - bytes_read_v, m_file.get());
		if (0u == result_v) {
		  set_error(from_errno(errno));
		  return bytes_read_v;
		}
		bytes_read_v += result_v;
	}

  if (0u != std::fseek(m_file.get(), position_save_v, SEEK_SET)) {
    set_error(from_errno(errno));   
  }

  return bytes_read_v;
}

auto cfile_block::write(std::span<const std::byte> buffer_v, std::uintmax_t offset_v) -> std::size_t
{
  clear_error();
  if (nullptr == m_file) {
    set_error(error::bad_file_handle);
    return 0u;
  }

  auto const position_save_v = std::ftell(m_file.get());
  if (-1L == position_save_v) {
    set_error(from_errno(errno));
    return 0u;
  }

  if (0u != std::fseek(m_file.get(), offset_v, SEEK_SET)) {
    set_error(from_errno(errno));
    return 0u;
  }

  auto const write_count_v = std::fwrite(buffer_v.data(), sizeof(std::byte), buffer_v.size(), m_file.get());
  if (0u == write_count_v) {
    set_error(from_errno(errno));
    return 0u;
  }

  if (0u != std::fseek(m_file.get(), position_save_v, SEEK_SET)) {
    set_error(from_errno(errno));
  }

  return write_count_v;
}

auto cfile_block::flush() -> bool
{
  clear_error();
  if (nullptr == m_file) {
    set_error(error::bad_file_handle);
    return false;
  }

  if (0L != std::fflush(m_file.get())) {
    set_error(from_errno(errno));
    return false;
  }

  return true;
}

auto cfile_block::read(std::span<std::byte> buffer_v) -> std::size_t
{
  clear_error();
  if (nullptr == m_file) {
    set_error(error::bad_file_handle);
    return 0u;
  }

  auto const read_count_v = std::fread(buffer_v.data(), sizeof(std::byte), buffer_v.size(), m_file.get());
  if (0u == read_count_v) {
    set_error(from_errno(errno));
    return 0u;
  }

  return read_count_v;
}

auto cfile_block::write(std::span<const std::byte> buffer_v) -> std::size_t
{
  clear_error();
  if (nullptr == m_file) {
    set_error(error::bad_file_handle);
    return 0u;
  }

  auto const write_count_v = std::fwrite(buffer_v.data(), sizeof(std::byte), buffer_v.size(), m_file.get());
  if (0u == write_count_v) {
    set_error(from_errno(errno));
    return 0u;
  }

  return write_count_v;
}

auto cfile_block::seek(std::uintmax_t offset_v, relative_to relative_to_v) -> std::uintmax_t
{
  clear_error();
  if (nullptr == m_file) {
    set_error(error::bad_file_handle);
    return 0u;
  }

  auto const whence_v = [&relative_to_v]() -> int {
    switch (relative_to_v) {
    case relative_to::start:
      return SEEK_SET;
    case relative_to::current:
      return SEEK_CUR;
    case relative_to::end:
      return SEEK_END;
    default:
      return SEEK_SET;
    }
  }();

  auto const position_v = std::ftell(m_file.get());
  if (-1L == position_v) {
    set_error(from_errno(errno));
    return 0u;
  }
  
  if (0u != std::fseek(m_file.get(), offset_v, whence_v)) {
    set_error(from_errno(errno));
    return 0u;
  }

  return position_v;  
}

auto cfile_block::tell() const -> std::uintmax_t
{
  clear_error();
  if (nullptr == m_file) {
    set_error(error::bad_file_handle);
    return 0u;
  }

  auto const position_v = std::ftell(m_file.get());
  if (-1L == position_v) {
    set_error(from_errno(errno));
    return 0u;
  }

  return position_v;
}

auto cfile_block::type() const -> vfsio::device_type
{
  clear_error();
  return device_type::file;
}

auto cfile_block::aligment() const -> std::size_t
{
  clear_error();
  return 1u;
}