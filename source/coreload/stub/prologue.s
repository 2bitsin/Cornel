.386
.model tiny

  extern STUB_init_:proc near
  extern STUB_exit_:proc near

_TEXT segment use16 public 'CODE'

  _prologue_start proc near    
    mov         sp,     G_STACK_SIZE
    mov         ax,     cs
    mov         ss,     ax
    mov         ds,     ax
    mov         es,     ax
    mov         fs,     ax
    mov         gs,     ax
    sti
    call        STUB_init_
    jmp         STUB_exit_
  _prologue_start endp

_TEXT ends

_PROLOGUE segment use16 public 'PROLOGUE'

  _entry proc near public
  IF DEBUG_STARTUP equ 1
  Q$0:
    cli
    mov         sp,     0x9000
    mov         ss,     sp
    mov         sp,     0x0000
    call        Q$1
  Q$1:          
    pop         ax
    mov         dx,     cs
    shl         dx,     4
    add         ax,     dx
    cmp         ax,     G_LOAD_ADDRESS + (offset Q$1 - offset Q$0)
    jz          Q$2
    mov         ax,     0x0e01
    mov         bx,     0x0007
    int         0x10
    cli
    hlt    
  Q$2:
  ENDIF
    db 0xea
    dw offset _prologue_start
    dw G_BASE_ADDRESS/16
  _entry endp

_PROLOGUE ends

_DATA segment use16 public 'DATA'
_DATA ends

end

