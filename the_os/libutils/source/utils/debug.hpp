#pragma once

#include <utils/macros.hpp>
#include <cstdlib>

CO_INLINE
static inline auto __debugbreak() 
{
  __asm__ __volatile__ ("xchgw %bx, %bx");
}


CO_INLINE
static inline void __debug_char(char value)
{
  __asm__ __volatile__ ("outb %0, $0xe9" : : "a"(value));
}

CO_INLINE
static inline void __debug_print(char const* message)
{
  for (auto i = 0; message[i] != '\0'; ++i)
    __debug_char(message[i]);  
}

#define _debug_var(X, Fmt) #X "=", Fmt((X)) 


CO_INLINE
static inline auto __debug_assert_impl(bool condition, char const* message) 
{
  if (!condition) 
  {
    __debug_print("ASSERTION FAILED: ");
    __debug_print(message);
    __debug_char('\n');
    __debugbreak();
    std::abort();
  }
}

#define __debug_assert(X) __debug_assert_impl((X), #X)
