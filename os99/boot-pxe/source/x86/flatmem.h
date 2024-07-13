#pragma once 

#include <stdint.h>
#include <stddef.h>

#pragma pack(__push, 1)
  union pointer_helper
  {
    uint32_t d;
    struct 
    { 
      uint16_t lo;
      uint16_t hi; 
    } w;
    void const far* fpc;
    void far* fpm;
  };
#pragma pack(__pop)

static inline uint32_t to_linear(void const far* address) 
{
  pointer_helper p;
  p.fpc = address;
  return (((uint32_t)p.w.hi) << 4u) + p.w.lo;
}

static inline void far* from_linear(uint32_t linear)  
{  
  pointer_helper p;
  p.w.hi = linear >> 4u;
  p.w.lo = linear & 0xfu;
  return p.fpm;  
}

void __cdecl flat_fill(uint32_t target_address, uint8_t fill_with, uint32_t bytes_to_fill);
void __cdecl flat_fill_w(uint32_t target_address, uint16_t fill_with, uint32_t words_to_fill);
void __cdecl flat_fill_d(uint32_t target_address, uint32_t fill_with, uint32_t dwords_to_fill);

void __cdecl flat_copy(uint32_t target_address, uint32_t source_address, uint32_t bytes_to_copy);




