    format binary    
    use16

    org     0x7c00

preamble:
    mov     ax,     cs
    mov     ds,     ax
    mov     es,     ax
    mov     fs,     ax
    mov     gs,     ax

    mov     si,     hello
    call    putstr
    
    cli
    hlt
        
putstr:
    lodsb 
    call    putchar
    or      al,     al
    jnz     putstr
    ret

putchar: 
    push    ax
    push    bx
    mov     ah,     0x0e
    mov     bx,     0x07
    int     0x10
    pop     bx
    pop     ax
    ret

hello:
    db      "Hello, world!\n"
