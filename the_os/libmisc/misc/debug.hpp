#pragma once

#include "macros.hpp"

CO_INLINE
static inline auto __debugbreak() 
{
  __asm__ __volatile__ ("xchgw %bx, %bx");
}

#define co_assert(X) if(!(X)) { __debugbreak(); }


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