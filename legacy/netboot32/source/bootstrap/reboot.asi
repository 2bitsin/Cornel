
  reboot_via_8042:
    in      al,     0x64
    and     al,     0x02
    jnz     reboot_via_8042
    mov     al,     0xfe
    out     0x64,   al
    ret     ; failed ?:/