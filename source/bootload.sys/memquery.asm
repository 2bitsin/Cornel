_TEXT segment use16 public 'CODE'

MQ_e820_ proc far public
  ;[DX AX] [BX] [CX]

  push    es
  push    di

  mov     es,     dx
  mov     di,     ax
  add     di,     8

  mov     eax,    0xE820
  mov     ebx,    dword ptr es:[di - 4]
  mov     ecx,    dword ptr es:[di - 8]
  sub     ecx,    8
  mov     edx,    'SMAP'

  int     0x15
  jnc     MQ_e820_no_carry

  xor     al,     al
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
  pop     es
  ret

MQ_e820_ endp
  
_TEXT ends

end