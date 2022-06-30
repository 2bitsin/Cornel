.386
.model tiny

_TEXT segment use16 public 'CODE'

MEM_read_acpi_entry_ proc near public
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
  jnc     MEM_read_acpi_entry_no_carry
  mov     al,     ah
  xor     ah,     ah
  jmp     MEM_read_acpi_entry_exit

MEM_read_acpi_entry_no_carry:
  cmp     eax,    'SMAP'
  je      MEM_read_acpi_entry_smap_ok
  mov     ax,     0x86
  jmp     MEM_read_acpi_entry_exit

MEM_read_acpi_entry_smap_ok:  
  mov     dword ptr es:[di - 8], ecx
  mov     dword ptr es:[di - 4], ebx
  xor     ax,     ax

MEM_read_acpi_entry_exit:
  pop     di
  ret

MEM_read_acpi_entry_ endp


_TEXT ends

end