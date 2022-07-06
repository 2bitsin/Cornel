.386
.model tiny

_TEXT segment use16 para public 'CODE'
  _noop proc near
    ret
  _noop endp
_TEXT ends

_DATA segment use16 para public 'DATA'
  IRQ_table:
    dw offset _noop
    dw offset _noop
    dw offset _noop
    dw offset _noop
    dw offset _noop
    dw offset _noop
    dw offset _noop
    dw offset _noop
    dw offset _noop
    dw offset _noop
    dw offset _noop
    dw offset _noop
    dw offset _noop
    dw offset _noop
    dw offset _noop
    dw offset _noop  

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
    dd '!QRI'
    dd '!QRI'
    dd '!QRI'
    dd '!QRI'
    dd '!QRI'
    dd '!QRI'
    dd '!QRI'
    dd '!QRI'
    dd '!QRI'
    dd '!QRI'
    dd '!QRI'
    dd '!QRI'
    dd '!QRI'
    dd '!QRI'
    dd '!QRI'
    dd '!QRI'

_DATA ends

_TEXT segment use16 para public 'CODE'

  IRQ_M MACRO number
   ; save state
    pushad
    push      ds
    push      es
    push      fs
    push      gs   

    push      word ptr number
    call      word ptr cs:[_IRQ_stub_table + number*2]   
    add       sp,       2

    ; signal end of interrupt
  IF number GE 8
    mov       al,       0x20
    out       0xa0,     al
  ENDIF      
    out       0x20,     al

    ; restore state
    pop       gs
    pop       fs
    pop       es
    pop       ds    
    popad
    
    jmp       dword ptr cs:[_IRQ_save + number*4]   

  ENDM

  _IRQ0_stub proc far 
    IRQ_M 0
  _IRQ0_stub endp

  _IRQ1_stub proc far 
    IRQ_M 1  
  _IRQ1_stub endp

  _IRQ2_stub proc far 
    IRQ_M 2  
  _IRQ2_stub endp

  _IRQ3_stub proc far 
    IRQ_M 3  
  _IRQ3_stub endp

  _IRQ4_stub proc far 
    IRQ_M 4
  _IRQ4_stub endp

  _IRQ5_stub proc far 
    IRQ_M 5
  _IRQ5_stub endp

  _IRQ6_stub proc far 
    IRQ_M 6
  _IRQ6_stub endp

  _IRQ7_stub proc far 
    IRQ_M 7
  _IRQ7_stub endp

  _IRQ8_stub proc far 
    IRQ_M 8
  _IRQ8_stub endp

  _IRQ9_stub proc far 
    IRQ_M 9
  _IRQ9_stub endp

  _IRQ10_stub proc far 
    IRQ_M 10
  _IRQ10_stub endp

  _IRQ11_stub proc far 
    IRQ_M 11
  _IRQ11_stub endp

  _IRQ12_stub proc far 
    IRQ_M 12
  _IRQ12_stub endp

  _IRQ13_stub proc far 
    IRQ_M 13
  _IRQ13_stub endp

  _IRQ14_stub proc far 
    IRQ_M 14
  _IRQ14_stub endp

  _IRQ15_stub proc far  
    IRQ_M 15
  _IRQ15_stub endp



  IRQ_init_ proc near public
    push      edi

    push      dx
    push      cx
    push      bx

    mov       cx,       16
    xor       bx,       bx
  L$0:
    test      ax,       1
    jz        L$1
    mov       dx,       cs:[_IRQ_stub_table + bx*2]
    push      bx
    mov       bl,       cs:[_IRQ_map + bx]
    xor       bh,       bh

    mov       edi,      dword ptr gs:[bx*4]

    mov       word ptr gs:[bx*4 + 0], dx 
    mov       word ptr gs:[bx*4 + 2], cs
            
    pop       bx

    mov       dword ptr cs:[_IRQ_save + bx*4], edi
   
  L$1:
    shr       ax,       1
    jz        L$2
    inc       bx
    dec       cx
    jnz       L$0
  L$2:

    pop       bx
    pop       cx
    push      dx

    pop       edi
    ret
  IRQ_init_ endp


  IRQ_set_ proc near public
    push      bx
    and       bx,       0x0f
    mov       word ptr [IRQ_table + bx*2], ax
    pop       bx
    ret
  IRQ_set_ endp

  REQ_get_ proc near public
    push      bx
    mov       bx,       ax
    and       bx,       0x0f
    mov       ax,       word ptr [IRQ_table + bx*2]
    pop       bx
    ret
  REQ_get_ endp

_TEXT ends

end

