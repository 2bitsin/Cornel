#pragma once

#include <span>
#include <hardware/x86asm.hpp>
#include <hardware/x86arch.hpp>
#include <misc/macros.hpp>

namespace x86arch
{

#ifdef __cpp_lib_span
  template <typename T>
  requires (sizeof (T) == 8)
  CO_INLINE
  static inline void lgdt (std::span<T> table)
  {
    const auto limit = table.size()*sizeof(T);
    co_assert(limit < 0x10000u);   
    lgdt (Xdtr_t{ (std::uint16_t)limit, table.data() });
  }

  template <typename T>
  requires (sizeof (T) == 8)
  CO_INLINE
  static inline void lidt (std::span<T> table)
  {
    const auto limit = table.size()*sizeof(T);
    co_assert(limit < 0x10000u);   
    lidt (Xdtr_t{ (std::uint16_t)limit, table.data() });
  }

  /// -----------------------------------------------------------------------

  template <typename T = std::uint64_t>
  CO_INLINE
  static inline auto sgdt () -> std::span<T>
  {
    Xdtr_t tr;
    sgdt (tr);
    return std::span<T> { (T*)tr.base, tr.limit / sizeof(T) };
  }

  template <typename T = std::uint64_t>
  CO_INLINE
  static inline auto sidt () -> std::span<T>
  {
    Xdtr_t tr;
    sidt (tr);
    return std::span<T> { (T*)tr.base, tr.limit / sizeof(T) };
  }

#endif // __cpp_lib_span

  enum class segment_type
  {
    code,
    data,
    stack    
  };

  struct gta_make32_type  
  {
    segment_type  type        { segment_type::data };
    std::uint32_t base        { 0x00000000u };
    std::uint32_t size        { 0xffffffffu };    
    std::uint8_t  priv_level  { 0x00u };
    bool          present     { true };    
  };

  auto gdt_resize(std::uint16_t new_size, std::uint64_t defval = 0u, std::uint32_t flags = 0u) -> std::span<std::uint64_t>;
  auto gdt_make32(gta_make32_type params) -> std::uint64_t;  
  auto gdt_size() -> std::uint16_t;
  auto gdt_set(std::uint16_t index, std::uint64_t descriptor) -> std::uint64_t;
  auto gdt_get(std::uint16_t index) -> std::uint64_t;
  auto gdt_selector(std::uint16_t index, std::uint8_t rpl) -> std::uint16_t;

}
