#include "x86/flatmem.h"

uint32_t __declspec(naked) flat_fill(
  uint32_t  target_address, 
  uint8_t   fill_with, 
  uint32_t  bytes_to_fill) 
{__asm{  
  push      bp
  mov       bp,     sp
  push      es
  push      ax
  pushf
  push      edi
  push      ecx
  xor       ax,     ax
  mov       es,     ax
  mov       edi,    [bp+4]
  mov       al,     [bp+8]
  mov       ecx,    [bp+10]
  cld
  db        0x67
  rep       stosb
  pop       ecx
  pop       edi
  popf
  pop       ax
  pop       es
  pop       bp
  ret
}}


uint32_t __declspec(naked) flat_copy(
  uint32_t  target_address, 
  uint32_t  source_address, 
  uint32_t  bytes_to_copy)
{__asm{
  push      bp
  mov       bp,     sp
  pushf
  push      ds
  push      es
  push      eax
  push      edi
  push      esi
  push      ecx
  xor       cx,     cx
  mov       ds,     cx
  mov       es,     cx
  mov       ecx,    [bp+12]
  mov       esi,    [bp+8]
  mov       edi,    [bp+4]
  cmp       edi,    esi
  jz        _done_
  ja        _bkwd_
  cld
  db        0x67
  rep       movsb  
  jmp       _done_
_bkwd_:
  std
  add       esi,    ecx
  dec       esi
  add       edi,    ecx
  dec       edi
  db        0x67
  rep       movsb
_done_:
  pop       ecx
  pop       esi
  pop       edi
  pop       eax
  pop       es
  pop       ds
  popf
  pop       bp  
  ret
}} 

