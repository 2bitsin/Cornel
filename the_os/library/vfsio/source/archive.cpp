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
  
  offset_v = align (offset_v + header_v.size);  
  m_block->seek(error_v, offset_v, relative_to::beginning);
  if (error_v != error::none)
    return false;

  m_stack.pop();
  return true;
}

auto vfsio::archive::load(error& error_v, std::string_view path_v, memory::buffer<std::byte>& buffer_v) const -> bool
{
  header_type header_v;
  std::string_view pcomp_v; 
  std::uintmax_t offset_v { 0 };
  std::uintmax_t ending_v { 0 };
  auto header_bytes_v = utils::as_writable_bytes(header_v); 

  error_v = vfsio::error::none;
  ending_v = m_block->size(error_v);
  if (error_v != vfsio::error::none) {
    return false;
  }
  
  while(!path_v.empty())
  {
    auto it = path_v.find('/');
    if (it == std::string_view::npos) {
      pcomp_v = std::exchange(path_v, std::string_view());
    }
    else {
      pcomp_v = path_v.substr(0, it);
      path_v = path_v.substr(it + 1);
    }
    
    if (pcomp_v.empty()) {
      break;
    }
    
    while(true) 
    {
      const auto read_bytes_v = m_block->read(error_v, header_bytes_v, offset_v);
      if (error_v != vfsio::error::none || 
        read_bytes_v.size() <= offsetof(header_type, name) ||
        read_bytes_v.size() < header_size(header_v))         
      {
        return {};
      }
          
      std::string_view curr_name_v (header_v.name, header_v.name_len);
      curr_name_v = curr_name_v.substr(0, curr_name_v.find_last_not_of('\0') + 1);
      if (pcomp_v == curr_name_v) 
      {
        if (header_v.type_id == type_id_enum::directory) 
        {
          ending_v = offset_v + header_v.size;
          offset_v += header_size(header_v);
          offset_v = align(offset_v);
        }
        break;
      }
      
      offset_v += header_v.size;    
      offset_v = align(offset_v);
      if (offset_v >= ending_v) {
        error_v = vfsio::error::not_found;
        return false;
      }
    }   
    
    if (path_v.empty()) {
      const auto data_size_v = header_v.size - header_size(header_v);
      offset_v += header_size(header_v);
      if (buffer_v.size() != data_size_v) {
        buffer_v.resize(data_size_v);
      }
      const auto bytes_read_v = m_block->read(error_v, buffer_v, offset_v);
      if (error_v != vfsio::error::none) {
        return false;
      }
      if (bytes_read_v.size() != buffer_v.size()) {
        error_v = vfsio::error::io_error;
        return false;
      }
      return true;
    }   
  }
  error_v = vfsio::error::internal_error; 
  return false;
}

auto vfsio::archive::resize(error& error_v, std::uintmax_t new_size_v) -> std::uintmax_t
{
  error_v = vfsio::error::none;
  if (m_stack.empty()) {
    error_v = vfsio::error::invalid_operation;
    return 0;
  }
  
  auto [offset_v, header_v] = m_stack.top();
  
  if (header_v.type_id == type_id_enum::directory) {
    error_v = vfsio::error::invalid_operation;
    return 0;
  }

  offset_v += header_size(header_v);

  auto const curr_offset_v = m_block->tell(error_v);
  
  if (error_v != vfsio::error::none) 
    return 0;
  
  auto const curr_size_v = curr_offset_v - offset_v;
  
  if (new_size_v == curr_size_v)
    return new_size_v;
    
  if (new_size_v < curr_size_v) 
  {
    m_block->seek(error_v, offset_v + new_size_v, relative_to::beginning);
    if (error_v != vfsio::error::none)
      return 0;   
  }

  auto const actual_size_v = m_block->resize(error_v, offset_v + new_size_v);
  if (error_v != vfsio::error::none)
    return 0;
  
  return actual_size_v - offset_v;  
}

auto vfsio::archive::flush(error& error_v) -> bool
{
  error_v = error::none;
  return m_block->flush(error_v);
}

auto vfsio::archive::write(error& error_v, std::span<std::byte const> data_v) -> std::span<std::byte const>
{
  return helper::IFile::write(error_v, data_v);
}

auto vfsio::archive::read(error& error_v, std::span<std::byte> data_v) -> std::span<std::byte>
{
  return helper::IFile::read(error_v, data_v);
}
