non_maskable_irq_off: 
    push    ax
    in      al,     0x70
    or      al,     0x80
    out     0x70,   al
    pop     ax
    ret

non_maskable_irq_on: 
    push    ax
    in      al,     0x70
    and     al,     0x7f
    out     0x70,   al
    pop     ax
    ret
