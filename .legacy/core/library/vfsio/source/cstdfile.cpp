#include <vfsio/cstdfile.hpp>

#include <cstdio>
#include <cstring>

using vfsio::cstdfile;

auto cstd_fopen(vfsio::error& error_v, std::string_view path_v, std::string_view mode_v) 
  -> cstdfile::cstdfile_ptr
{
  error_v = vfsio::error::none;
  std::string path_vv { path_v };
  std::string mode_vv { mode_v };
  cstdfile::cstdfile_ptr file_ptr_v { 
    std::fopen(path_vv.c_str(), mode_vv.c_str()) 
  };
  
  if (nullptr == file_ptr_v)
  {
    error_v = vfsio::from_errno(errno);
    return nullptr;
  }

  return file_ptr_v;
}

cstdfile::cstdfile(cstdfile_ptr file_v)
: m_file(std::move(file_v))
{}


cstdfile::cstdfile(error& error_v, std::string_view path_v, std::string_view mode_v)
: cstdfile(cstd_fopen(error_v, path_v, mode_v))
{}

cstdfile::~cstdfile()
{}

auto vfsio::cstdfile::write(error& error_v, std::span<std::byte const> buffer_v, std::uintmax_t offset_v) -> std::span<std::byte const> 
{ 
  const auto position_save_v = tell(error_v);
  if (error_v != vfsio::error::none)
    return buffer_v;
  
  seek(error_v, offset_v, vfsio::relative_to::beginning);
  if (error_v != vfsio::error::none) {
    seek(error_v, position_save_v, vfsio::relative_to::beginning);
    return buffer_v;
  }
  
  const auto written_v = write(error_v, buffer_v); 
  seek(error_v, position_save_v, vfsio::relative_to::beginning);
  return written_v;
}

auto vfsio::cstdfile::read(error& error_v, std::span<std::byte> buffer_v, std::uintmax_t offset_v) -> std::span<std::byte> 
{
  const auto position_save_v = tell(error_v);
  if (error_v != vfsio::error::none)
    return buffer_v;

  seek(error_v, offset_v, vfsio::relative_to::beginning);
  if (error_v != vfsio::error::none) {
    seek(error_v, position_save_v, vfsio::relative_to::beginning);
    return buffer_v;
  }

  const auto read_v = read(error_v, buffer_v);
  seek(error_v, position_save_v, vfsio::relative_to::beginning);
  return read_v;
}

auto vfsio::cstdfile::write(error& error_v, std::span<std::byte const> buffer_v) -> std::span<std::byte const> 
{ 
  error_v = error::none;
  if (const auto result_v = fwrite(buffer_v.data(), sizeof(std::byte), buffer_v.size(), m_file.get()); 0u != result_v) 
    return buffer_v.subspan(result_v);
  error_v = vfsio::from_errno(errno);   
  return buffer_v;
}

auto vfsio::cstdfile::read(error& error_v, std::span<std::byte> buffer_v) -> std::span<std::byte>
{
  error_v = error::none;
  if (const auto result_v = fread(buffer_v.data(), sizeof(std::byte), buffer_v.size(), m_file.get()); 0u != result_v) 
    return buffer_v.subspan(0, result_v);
  error_v = vfsio::from_errno(errno);
  return buffer_v.subspan(0, 0);
}

auto vfsio::cstdfile::seek(error& error_v, std::intmax_t offset_v, relative_to origin_v) -> std::uintmax_t
{
  error_v = error::none;  
  int origin_vv { SEEK_CUR }; 
  switch (origin_v)
  {
  case relative_to::beginning: origin_vv = SEEK_SET; break;
  case relative_to::current:   origin_vv = SEEK_CUR; break;
  case relative_to::ending:    origin_vv = SEEK_END; break;
  default:
    error_v = error::invalid_argument;
    return 0;
  }
    
  if (const auto result_v = std::fseek(m_file.get(), offset_v, origin_vv); result_v)
  {
    error_v = vfsio::from_errno(errno);
    return 0;
  }

  return cstdfile::tell(error_v);
}

auto vfsio::cstdfile::tell(error& error_v) const -> std::uintmax_t
{
  error_v = error::none;
  if (const auto result_v = std::ftell(m_file.get()); result_v >= 0L)
  {
    return result_v;
  }

  error_v = vfsio::from_errno(errno);
  return 0;
}

#ifdef _WIN32
  #include <io.h> 
#endif

#if (defined(__unix__) || defined(__unix) || (defined(__APPLE__) && defined(__MACH__)))
  #define _FILE_OFFSET_BITS 64  
  #include <unistd.h>
#endif

  #include <sys/stat.h>
  #include <sys/types.h>

auto vfsio::cstdfile::size(error& error_v) const -> std::uintmax_t
{ 
  error_v = error::none;
  
#ifdef _WIN32
  const auto fd_v = _fileno(m_file.get());
  struct _stat64 stat_v;
  std::memset(&stat_v,0,sizeof(stat_v));
  if (_fstat64(fd_v, &stat_v) < 0)
#elif (defined(__unix__) || defined(__unix) || (defined(__APPLE__) && defined(__MACH__)))
  const auto fd_v = fileno(m_file.get());
  struct stat stat_v;
  std::memset(&stat_v, 0, sizeof(stat_v));
  if (fstat(fd_v, &stat_v) < 0)
#endif  
  {
    error_v = vfsio::from_errno(errno);
    return 0;
  }
  return stat_v.st_size;
}

auto vfsio::cstdfile::resize(error& error_v, std::uintmax_t new_size_v) -> std::uintmax_t
{
  error_v = error::none;
  const auto size_v = cstdfile::size(error_v);
  if (error_v != error::none)
    return 0;
  if (new_size_v == size_v)
    return size_v;
#ifdef _WIN32
  if (const auto result_v = _chsize_s(_fileno(m_file.get()), new_size_v); result_v)
  {
    error_v = vfsio::from_errno(errno);
    return 0;
  }
  return new_size_v;
#endif
#if (defined(__unix__) || defined(__unix) || (defined(__APPLE__) && defined(__MACH__)))
  if (const auto result_v = ftruncate(fileno(m_file.get()), new_size_v); result_v)
  {
    error_v = vfsio::from_errno(errno);
    return 0;
  }
  return new_size_v;
#endif
}

auto vfsio::cstdfile::flush(error& error_v) -> bool
{
  if (std::fflush(m_file.get()))
  {
    error_v = vfsio::from_errno(errno);
    return false;
  }
  return true;
}
