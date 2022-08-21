#include <stdint.h>

uint16_t * const G_screen = (uint16_t*)0xB8000;

void NET_main (void)
{
  int i;

  for (i = 0; i < 80*25; ++i) {
    G_screen[i] = 0x0700 + '#';
  }

  asm("hlt");
}