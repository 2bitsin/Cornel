#pragma once

#include <cstddef>
#include <cstdint>
#include <type_traits>

namespace io
{
  static inline void outb (std::uint16_t port, std::uint8_t value)
  { __asm__ __volatile__ ("outb %0, %1" : : "a" (value), "Nd" (port)); }

  static inline void outw (std::uint16_t port, std::uint16_t value)
  { __asm__ __volatile__ ("outw %0, %1" : : "a" (value), "Nd" (port)); }
 
  static inline void outl (std::uint16_t port, std::uint32_t  value)
  { __asm__ __volatile__ ("outl %0, %1" : : "a" (value), "Nd" (port)); }

  static inline auto inb (std::uint16_t port) -> std::uint8_t 
  {
    std::uint8_t value { 0 };
    __asm__ __volatile__ ("inb %1, %0" : "=a" (value) : "Nd" (port));
    return value;
  }

  static inline auto inw (std::uint16_t port) -> std::uint16_t 
  {
    std::uint16_t value { 0 };
    __asm__ __volatile__ ("inb %1, %0" : "=a" (value) : "Nd" (port));
    return value;
  }

  static inline auto inl (std::uint16_t port) -> std::uint32_t 
  {
    std::uint32_t value { 0 };
    __asm__ __volatile__ ("inb %1, %0" : "=a" (value) : "Nd" (port));
    return value;
  }

  template <typename T>
  requires (std::is_integral_v<T>)
  static inline void out (std::uint16_t port, T value)
  {
    if constexpr (std::is_same_v<T, std::uint8_t> || std::is_same_v<T, std::int8_t>)
      outb (port, (std::uint8_t)value);
    else if constexpr (std::is_same_v<T, std::uint16_t> || std::is_same_v<T, std::int16_t>)
      outw (port, (std::uint16_t)value);
    else if constexpr (std::is_same_v<T, std::uint32_t> || std::is_same_v<T, std::int32_t>)
      outl (port, (std::uint32_t)value);
    else static_assert (sizeof(T*) == 0, "Invalid type");
  }

  template <typename T>
  requires (std::is_integral_v<T>)
  static inline auto in (std::uint16_t port, T& value) -> void
  {
    if constexpr (std::is_same_v<T, std::uint8_t> || std::is_same_v<T, std::int8_t>)
      value = (T)inb(port);
    else if constexpr (std::is_same_v<T, std::uint16_t> || std::is_same_v<T, std::int16_t>)
      value = (T)inw(port);
    else if constexpr (std::is_same_v<T, std::uint32_t> || std::is_same_v<T, std::int32_t>)
      value = (T)inl(port);
    else static_assert(sizeof(T*) == 0, "Invalid type");       
  }

  template <typename T>
  requires (std::is_integral_v<T>)
  static inline auto in (std::uint16_t port) -> T
  {
    T value;
    in(port, value);
    return value;
  }
}

#pragma pack(push, 1)

struct Xdtr_t
{
  std::uint16_t limit;
  std::uint32_t base;
};

static inline void load_gdt (Xdtr_t const& tr)
{
  __asm__ __volatile__ ("lgdt %0" : : "m" (tr));
}

static inline void load_idt (Xdtr_t const& tr)
{
  __asm__ __volatile__ ("lidt %0" : : "m" (tr));
}

static inline void load_ldt (std::uint16_t tr)
{
  __asm__ __volatile__ ("ldtr %0" : : "Nd" (tr));
}


#pragma pack(pop)
