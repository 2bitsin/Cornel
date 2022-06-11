#include "memory.hpp"
#include "bios.hpp"

inline ubyte memory_load_map_entry(e820_entry_type& out_entry, uint32& inout_size, uint32& inout_next)
{
  uint32 out_smap = 0;
  uint32 out_next = 0;
  uint32 out_size = 0;
  uint16 out_errc = 0;

  out_size = sizeof(e820_entry_type);  

  __asm
  {
    mov eax, 0xe820
    mov edx, 'SMAP'
    mov ebx, dword ptr [inout_next]
    mov ecx, dword ptr [out_size]
    les di, [out_entry]
    int 0x15
    pushf
    mov [out_smap], eax
    mov [out_next], ebx
    mov [out_size], ecx
    pop [out_errc]
  }

  if (out_smap != 'SMAP' || (out_errc & 1)) {
    return out_smap & 0xff;
  }

  inout_size = out_size;
  inout_next = out_next;

  return 0;
}



void memory_load_map()
{
  e820_entry_type entry;
  uint32 size = sizeof(e820_entry_type);
  uint32 next = 0;
  while(memory_load_map_entry(entry, size, next) == 0)
  {
    tty_put_string("Entry: ");
    switch (entry.type)
    {
    case 1: tty_put_string("Availble"); break;
    case 2: tty_put_string("Reserved"); break;
    case 3: tty_put_string("ACPI Reclaim"); break;
    case 4: tty_put_string("ACPI NVS Memory"); break;
    default: tty_put_string("Unknown"); break;
    }
    tty_put_string("\n");
    if (next == 0)
      break;
  }
}
