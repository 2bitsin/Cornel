#ifndef __TERMIO_HPP__
#define __TERMIO_HPP__


#include "charconv.hpp"


void tty_put_char(char c, ubyte color = 0, ubyte page = 0);
#pragma aux tty_put_char =      \
  "mov ah, 0x0E"                \
  "int 0x10"                    \
  parm [al] [bl] [bh]           \
  modify [ah]                   ;

char tty_wait_key_char();
#pragma aux tty_wait_key_char = \
  "mov ah, 0x00"                \
  "int 0x16"                    \
  parm []                       \
  modify [ah]                   \
  value [al]                    ;

ubyte tty_wait_key_code();
#pragma aux tty_wait_key_code = \
  "mov ah, 0x00"                \
  "int 0x16"                    \
  parm []                       \
  modify [al]                   \
  value [ah]                    ;

void tty_put_string(const char *str, ubyte color = 0, ubyte page = 0);

template <typename T>
inline void tty_put_integer(T value, int base = 10, ubyte color = 0, ubyte page = 0)
{ 
  static char buf[32]; 
  char* end;
  if (base != 16)
    end = to_chars(buf, buf + 32, value, base);
  else
    end = to_hex(buf, buf + 32, value);
  *end = 0;
  tty_put_string(buf, color, page);
}

#endif