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

inline pfvoid_t get_ivt_entry(uint8_t v_index) 
{
  return ((pfvoid_t far*)0x0u)[v_index];
}

inline void set_ivt_entry(uint8_t v_index, pfvoid_t v_value) 
{
  ((pfvoid_t far*)0x0u)[v_index] = v_value;
}

