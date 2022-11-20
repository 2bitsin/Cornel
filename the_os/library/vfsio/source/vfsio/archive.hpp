#pragma once

#include "vfsio.hpp"

#include <memory/buffer.hpp>

#include <stack>
#include <type_traits>

namespace vfsio
{
  struct archive final: vfsio::helper::IFile
  {
    enum type_id_enum: std::uint8_t
    {
      directory = 0x00,
      file      = 0x01
    };
  protected:
  #pragma pack(push, 1)
    struct header_type
    {
      std::uint32_t size;
      type_id_enum type_id;
      std::uint8_t name_len;
      char name[250];
    };

    static constexpr inline auto offset_of_name = offsetof(header_type, name);
  #pragma pack(pop) 
  public:
    
    archive(error& error_v, ::vfsio::IFile* file_v);
   ~archive();

    auto open  (error& error_v, type_id_enum type_v, std::string_view name_v) -> bool;
    auto close (error& error_v) -> bool;    
    
    auto write (error& error_v, std::span<std::byte const> data_v) -> std::span<std::byte const> override ;
    auto read  (error& error_v, std::span<std::byte> data_v) -> std::span<std::byte> override ;
    auto write (error& error_v, std::span<std::byte const> buffer_v, std::uintmax_t offset_v) -> std::span<std::byte const> override;
    auto read  (error& error_v, std::span<std::byte> buffer_v, std::uintmax_t offset_v) -> std::span<std::byte> override;
    auto size  (error& error_v) const -> std::uintmax_t override;

    auto load  (error& error_v, std::string_view path_v, memory::buffer<std::byte>& buffer_v) const -> bool;

  protected:
    static auto header_size(header_type const& header_v) -> std::size_t;
    static auto as_bytes(header_type const& header_v) -> std::span<std::byte const>;
    
    template <typename T>
    static auto align(T&& value_v) -> std::remove_reference_t<T>
    {
      using Q = std::remove_reference_t<decltype(value_v)>;
      return (value_v + Q(0xfu)) & ~Q(0xfu);      
    }

    template <typename T>
    static auto is_aligned(T&& value_v) -> bool
    {
      using Q = std::remove_reference_t<T>;
      return (value_v & Q(0xfu)) == Q(0);
    }

  private:
    
    vfsio::IFile* m_block;
    template<typename...T> using tuple = std::tuple<T...>;
    template<typename...T> using stack = std::stack<T...>;
    stack<tuple<uintmax_t, header_type>> m_stack;
  };
  
}