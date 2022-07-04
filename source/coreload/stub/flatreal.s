.386
.model tiny

  extern RTC_nmi_disable_:proc
  extern RTC_nmi_enable_:proc
  extern _GDTR_bits:proc

_TEXT segment use16 para public 'CODE'

  _FLAT_init proc near public
  
    pushad 
    push      ds 
    push      es
    push      fs
    push      gs
  
    cli
    call      RTC_nmi_disable_
  
    db        0x0f, 0x01, 0x16  ; lgdt [GDTR_temp]
    dw        offset _GDTR_bits
    db        0x0f, 0x20, 0xc0  ; mov eax, cr0
    xor       ax,           1
    db        0x0f, 0x22, 0xc0  ; mov cr0, eax
    db        0xea              ; jmp 0x8:_FLAT_init32
    dw        offset _FLAT_init32 + G_LOAD_ADDRESS - G_STACK_SIZE
    dw        0x0008 
  
  _FLAT_backto16:
    db        0x0f, 0x20, 0xc0  ; mov eax, cr0
    xor       ax,           1
    db        0x0f, 0x22, 0xc0  ; mov cr0, eax
    db        0xea              ; jmp 0x0000:_FLAT_backtorm
    dw        offset _FLAT_backtorm + G_LOAD_ADDRESS - G_STACK_SIZE
    dw        0x0000
  _FLAT_backtorm:
    db        0xea              ; jmp LOAD_SEGMENT:_FLAT_reload_cs
    dw        offset _FLAT_reload_cs
    dw        (G_LOAD_ADDRESS - G_STACK_SIZE)/16
  _FLAT_reload_cs:
  
    call      RTC_nmi_enable_
    sti   
    pop       gs 
    pop       fs
    pop       es
    pop       ds
    popad
    ret
  
  _FLAT_init endp
  
  ; FLAT_memcpy (uint32_t dst, uint32_t src, uint32_t size)
  _FLAT_copy proc near public
  
    push      bp
    mov       bp,         sp
  
    push      ds
    push      es  
    push      eax
    push      ecx
    push      esi
    push      edi
  
    xor       ax,         ax
    mov       ds,         ax
    mov       es,         ax
    mov       edi,        dword ptr [bp + 4]
    mov       esi,        dword ptr [bp + 8]
    mov       ecx,        dword ptr [bp + 12]
  
  L$1:
    mov       eax,        ds:[esi]
    mov       ds:[edi],   eax
    inc       esi
    inc       edi
    dec       ecx
    jnz       L$1  
  
    pop       edi
    pop       esi
    pop       ecx
    pop       eax
    pop       es
    pop       ds
  
    pop       bp
    ret
  
  _FLAT_copy endp

_TEXT ends

_TEXT32 segment use32 para public 'CODE'

  _FLAT_init32 proc near

    mov   ax,           0x10
    mov   ds,           ax
    mov   es,           ax
    mov   fs,           ax
    mov   gs,           ax

    db    0xea                    ; jmp 0x18:_FLAT_backto16
    dd    offset _FLAT_backto16 + G_LOAD_ADDRESS - G_STACK_SIZE
    dw    0x0018

  _FLAT_init32 endp

_TEXT32 ends 

end
