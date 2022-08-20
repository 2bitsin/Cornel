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
    db        0xea              ; jmp 0x8:_FLAT_load_32bit_segments
    dw        G_BASE_ADDRESS + offset _FLAT_load_32bit_segments
    dw        0x0008   

  _FLAT_reload_16bit_code_segment:
    db        0x0f, 0x20, 0xc0  ; mov eax, cr0
    xor       ax,           1
    db        0x0f, 0x22, 0xc0  ; mov cr0, eax
    db        0xea              ; jmp 0x0000:_FLAT_come_down_to_real_mode
    dw        G_BASE_ADDRESS + offset _FLAT_come_down_to_real_mode
    dw        0x0000

  _FLAT_come_down_to_real_mode:
    db        0xea              ; jmp LOAD_SEGMENT:_FLAT_adjust_real_mode_code_segment
    dw        offset _FLAT_adjust_real_mode_code_segment
    dw        G_BASE_ADDRESS/16

  _FLAT_adjust_real_mode_code_segment:  
    call      RTC_nmi_enable_
    sti   

    ; FS = GS = base of 0
    xor       ax,           ax
    mov       fs,           ax
    mov       gs,           ax

    pop       es
    pop       ds
    popad    
    ret
  
  _FLAT_init endp
  
_TEXT ends

_TEXT32 segment use32 para public 'CODE32'

  _FLAT_load_32bit_segments proc near

    ; Load 32bit flat data segments
    mov   ax,           0x10
    mov   ds,           ax
    mov   es,           ax
    mov   fs,           ax
    mov   gs,           ax

    db    0xea                    ; jmp 0x18:_FLAT_reload_16bit_code_segment
    dd    G_BASE_ADDRESS + offset _FLAT_reload_16bit_code_segment
    dw    0x0018

  _FLAT_load_32bit_segments endp

_TEXT32 ends 

_TEXT segment use16 para public 'CODE'
  ; FLAT_copy8 (uint32_t dst, uint32_t src, uint32_t size)
  _FLAT_copy1 proc near public
  
    push      bp
    mov       bp,         sp
  
    push      eax
    push      esi
    push      edi
    push      ecx
    push      ebx
  
    mov       edi,        dword ptr [bp + 4]
    mov       esi,        dword ptr [bp + 8]
    mov       ecx,        dword ptr [bp + 12]
    xor       ebx,        ebx
  
  L$1:
    mov       al,         gs:[esi + ebx]
    mov       gs:[edi + ebx], al
    inc       ebx
    dec       ecx
    jnz       L$1  
  
    pop       ebx
    pop       ecx
    pop       edi
    pop       esi
    pop       eax
  
    pop       bp
    ret
  
  _FLAT_copy1 endp

  ; FLAT_copy2 (uint32_t dst, uint32_t src, uint32_t size)
  _FLAT_copy2 proc near public
  
    push      bp
    mov       bp,         sp
  
    push      eax
    push      esi
    push      edi
    push      ecx
    push      ebx
  
    mov       edi,        dword ptr [bp + 4]
    mov       esi,        dword ptr [bp + 8]
    mov       ecx,        dword ptr [bp + 12]
    xor       ebx,        ebx
  
  L$2:
    mov       ax,         gs:[esi + ebx*2]
    mov       gs:[edi + ebx*2], ax
    inc       ebx
    dec       ecx
    jnz       L$2
  
    pop       ebx
    pop       ecx
    pop       edi
    pop       esi
    pop       eax
  
    pop       bp
    ret
  
  _FLAT_copy2 endp

  ; FLAT_copy4 (uint32_t dst, uint32_t src, uint32_t size)
  _FLAT_copy4 proc near public
  
    push      bp
    mov       bp,         sp
  
    push      eax
    push      esi
    push      edi
    push      ecx
    push      ebx
  
    mov       edi,        dword ptr [bp + 4]
    mov       esi,        dword ptr [bp + 8]
    mov       ecx,        dword ptr [bp + 12]
    xor       ebx,        ebx
  
  L$3:
    mov       eax,        gs:[esi + ebx*4]
    mov       gs:[edi + ebx*4], eax
    inc       ebx
    dec       ecx
    jnz       L$3
  
    pop       ebx
    pop       ecx
    pop       edi
    pop       esi
    pop       eax
  
    pop       bp
    ret
    
  _FLAT_copy4 endp

  ; FLAT_fill1 (uint32_t dst, uint32_t size, uint8_t value)
  _FLAT_fill1 proc near public
  
    push      bp
    mov       bp,         sp
  
    push      eax
    push      ecx
    push      edi
    push      ebx
  
    mov       edi,        dword ptr [bp + 4]    
    mov       ecx,        dword ptr [bp + 8]
    mov       al,         byte ptr [bp + 12]
    xor       ebx,        ebx

  Q$1:
    mov       gs:[edi + ebx], al
    inc       ebx
    dec       ecx
    jnz       Q$1
  
    pop       ebx
    pop       edi
    pop       ecx
    pop       eax

    pop       bp
    ret

  _FLAT_fill1 endp

  ; FLAT_fill2 (uint32_t dst, uint32_t size, uint16_t value)
  _FLAT_fill2 proc near public
  
    push      bp
    mov       bp,         sp
  
    push      eax
    push      ecx
    push      edi
    push      ebx
  
    mov       edi,        dword ptr [bp + 4]    
    mov       ecx,        dword ptr [bp + 8]
    mov       ax,         word ptr [bp + 12]
    xor       ebx,        ebx

  Q$2:
    mov       gs:[edi + ebx*2], ax
    inc       ebx
    dec       ecx
    jnz       Q$2
  
    pop       ebx
    pop       edi
    pop       ecx
    pop       eax

    pop       bp
    ret

  _FLAT_fill2 endp

  ; FLAT_fill4 (uint32_t dst, uint32_t size, uint32_t value)
  _FLAT_fill4 proc near public
  
    push      bp
    mov       bp,         sp
  
    push      eax
    push      ecx
    push      edi
    push      ebx
  
    mov       edi,        dword ptr [bp + 4]    
    mov       ecx,        dword ptr [bp + 8]
    mov       eax,        dword ptr [bp + 12]
    xor       ebx,        ebx

  Q$3:
    mov       gs:[edi + ebx*4], eax
    inc       ebx
    dec       ecx
    jnz       Q$3
  
    pop       ebx
    pop       edi
    pop       ecx
    pop       eax

    pop       bp
    ret

  _FLAT_fill4 endp

_TEXT ends

end
