#pragma once

inline auto __debugbreak() 
{
  __asm__ __volatile__ ("xchgw %bx, %bx");
}

#define co_assert(X)