#include "print.h"
#include "longdiv.h"

void bios_put_char (char value, uint8_t page, uint8_t color);
#pragma aux bios_put_char = \
  "mov ah, 0x0e" \
  "int 0x10" \
  parm [al] [bh] [bl] \
  modify [ax] [bx];

void bios_set_video_mode (uint8_t mode);
#pragma aux bios_set_video_mode = \
  "mov ah, 0x00" \
  "int 0x10" \
  parm [al] \
  modify [ax];
  
void init_text() 
{ 
  bios_set_video_mode(0x03);
}

void print_char(char value)
{
  if (value == '\n')
    bios_put_char('\r', 0, 0x07);
  bios_put_char(value, 0, 0x07);
}

void print_string(const char* value)
{
  while (*value)
    print_char(*value++);
}

void print_string_n(const char* value, size_t n)
{
  while (n--)
    print_char(*value++);
}

static inline char hex_char (uint8_t value)
{
  if (value > 36)
    return '-';
  return value < 10 ? '0' + value : 'A' + (value - 10);
}

void print_hex8(uint8_t value)
{
  print_char(hex_char((value >> 4) & 0x0F));
  print_char(hex_char(value & 0x0F));
}

void print_hex16(uint16_t value)
{
  print_hex8((value >> 8) & 0xFF);
  print_hex8(value & 0xFF);
}

void print_hex32(uint32_t value)
{
  print_hex16((value >> 16) & 0xFFFF);
  print_hex16(value & 0xFFFF);
}

void print_hex64(uint64_t value)
{
  print_hex32(((const uint32_t *)&value)[1]);
  print_hex32(((const uint32_t *)&value)[0]);  
}

static char G_print_buffer[32];

void reverse(char* value, size_t size)
{
  char temp;
  size_t i;
  for(i = 0u; i < size / 2u; ++i)
  {
    temp = value[i];
    value[i] = value[size - i - 1];
    value[size - i - 1] = temp;
  }
}

void print_dec32(uint32_t value)
{
  int i = 0;
  static u_longdiv_type t;
  static const unsigned long d = 10u;

  if (value == 0)
  {
    print_char('0');
    return;
  }

  while(value > 0)
  {
    t.d = value;
    long_64_udiv_32(&t, &d);
    G_print_buffer[i++] = hex_char(t.r);
    value = t.q;    
  }
  reverse(G_print_buffer, i);
  print_string_n(G_print_buffer, i);
}

void print_dec16(uint16_t value)
{
  print_dec32(value);
}

void print_dec8(uint8_t value)
{
  print_dec32(value);
}


