#include <vfsio/archive.hpp>
#include <utils/utils.hpp>

#include <cstring>

using vfsio::archive;

archive::archive(error& error_v, ::vfsio::IFile* file_v)
: m_block { file_v }  
{
  if (nullptr == m_block) {
    error_v = error::invalid_argument;
  } 
}

archive::~archive()
{
  
  if (nullptr != m_block)
  {
    error error_v { error::none };
    while(!m_stack.empty())
    {
      close(error_v);
      if (error_v != error::none)
        break;
    }
    m_block->flush(error_v);
  }
}

auto vfsio::archive::open(error& error_v, type_id_enum type_v, std::string_view name_v) -> bool
{
  if (name_v.size() > 250) {
    error_v = error::name_too_long;
    return false;
  }

  if (m_block == nullptr) {
    error_v = error::invalid_file;
    return false;
  }

  if (!m_stack.empty())
  {
    auto const& [offset_v, header_v] = m_stack.top();
    if (header_v.type_id != type_id_enum::directory)
    {
      error_v = error::invalid_operation;
      return false;
    }     
  }

  header_type header_v;   
  std::memset(&header_v, 0, sizeof(header_v));
  header_v.name_len = align(offset_of_name + name_v.size()) - offset_of_name;
  header_v.type_id = type_v;
  std::copy(name_v.begin(), name_v.end(), std::begin(header_v.name));

  auto offset_v = m_block->tell(error_v);
  if (error_v != error::none) {
    return false;
  }
  
  if (!is_aligned(offset_v)) {
    offset_v = align(offset_v);
  }
  
  m_block->seek(error_v, offset_v, relative_to::beginning);
  if (error_v != error::none)
    return false;
  
  m_stack.push({offset_v, header_v}); 
  const auto result_v = m_block->write(error_v, as_bytes(header_v));
  if (!result_v.empty() || error_v != error::none)
    return false;
  
  seek(error_v, 0, relative_to::beginning);
  if (error_v != error::none)
    return false;

  return true;
}


auto vfsio::archive::write(error& error_v, std::span<std::byte const> buffer_v, std::uintmax_t offset_v) -> std::span<std::byte const>
{
  if (m_stack.empty()) 
  {
    error_v = error::invalid_operation;
    return buffer_v;
  }
  
  auto const& [header_offset_v, header_v] = m_stack.top();
  std::uintmax_t absolute_v = header_offset_v + header_size(header_v) + offset_v;
  return m_block->write(error_v, buffer_v, absolute_v); 
}

auto vfsio::archive::read(error& error_v, std::span<std::byte> buffer_v, std::uintmax_t offset_v) -> std::span<std::byte>
{
  error_v = error::not_implemented;
  return buffer_v.subspan(buffer_v.size());
}

auto vfsio::archive::size(error& error_v) const -> std::uintmax_t
{
  error_v = error::none;
  if (m_stack.empty ()) {
    error_v = error::invalid_operation;
  }
  auto const& [offset_v, header_v] = m_stack.top();
  
  auto const archive_size_v = m_block->size(error_v);
  if (error_v != error::none)
    return 0;
  return archive_size_v - offset_v;
}

auto vfsio::archive::header_size(header_type const& header_v) -> std::size_t
{
  return offset_of_name + header_v.name_len;
}

auto vfsio::archive::as_bytes(header_type const& header_v) -> std::span<std::byte const>
{
  return utils::as_bytes(header_v).subspan(0, header_size(header_v));
}

auto vfsio::archive::close(error& error_v) -> bool
{
  error_v = error::none;
  if (m_stack.empty()) {
    error_v = error::invalid_operation;
    return false;
  }

  auto& [offset_v, header_v] = m_stack.top(); 
  header_v.size = size(error_v);
  if (error_v != error::none)
    return false;
  const auto result_v = m_block->write(error_v, as_bytes(header_v), offset_v);
  if (!result_v.empty() || error_v != error::none)
    return false;
  
  m_block->seek(error_v, 0, relative_to::ending);
  if (error_v != error::none)
    return false;

  m_stack.pop();
  return true;
}

auto vfsio::archive::load(error& error_v, std::string_view path_v) const -> memory::buffer<std::byte>
{
  error_v = error::not_implemented;
  return {};
}

auto vfsio::archive::write(error& error_v, std::span<std::byte const> data_v) -> std::span<std::byte const>
{
  return helper::IFile::write(error_v, data_v);
}

auto vfsio::archive::read(error& error_v, std::span<std::byte> data_v) -> std::span<std::byte>
{
  return helper::IFile::read(error_v, data_v);
}
