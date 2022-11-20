#pragma once

#include <vfsio/vfsio.hpp>
#include <textio/format.hpp>

template <typename Target, typename Context>
struct WPNotify:
  public vfsio::IFile
{
  using error = vfsio::error;
  using relative_to = vfsio::relative_to;
  using node_type = vfsio::node_type;

  WPNotify(vfsio::IFile& file_v, Target& target_v, Context context_v)
  : m_file    { file_v }
  , m_target  { target_v }
  , m_context { std::move(context_v) }
  {}

  auto type (error& error_v) const -> node_type override { return m_file.type(error_v); }
  auto flush (error& error_v) -> bool override { return m_file.flush(error_v); }
  auto seek (error& error_v, std::intmax_t offset_v, relative_to origin_v) -> std::uintmax_t override { return m_file.seek(error_v, offset_v, origin_v); }
  auto tell (error& error_v) const -> std::uintmax_t override { return m_file.tell(error_v); }
  auto size (error& error_v) const -> std::uintmax_t override { return m_file.size(error_v); }
  auto read (error& error_v, std::span<std::byte> buffer_v) -> std::span<std::byte> override { return m_file.read(error_v, buffer_v); }
  auto read (error& error_v, std::span<std::byte> buffer_v, std::uintmax_t offset_v) -> std::span<std::byte> override { return m_file.read(error_v, buffer_v, offset_v); }

  auto resize (error& error_v, std::uintmax_t new_size_v) -> std::uintmax_t override 
  { 
    const auto actual_size_v = m_file.resize(error_v, new_size_v); 
    m_target.notify_resize(m_context, error_v, new_size_v);
    return actual_size_v;
  }

  auto write (error& error_v, std::span<std::byte const> buffer_v) -> std::span<std::byte const> override 
  { 
    const auto offset_v = m_file.tell(error_v);    
    if (error_v != error::none)
      return buffer_v;
    const auto unwritten_bytes_v = m_file.write(error_v, buffer_v);
    m_target.notify_write(m_context, error_v, offset_v + buffer_v.size() - unwritten_bytes_v.size());
    return unwritten_bytes_v;
  }

  auto write (error& error_v, std::span<std::byte const> buffer_v, std::uintmax_t offset_v) -> std::span<std::byte const> override 
  { 
    const auto unwritten_bytes_v = m_file.write(error_v, buffer_v, offset_v);
    m_target.notify_write(m_context, error_v, offset_v + buffer_v.size() - unwritten_bytes_v.size());
    return unwritten_bytes_v;
  }

private:
  vfsio::IFile& m_file;
  Target& m_target;
  Context m_context;
};

