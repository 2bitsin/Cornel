.386
.model tiny

_TEXT segment use16 public 'CODE'

MQ_e820_ proc near public
  ;[AX] [BX] [CX]

  push    di
  mov     di,     ax
  add     di,     8

  mov     eax,    0xE820
  mov     ebx,    dword ptr es:[di - 4]
  mov     ecx,    dword ptr es:[di - 8]
  sub     ecx,    8
  mov     edx,    'SMAP'

  int     0x15
  jnc     MQ_e820_no_carry
  mov     al,     ah
  xor     ah,     ah
  jmp     MQ_e820_exit

MQ_e820_no_carry:
  cmp     eax,    'SMAP'
  je      MQ_e820_smap_ok
  mov     ax,     0x86
  jmp     MQ_e820_exit

MQ_e820_smap_ok:  
  mov     dword ptr es:[di - 8], ecx
  mov     dword ptr es:[di - 4], ebx
  xor     ax,     ax

MQ_e820_exit:
  pop     di
  ret

MQ_e820_ endp
  
_TEXT ends

end