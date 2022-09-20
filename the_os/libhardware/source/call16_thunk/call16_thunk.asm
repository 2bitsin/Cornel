    org   G_ORG
   
    use32
    jmp   short call16_thunk

    align 4
  _eax_   : dd 0xcccccccc
  _ebx_   : dd 0xcccccccc
  _ecx_   : dd 0xcccccccc
  _edx_   : dd 0xcccccccc
  _esi_   : dd 0xcccccccc
  _edi_   : dd 0xcccccccc
  _ebp_   : dd 0xcccccccc

  _ds_    : dw 0xcccc
  _es_    : dw 0xcccc
  _fs_    : dw 0xcccc
  _gs_    : dw 0xcccc
  
  _esp_   : dd 0xcccccccc
  _ss_    : dw 0xcccc
  _flags_ : dw 0xcccc

  _cs_ip_ : dw call16_thunk.int_xx, 0

  _o_esp_ : dd 0xcccccccc
  _o_ss_  : dw 0xcccc
    
  call16_thunk:
    ; save registers
    pushad 
    push        es
    push        ds
    push        fs
    push        gs
    pushfd

    ; save stack
    mov         [_o_ss_ ],  ss
    mov         [_o_esp_],  esp

    ; start thunk
    jmp         0x18:.load_code16
  .load_code16: 

    use16

    mov         eax,        cr0
    and         ax,         0xfe
    mov         cr0,        eax
    jmp         0x00:.real_mode    
  .real_mode:   
    mov         eax,        [cs:_eax_]
    mov         ebx,        [cs:_ebx_]
    mov         ecx,        [cs:_ecx_]
    mov         edx,        [cs:_edx_]
    mov         esi,        [cs:_esi_]
    mov         edi,        [cs:_edi_]
    mov         ebp,        [cs:_ebp_]
    mov         ds,         [cs:_ds_]
    mov         es,         [cs:_es_]
    mov         fs,         [cs:_fs_]
    mov         gs,         [cs:_gs_]
    mov         sp,         cs
    mov         ss,         sp
    mov         sp,         _flags_
    popf
    mov         ss,         [cs:_ss_]
    mov         esp,        [cs:_esp_]
    call        dword [cs:_cs_ip_]

    mov         [cs:_esp_], esp
    mov         [cs:_ss_],  ss
    mov         sp,         cs
    mov         ss,         sp
    mov         sp,         _flags_+2
    pushf 
    mov         [cs:_gs_],  gs
    mov         [cs:_fs_],  fs
    mov         [cs:_es_],  es
    mov         [cs:_ds_],  ds
    mov         [cs:_ebp_], ebp
    mov         [cs:_edi_], edi
    mov         [cs:_esi_], esi
    mov         [cs:_edx_], edx
    mov         [cs:_ecx_], ecx
    mov         [cs:_ebx_], ebx
    mov         [cs:_eax_], eax

  .reentering_prot_mode:
    mov         eax,        cr0
    or          ax,         1
    mov         cr0,        eax
    jmp         0x08:.prot_mode
   
    use32

  .prot_mode:
    lss         esp,        [cs:_o_esp_]
    popfd
    pop         gs
    pop         fs
    pop         es
    pop         ds
    popad
    ret
  
    use16
  
  .int_xx:
    int         0xcc
    retf

    times 512-($-$$) nop
