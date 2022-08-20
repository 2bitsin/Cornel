.386
.model tiny

_DATA segment use16 para public 'DATA'
  
  extern _IRQ_handler_table:proc near
  extern _IRQ_saved_isr_table:proc near  
  extern _IRQ_save_ss:proc near
  extern _IRQ_save_sp:proc near
  extern _IRQ_stack_top:proc near
_DATA ends

_TEXT segment use16 para public 'CODE'

  IRQ_M MACRO number
  LOCAL L_end, L_no_ack, L_iret

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
    call      word ptr cs:[_IRQ_handler_table + number*2]    
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
    dw        offset _IRQ_saved_isr_table + number*4
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

_TEXT ends

end

