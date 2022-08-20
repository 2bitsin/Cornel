#ifndef __FLATREAL_H__
#define __FLATREAL_H__


#include "types.h"
#include "x86asm.h"

void __cdecl FLAT_init();

void __cdecl FLAT_copy1(uint32_t dst, uint32_t src, uint32_t len);
void __cdecl FLAT_copy2(uint32_t dst, uint32_t src, uint32_t len);
void __cdecl FLAT_copy4(uint32_t dst, uint32_t src, uint32_t len);

void __cdecl FLAT_fill1(uint32_t dst, uint32_t len, uint8_t  val);
void __cdecl FLAT_fill2(uint32_t dst, uint32_t len, uint16_t val);
void __cdecl FLAT_fill4(uint32_t dst, uint32_t len, uint32_t val);

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

