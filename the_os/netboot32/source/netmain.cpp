#include <cstdint>

uint16_t * const G_screen = (uint16_t*)0xB8000;

extern "C"
void Netboot_main (void)
{
  for (auto i = 0; i < 80*25; ++i) {
    G_screen[i] = 0x0700 + '#';
  }

  asm("hlt");
}