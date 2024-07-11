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
  return (p.w.hi * 0x10ul) + p.w.lo;
}

static inline void far* from_linear(uint32_t linear)  
{  
  pointer_helper p;
  p.w.hi = linear >> 4u;
  p.w.lo = linear & 0xfu;
  return p.fpm;  
}

uint32_t flat_copy_fwd(uint32_t target_address, uint32_t source_address, uint32_t bytes_to_copy);

#pragma aux flat_copy   = \
  "push   ds            " \
  "xchg   ax,     di    " \
  "shl    eax,    16    " \
  "and    edi,    0xffff" \
  "add    edi,    eax   " \
  "xchg   bx,     si    " \
  "shl    ebx,    16    " \
  "and    esi,    0xffff" \
  "add    esi,    ebx   " \
  "xchg   dx,     cx    " \
  "shl    edx,    16    " \
  "and    ecx,    0xffff" \
  "add    ecx,    edx   " \
  "xor    ax,     ax    " \
  "mov    ds,     ax    " \
  "L0:                  " \
  "mov    al, ds:[esi]  " \
  "mov    ds:[edi], al  " \
  "inc    esi           " \
  "inc    edi           " \
  "dec    ecx           " \
  "jnz    L0            " \
  "pop    ds            " \
  parm caller [ax di] [bx si] [dx cx] \
  modify [ax bx cx dx si di];