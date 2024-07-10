#include <i86.h>
#include "x86/bios.h"


uint16_t __declspec(naked) __watcall BIOS_query_memmap(
  memmap_item far* v_buff, uint32_t *v_size, uint32_t* v_next)
{__asm{
  push    es
  push    di
  push    si
  push    ecx
  push    ebx
  push    edx  
  mov     si,     bx
  mov     di,     cx
  mov     ecx,    [si]
  mov     ebx,    [di]
  push    si
  push    di
  mov     es,     dx
  mov     di,     ax  
  mov     edx,    0x534D4150
  mov     eax,    0x0000e820
  int     0x15
  jc      _error_
  cmp     eax,    0x534D4150
  jne     _error_
  pop     si
  mov     [si],   ebx
  pop     si
  mov     [si],   ecx
  xor     eax,    eax  
  jmp     _done_
_error_:
  and     eax,    0xffff  
_done_:
  pop     edx
  pop     ebx
  pop     ecx  
  pop     si
  pop     di
  pop     es
  ret  
}}

uint16_t __watcall BIOS_serial_puts (uint8_t port_number, char const far* string) {
  uint16_t index;
  for(index = 0; string[index]; index += 1u)
    BIOS_serial_putc(port_number, string[index]);
  return index;
}

uint16_t __watcall BIOS_serial_addr(uint8_t port_number) {
  static uint16_t far* _port_addr = (uint16_t far*)0ul;
  return _port_addr[port_number & 3u];
}
