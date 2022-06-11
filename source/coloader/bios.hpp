#ifndef __BIOS_HPP__
#define __BIOS_HPP__

#include "types.hpp"

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

#endif 