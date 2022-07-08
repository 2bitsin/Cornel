.386
.model tiny

_TEXT segment use16 para public 'CODE'
  _noop proc near
    ret
  _noop endp
_TEXT ends

_DATA segment use16 para public 'DATA'
  extern _SER_irq:proc near
  extern _PIT_irq:proc near

  _IRQ_table:
    dw offset _PIT_irq ; 0   
    dw offset _noop ; 1
    dw offset _noop ; 2
    dw offset _SER_irq ; 3
    dw offset _SER_irq ; 4
    dw offset _noop ; 5
    dw offset _noop ; 6
    dw offset _noop ; 7
    dw offset _noop ; 8
    dw offset _noop ; 9
    dw offset _noop ; 10
    dw offset _noop ; 11
    dw offset _noop ; 12
    dw offset _noop ; 13
    dw offset _noop ; 14
    dw offset _noop ; 15

  _IRQ_stub_table:
    dw offset _IRQ0_stub
    dw offset _IRQ1_stub
    dw offset _IRQ2_stub
    dw offset _IRQ3_stub
    dw offset _IRQ4_stub
    dw offset _IRQ5_stub
    dw offset _IRQ6_stub
    dw offset _IRQ7_stub
    dw offset _IRQ8_stub
    dw offset _IRQ9_stub
    dw offset _IRQ10_stub
    dw offset _IRQ11_stub
    dw offset _IRQ12_stub
    dw offset _IRQ13_stub
    dw offset _IRQ14_stub
    dw offset _IRQ15_stub

  _IRQ_map:
    db 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F
    db 0x70, 0x71, 0x72, 0x73, 0x74, 0x75, 0x76, 0x77

  _IRQ_save:
    dd 16 dup (0x21515249)
  
  _IRQ_save_ss:
    dw 'SS'

  _IRQ_save_sp:
    dw 'PS'

  _IRQ_mask:
    dw 0x0000

  _IRQ_init_mask:
    dw 0xFFFF

  _IRQ_next_action:
    dw 0x0000

  _IRQ_stack_bottom:
    dq        128 dup (0x4B43415453515249)
  _IRQ_stack_top:
_DATA ends

_TEXT segment use16 para public 'CODE'

  IRQ_M MACRO number
  LOCAL L_end, L_no_ack, L_iret

    test      word ptr cs:[_IRQ_mask], (1 shl number)
    jz        short L_end

   ; save stack    
    mov       word ptr cs:[_IRQ_save_ss], ss
    mov       word ptr cs:[_IRQ_save_sp], sp

    mov       sp,       cs
    mov       ss,       sp
    mov       sp,       offset _IRQ_stack_top

   ; save registers
    pushad
    push      ds
    push      es
    push      fs
    push      gs   

    mov       ax,       cs
    mov       ds,       ax
    mov       es,       ax
    xor       ax,       ax
    mov       fs,       ax
    mov       gs,       ax
    push      word ptr number
    call      word ptr cs:[_IRQ_table + number*2]    
    add       sp,       2

    ; load return value into flags
    mov       ah,       al
    sahf

    ; restore state
    pop       gs
    pop       fs
    pop       es
    pop       ds    

    ; signal end of interrupt

    ; skip of PF=0
    jpo       short L_no_ack
    mov       al,       0x20
  IF number GE 8
    out       0xa0,     al
  ENDIF      
    out       0x20,     al
  L_no_ack:
    
    popad
    
    mov       ss, word ptr cs:[_IRQ_save_ss]
    mov       sp, word ptr cs:[_IRQ_save_sp]
    
    ; skip if CF=0
    jnc       short L_iret
  L_end:    
    db        0x2E, 0xFF, 0x2E 
    dw        offset _IRQ_save + number*4
  L_iret:
    iret

  ENDM

  _IRQ0_stub proc far public
    IRQ_M 0
  _IRQ0_stub endp

  _IRQ1_stub proc far public
    IRQ_M 1  
  _IRQ1_stub endp

  _IRQ2_stub proc far public 
    IRQ_M 2  
  _IRQ2_stub endp

  _IRQ3_stub proc far public 
    IRQ_M 3  
  _IRQ3_stub endp

  _IRQ4_stub proc far public 
    IRQ_M 4
  _IRQ4_stub endp

  _IRQ5_stub proc far public 
    IRQ_M 5
  _IRQ5_stub endp

  _IRQ6_stub proc far public 
    IRQ_M 6
  _IRQ6_stub endp

  _IRQ7_stub proc far public 
    IRQ_M 7
  _IRQ7_stub endp

  _IRQ8_stub proc far public 
    IRQ_M 8
  _IRQ8_stub endp

  _IRQ9_stub proc far public 
    IRQ_M 9
  _IRQ9_stub endp

  _IRQ10_stub proc far public 
    IRQ_M 10
  _IRQ10_stub endp

  _IRQ11_stub proc far public 
    IRQ_M 11
  _IRQ11_stub endp

  _IRQ12_stub proc far public
    IRQ_M 12
  _IRQ12_stub endp

  _IRQ13_stub proc far public 
    IRQ_M 13
  _IRQ13_stub endp

  _IRQ14_stub proc far public 
    IRQ_M 14
  _IRQ14_stub endp

  _IRQ15_stub proc far public  
    IRQ_M 15
  _IRQ15_stub endp


  IRQ_init_ proc near public
    pushf
    cli
    push      edi
    push      ebx
    push      dx
    push      cx
    
    mov       bx,       cs:[_IRQ_init_mask]
    not       bx
    and       ax,       bx

    or        word ptr cs:[_IRQ_init_mask], ax
    or        word ptr cs:[_IRQ_mask], ax    

    mov       cx,       16
    xor       ebx,      ebx
    mov       gs,       bx

  L$0:
    test      ax,       1
    jz        L$1
    mov       dx,       cs:[_IRQ_stub_table + ebx*2]
    push      bx
    mov       bl,       cs:[_IRQ_map + ebx]
    xor       bh,       bh

    mov       edi,      dword ptr gs:[ebx*4]

    mov       word ptr gs:[ebx*4 + 0], dx 
    mov       word ptr gs:[ebx*4 + 2], cs
            
    pop       bx

    mov       dword ptr cs:[_IRQ_save + ebx*4], edi
   
  L$1:
    shr       ax,       1
    jz        L$2
    inc       bx
    dec       cx
    jnz       L$0
  L$2:

    pop       cx
    pop       dx
    pop       ebx    
    pop       edi
    popf
    ret
  IRQ_init_ endp


  IRQ_set_ proc near public
    pushf
    cli
    push      edx
    and       dx,       0x0f
    mov       word ptr [_IRQ_table + edx*2], ax
    pop       edx
    popf
    ret
  IRQ_set_ endp

  IRQ_get_ proc near public
    push      edx
    mov       dx,       ax
    and       dx,       0x0f
    mov       ax,       word ptr [_IRQ_table + edx*2]
    pop       edx
    ret
  IRQ_get_ endp

  IRQ_enable_ proc near public
    or        word ptr cs:[_IRQ_mask], ax    
    ret
  IRQ_enable_ endp
  
  IRQ_disable_ proc near public
    not       ax
    and       word ptr cs:[_IRQ_mask], ax    
    ret
  IRQ_disable_ endp

_TEXT ends

end

