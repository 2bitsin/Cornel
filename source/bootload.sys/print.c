#include "print.h"

void bios_put_char (char value, unsigned char page, unsigned char color);
#pragma aux bios_put_char = \
  "mov ah, 0x0e" \
  "int 0x10" \
  parm [al] [bh] [bl] \
  modify [ax] [bx];
  

void print_char(char value)
{
  if (value == '\n')
    bios_put_char('\r', 0, 0x07);
  bios_put_char(value, 0, 0x07);
}

void print_string(const char _far* value)
{
  while (*value)
    print_char(*value++);
}

static inline char hex_char (char value)
{
  return value < 10 ? '0' + value : 'A' + (value - 10);
}

void print_hex8(const unsigned char value)
{
  print_char(hex_char((value >> 4) & 0x0F));
  print_char(hex_char(value & 0x0F));
}

void print_hex16(const unsigned short value)
{
  print_hex8((value >> 8) & 0xFF);
  print_hex8(value & 0xFF);
}

void print_hex32(const unsigned long value)
{
  print_hex16((value >> 16) & 0xFFFF);
  print_hex16(value & 0xFFFF);
}

void print_hex64(const unsigned __int64 value)
{
  print_hex32(((const unsigned long _far*)&value)[0]);
  print_hex32(((const unsigned long _far*)&value)[1]);  
}

