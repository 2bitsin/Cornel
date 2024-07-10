#pragma once

typedef void far* pfvoid_t;
typedef void const far* pfcvoid_t;

#pragma pack(__push, 1)

typedef struct 
{
  uint16_t di;
  uint16_t si;
  uint16_t bp; 
  uint16_t sp;

  union
  {
    struct
    {
      uint16_t bx;
      uint16_t dx; 
      uint16_t cx;
      uint16_t ax;
    };
    struct
    {
      uint8_t bl, bh;
      uint8_t dl, dh;
      uint8_t cl, ch;
      uint8_t al, ah;
    };
  };

  uint16_t gs;
  uint16_t fs;
  uint16_t es; 
  uint16_t ds; 
  uint16_t intnum; 
  uint16_t ip; 
  uint16_t cs;
  uint16_t flags;
} int_regs_t;

#pragma pack(__pop)

#include <dos

typedef void (__watcall far* dos_callback_type)(int_regs_t far*);

typedef void far* pfvoid_t;

inline bool set_dos_callback(uint8_t function, 
  dos_callback_type callback) 
{  
  if(function >= 0x80) 
    return false;
  pfvoid_t far* IVT = (pfvoid_t far*)0;
  pfvoid_t far* DVT = (pfvoid_t far*)IVT[0xff];
  DVT[function] = callback;  
}
