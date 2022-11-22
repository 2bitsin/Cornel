#pragma once

#include "vfsio.hpp"
#include "detail/archive.hpp"

#include <memory/buffer.hpp>

#include <stack>
#include <type_traits>

namespace vfsio
{
  struct archive final: vfsio::helper::IFile
  {
		using header_type = detail::archive_entry_header;
		using type_id_enum = detail::archive_entry_type_enum;

		static inline auto constexpr file = type_id_enum::file;
		static inline auto constexpr directory = type_id_enum::directory;		

    static constexpr inline auto offset_of_name = offsetof(header_type, name);
  public:
    
    archive(error& error_v, ::vfsio::IFile* file_v);
   ~archive();

    auto open   (error& error_v, type_id_enum type_v, std::string_view name_v) -> bool;
    auto close  (error& error_v) -> bool;    
    					  
    auto write  (error& error_v, std::span<std::byte const> data_v) -> std::span<std::byte const> override ;
    auto write  (error& error_v, std::span<std::byte const> buffer_v, std::uintmax_t offset_v) -> std::span<std::byte const> override;
							  
    auto read   (error& error_v, std::span<std::byte> data_v) -> std::span<std::byte> override ;
    auto read   (error& error_v, std::span<std::byte> buffer_v, std::uintmax_t offset_v) -> std::span<std::byte> override;
							  
    auto size   (error& error_v) const -> std::uintmax_t override;
		auto resize (error& error_v, std::uintmax_t new_size_v) -> std::uintmax_t override;
		auto flush	(error& error_v) -> bool override;

		
    auto load   (error& error_v, std::string_view path_v, memory::buffer<std::byte>& buffer_v) const -> bool;

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