#ifndef __FLATREAL_H__
#define __FLATREAL_H__


#include "types.h"
#include "x86assembly.h"

void __cdecl FLAT_init();
void __cdecl FLAT_copy1(uint32_t dst, uint32_t src, uint32_t len);
void __cdecl FLAT_copy2(uint32_t dst, uint32_t src, uint32_t len);
void __cdecl FLAT_copy4(uint32_t dst, uint32_t src, uint32_t len);

inline 
uint32_t __cdecl FLAT_fartolinear(const void _far* ptr)
{
  return ((((uint32_t)ptr) >> 16) << 4) + (((uint32_t)ptr) & 0xffff);
}

inline 
uint32_t __cdecl FLAT_neartolinear(const void* ptr)
{
  return (((uint32_t)ptr) & 0xffff) + ((uint32_t)(x86_ds()) << 4);
}


#endif

