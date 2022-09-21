#pragma once

#include <cstddef>
#include <cstdint>
#include <type_traits>

#include <utils/macros.hpp>
#include <utils/debug.hpp>

namespace x86arch
{
  CO_INLINE
  static inline void outb (std::uint16_t port, std::uint8_t value)
  { __asm__ volatile ("outb %0, %1" : : "a" (value), "Nd" (port)); }

  CO_INLINE
  static inline void outw (std::uint16_t port, std::uint16_t value)
  { __asm__ volatile ("outw %0, %1" : : "a" (value), "Nd" (port)); }
 
  CO_INLINE
  static inline void outl (std::uint16_t port, std::uint32_t  value)
  { __asm__ volatile ("outl %0, %1" : : "a" (value), "Nd" (port)); }

  CO_INLINE
  static inline auto inb (std::uint16_t port) -> std::uint8_t 
  {
    std::uint8_t value { 0 };
    __asm__ volatile ("inb %1, %0" : "=a" (value) : "Nd" (port));
    return value;
  }

  CO_INLINE
  static inline auto inw (std::uint16_t port) -> std::uint16_t 
  {
    std::uint16_t value { 0 };
    __asm__ volatile ("inb %1, %0" : "=a" (value) : "Nd" (port));
    return value;
  }

  CO_INLINE
  static inline auto inl (std::uint16_t port) -> std::uint32_t 
  {
    std::uint32_t value { 0 };
    __asm__ volatile ("inb %1, %0" : "=a" (value) : "Nd" (port));
    return value;
  }

  template <typename T>
  requires (std::is_integral_v<T>)
  CO_INLINE
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
  CO_INLINE 
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
  CO_INLINE
  static inline auto in (std::uint16_t port) -> T
  {
    T value;
    in(port, value);
    return value;
  }

  #pragma pack(push, 1)
  struct Xdtr_t
  {
    std::uint16_t limit;
    void* base;
  };
  #pragma pack(pop)

  CO_INLINE
  static inline void lgdt (Xdtr_t const& tr)
  {    
    __asm__ volatile ("lgdt %0" : : "m" (tr));
  }

  CO_INLINE
  static inline void lidt (Xdtr_t const& tr)
  {
    __asm__ volatile ("lidt %0" : : "m" (tr));
  }

  CO_INLINE
  static inline void lldt (uint16_t tr)
  {
    __asm__ volatile ("lldt %0" : : "rm" (tr));
  }
  
  /// -----------------------------------------------------------------------

  CO_INLINE
  static inline void sgdt (Xdtr_t& tr)
  {
    __asm__ volatile ("sgdt %0" : "=m" (tr));  
  }

  CO_INLINE
  static inline void sidt (Xdtr_t& tr)
  {
    __asm__ volatile ("sidt %0" : "=m" (tr));  
  }

  CO_INLINE
  static inline void sldt (uint16_t& tr)
  {
    __asm__ volatile ("sldt %0" : "=rm" (tr));  
  }

  /// -----------------------------------------------------------------------  

  CO_INLINE static inline void cli() { __asm__ volatile ("cli"); }
  CO_INLINE static inline void sti() { __asm__ volatile ("sti"); }
  CO_INLINE static inline void hlt() { __asm__ volatile ("hlt"); }
  CO_INLINE static inline void yield() { __asm__ volatile ("hlt"); }
  
  CO_INLINE static inline void store_flags(std::uint16_t& value) 
  {
    __asm__ volatile ("pushfw \n\t" "popw %0": "=rm" (value)); 
  }

  template <typename T> 
  CO_INLINE static inline void load_flags(std::uint16_t value) 
  {
    __asm__ volatile ("pushw %0 \n\t" "popfw" : : "rm" (value) : "cc");
  }
  
  CO_INLINE static inline void store_flags(std::uint32_t& value) 
  {
    __asm__ volatile ("pushfl \n\t" "popl %0": "=rm" (value)); 
  }

  template <typename T> 
  CO_INLINE static inline void load_flags(std::uint32_t value) 
  {
    __asm__ volatile ("pushl %0 \n\t" "popfl" : : "rm" (value) : "cc");
  }

  template <typename T = const void>
  CO_INLINE static inline auto stack_pointer() -> T*
  {
    T* value;
    __asm__ volatile ("movl %%esp, %0" : "=rm" (value));
    return value;
  }

}

