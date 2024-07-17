#pragma once

#include <cstddef>
#include <cstdint>
#include <string_view>
#include <span>
#include <memory>


namespace vfsio
{
  enum class error
  {
    none,
    not_implemented,
    not_supported,
    bad_cast,
    invalid_file,
    invalid_argument,
    invalid_operation,
		invalid_or_corrupted,
		unsupported_machine_type,
    name_too_long,
    not_found,
    io_error,
    access_denied,
    out_of_memory,
		internal_error,
    other
  };

  error from_errno(int error_v);
    
  enum class relative_to
  {
    beginning,
    current,
    ending
  };

  enum class node_type
  {
    undefined,
    stream,
    file,
    directory,
    directory_entry,
    volume
  };

  struct IStream;
  struct IFile;
  struct IDirectory;
  struct IDirectoryEntry;
  struct IVolume;

  struct INode
  {
    virtual ~INode() = default;   
    virtual auto type    (error& error_v) const -> node_type;   
    
    template <auto What_type>
    auto* as(error& error_v)
    {
      const auto type_v = type (error_v);
      if (error_v != error::none) 
        return nullptr;
      if (type_v != What_type) {        
        error_v = error::bad_cast;
        return nullptr;
      }
      
      if constexpr (What_type == node_type::stream)
        return (IStream*)this;
      else if constexpr (What_type == node_type::stream)
        return (IFile*)this;
      else if constexpr (What_type == node_type::directory)
        return (IDirectory*)this;
      else if constexpr (What_type == node_type::directory_entry)
        return (IDirectoryEntry*)this;
      else if constexpr (What_type == node_type::volume)
        return (IVolume*)this;
      else
        return this;
    }

    template <auto What_type>
    auto* as(error& error_v) const
    {
      const auto type_v = type (error_v);
      if (error_v != error::none) 
        return nullptr;
      if (type_v != What_type) {
        error_v = error::bad_cast;
        return nullptr;
      }
      
      if constexpr (What_type == node_type::stream)
        return (IStream const*)this;
      else if constexpr (What_type == node_type::stream)
        return (IFile const*)this;
      else if constexpr (What_type == node_type::directory)
        return (IDirectory const*)this;
      else if constexpr (What_type == node_type::directory_entry)
        return (IDirectoryEntry const*)this;
      else if constexpr (What_type == node_type::volume)
        return (IVolume const*)this;
      else
        return this;
    }
  };

  template <auto What_type>
  static inline auto node_cast(error& error_v, std::unique_ptr<INode>&& node_p)
    -> std::unique_ptr<decltype(*node_p->as<What_type>())>
  {
    const auto type_v = node_p->type(error_v);
    if (type_v != What_type)
      return nullptr;   
    return node_p.release()->as<What_type>();
  }

  template <auto What_type>
  static inline auto node_cast(error& error_v, std::unique_ptr<INode const>&& node_p)
    -> std::unique_ptr<decltype(*node_p->as<What_type>())>
  {
    const auto type_v = node_p->type(error_v);
    if (type_v != What_type)
      return nullptr;   
    return node_p.release()->as<What_type>();
  }
    
  struct IStream: 
    public INode
  {   
    virtual auto type    (error& error_v) const -> node_type override;
    virtual auto read    (error& error_v, std::span<std::byte> buffer_v) -> std::span<std::byte>;
    virtual auto write   (error& error_v, std::span<std::byte const> buffer_v) -> std::span<std::byte const>;
    virtual auto flush   (error& error_v) -> bool;
  };

  struct IFile :
    public IStream
  {
    virtual auto read    (error& error_v, std::span<std::byte> buffer_v, std::uintmax_t offset_v) -> std::span<std::byte>;
    virtual auto write   (error& error_v, std::span<std::byte const> buffer_v, std::uintmax_t offset_v) -> std::span<std::byte const>;
    using IStream::read;
    using IStream::write;
    
    virtual auto type    (error& error_v) const -> node_type override;
    virtual auto seek    (error& error_v, std::intmax_t offset_v, relative_to origin_v) -> std::uintmax_t;
    virtual auto tell    (error& error_v) const -> std::uintmax_t;
    virtual auto size    (error& error_v) const -> std::uintmax_t;
    virtual auto resize  (error& error_v, std::uintmax_t new_size_v) -> std::uintmax_t;
  };

  namespace helper
  {
    struct IFile :
      public ::vfsio::IFile
    {
      using vfsio::IFile::read;
      using vfsio::IFile::write;
      
      virtual auto read  (error& error_v, std::span<std::byte> buffer_v) -> std::span<std::byte> override;
      virtual auto write (error& error_v, std::span<std::byte const> buffer_v) -> std::span<std::byte const> override;
      virtual auto seek  (error& error_v, std::intmax_t offset_v, relative_to origin_v) -> std::uintmax_t override;
      virtual auto tell  (error& error_v) const -> std::uintmax_t override;

    private:
      std::uintmax_t m_offset { 0 };
    };
  }


  struct IDirectoryEntry:
    public INode
  {
    virtual auto type    (error& error_v) const -> node_type override;
    virtual auto name    (error& error_v) const -> std::string_view;
    virtual auto next    (error& error_v) const -> IDirectoryEntry const*;
    virtual auto open    (error& error_v) const -> INode*;
  };
  
  struct IDirectory:
    public INode
  {
    virtual auto type    (error& error_v) const -> node_type override;
    virtual auto exists  (error& error_v, std::string_view name_v) const -> bool;
    virtual auto open    (error& error_v, std::string_view name_v, std::uint32_t mode_v) const -> INode*;
    virtual auto create  (error& error_v, std::string_view name_v, std::uint32_t mode_v) -> INode*;
    virtual auto remove  (error& error_v, std::string_view name_v, bool recursive_v = false) -> bool;
    virtual auto rename  (error& error_v, std::string_view old_name_v, std::string_view new_name_v) -> bool;
    virtual auto front   (error& error_v) const -> IDirectoryEntry const*;
    virtual auto back    (error& error_v) const -> IDirectoryEntry const*;
  };
    
  struct IVolume:
    public INode
  {
    virtual auto type    (error& error_v) const -> node_type override;
    virtual auto root    (error& error_v) -> IDirectory*;   
  };
  
}
