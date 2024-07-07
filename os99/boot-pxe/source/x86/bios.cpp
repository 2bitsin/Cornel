#include <i86.h>
#include "x86/bios.h"


uint16_t __declspec(naked) __watcall BIOS_query_memmap(memmap_entry 
  far* v_buff, uint32_t *v_size, uint32_t* v_next)
{__asm{

  push    es
  push    di
  push    si
  push    ecx
  push    ebx
  push    edx
  

  mov     si,     bx    // size
  mov     di,     cx    // next
  mov     ecx,    [si]
  mov     ebx,    [di]
  push    si            // size
  push    di            // next

  mov     es,     dx
  mov     di,     ax  

  mov     edx,    0x534D4150
  mov     eax,    0x0000e820
  int     0x15
  jc      @error     
  cmp     eax,    0x534D4150
  jne     @error
  pop     si            // next
  mov     [si],   ebx
  pop     si            // size
  mov     [si],   ecx
  xor     eax,    eax  
@error:
  and     eax,    0xffff  
@done:
  pop     edx
  pop     ebx
  pop     ecx  
  pop     si
  pop     di
  pop     es
  ret
}}