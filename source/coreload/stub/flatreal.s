.386
.model tiny

  extern RTC_nmi_disable_:proc
  extern RTC_nmi_enable_:proc

  extern _GDT_bits:proc near
  extern _GDT_size:proc near

_TEXT segment use16 para public 'CODE'

  _FLAT_init proc near public
  
    pushad 
    push      ds 
    push      es
    push      fs
    push      gs
  
    cli
    call      RTC_nmi_disable_
  
    mov       bx,           sp   
    pushd     G_BASE_ADDRESS + offset _GDT_bits
    push      word ptr [_GDT_size]    
    xchg      bx,           sp

    db        0x0f, 0x01, 0x17  ; lgdt [bx]
    db        0x0f, 0x20, 0xc0  ; mov eax, cr0
    xor       ax,           1
    db        0x0f, 0x22, 0xc0  ; mov cr0, eax
    db        0xea              ; jmp 0x8:_FLAT_init32
    dw        G_BASE_ADDRESS + offset _FLAT_init32
    dw        0x0008 
  
  _FLAT_backto16:
    db        0x0f, 0x20, 0xc0  ; mov eax, cr0
    xor       ax,           1
    db        0x0f, 0x22, 0xc0  ; mov cr0, eax
    db        0xea              ; jmp 0x0000:_FLAT_backtorm
    dw        G_BASE_ADDRESS + offset _FLAT_backtorm
    dw        0x0000
  _FLAT_backtorm:
    db        0xea              ; jmp LOAD_SEGMENT:_FLAT_reload_cs
    dw        offset _FLAT_reload_cs
    dw        G_BASE_ADDRESS/16
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
  
  ; FLAT_copy8 (uint32_t dst, uint32_t src, uint32_t size)
  _FLAT_copy1 proc near public
  
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
    mov       al,         ds:[esi]
    mov       ds:[edi],   al
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
  
  _FLAT_copy1 endp

  ; FLAT_copy2 (uint32_t dst, uint32_t src, uint32_t size)
  _FLAT_copy2 proc near public
  
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
  
  L$2:
    mov       ax,         ds:[esi]
    mov       ds:[edi],   ax
    add       esi,        2
    add       edi,        2
    dec       ecx
    jnz       L$2
  
    pop       edi
    pop       esi
    pop       ecx
    pop       eax
    pop       es
    pop       ds
  
    pop       bp
    ret
  
  _FLAT_copy2 endp

  ; FLAT_copy4 (uint32_t dst, uint32_t src, uint32_t size)
  _FLAT_copy4 proc near public
  
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
  
  L$3:
    mov       eax,        ds:[esi]
    mov       ds:[edi],   eax
    add       esi,        4
    add       edi,        4
    dec       ecx
    jnz       L$3
  
    pop       edi
    pop       esi
    pop       ecx
    pop       eax
    pop       es
    pop       ds
  
    pop       bp
    ret
  
  _FLAT_copy4 endp

_TEXT ends

_TEXT32 segment use32 para public 'CODE32'

  _FLAT_init32 proc near

    mov   ax,           0x10
    mov   ds,           ax
    mov   es,           ax
    mov   fs,           ax
    mov   gs,           ax

    db    0xea                    ; jmp 0x18:_FLAT_backto16
    dd    offset _FLAT_backto16 + G_BASE_ADDRESS
    dw    0x0018

  _FLAT_init32 endp

_TEXT32 ends 

end
