#include "bios.hpp"


void tty_put_string(const char *str, ubyte color, ubyte page)
{
  for(int i =0; str[i] != 0; i++)
  {
    if (str[i] == '\n')
      tty_put_char('\r', color, page);
    tty_put_char(str[i], color, page);
  }
}