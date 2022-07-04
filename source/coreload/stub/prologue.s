.386
.model tiny

  extern STUB_init_:proc near

_TEXT segment use16 public 'CODE'

  _hardware_reset proc near
  L$0:
    in          al,     0x64
    test        al,     0x02
    jnz         L$0
    mov         al,     0xfe
    out         0x64,   al
    ret
  _hardware_reset endp  

  _local_print_char proc near
    push        ax
    push        bx
    mov         ah,     0x0e
    mov         bx,     0x0007
    int         0x10
    pop         bx
    pop         ax    
    ret
  _local_print_char endp

  _local_print_eax proc near
    push        eax
    shr         eax,    24
    call        _local_print_char
    pop         eax
    push        eax
    shr         eax,    16
    call        _local_print_char
    pop         eax
    push        eax
    shr         eax,    8
    call        _local_print_char
    pop         eax
    push        eax    
    call        _local_print_char
    pop         eax
    ret
  _local_print_eax endp

  _local_print_hex_digit proc near
    push        bx
    push        ax
    cmp         al,     9
    ja          L$1
    add         al,     '0'-'A'
  L$1:
    add         al,     'A' 
    mov         ah,     0x0e
    mov         bx,     0x0007
    int         0x10
    pop         ax
    pop         bx
    ret
  _local_print_hex_digit endp

  _local_print_hex16 proc near
    push        ax        
  ; FIRST DIGIT
    shr         ax,     12
    call        _local_print_hex_digit
  ; SECOND DIGIT
    pop         ax
    push        ax
    shr         ax,     8
    and         ax,     0xf
    call        _local_print_hex_digit
  ; THIRD DIGIT
    pop         ax
    push        ax
    shr         ax,     4
    and         ax,     0xf
    call        _local_print_hex_digit
  ; FOURTH DIGIT
    pop         ax
    push        ax    
    and         ax,     0xf
    call        _local_print_hex_digit
  ; NEW LINE
    mov         al,     13
    call        _local_print_char
    mov         al,     10
    call        _local_print_char
    pop         ax
    ret
  _local_print_hex16 endp  

  _prologue_start proc near
    mov         sp,     G_STACK_SIZE
    mov         ax,     cs
    mov         ss,     ax
    mov         ds,     ax
    mov         es,     ax
    mov         fs,     ax
    mov         gs,     ax

    call        STUB_init_
    call        _hardware_reset

    cli
    hlt

  _prologue_start endp

_TEXT ends

_DATA segment use16 public 'DATA'
_DATA ends

_PROLOGUE segment use16 public 'PROLOGUE'

  _entry proc near public 
    mov         sp,     0x1000

    mov         eax,    ' CS='
    call        _local_print_eax
    mov         ax,     cs
    call        _local_print_hex16

    mov         eax,    ' DS='
    call        _local_print_eax
    mov         ax,     ds
    call        _local_print_hex16


    mov         eax,    ' ES='
    call        _local_print_eax        
    mov         ax,     es
    call        _local_print_hex16


    mov         eax,    ' FS='
    call        _local_print_eax
    mov         ax,     fs  
    call        _local_print_hex16


    mov         eax,    ' GS='
    call        _local_print_eax        
    mov         ax,     gs    
    call        _local_print_hex16

    mov         eax,    ' SS='
    call        _local_print_eax        
    mov         ax,     ss
    call        _local_print_hex16

    db 0xea
    dw offset _prologue_start
    dw (G_LOAD_ADDRESS - G_STACK_SIZE)/16
  _entry endp

_PROLOGUE ends

end

