.386
.model tiny

_TEXT segment use16 public 'CODE'

  ; ******************************************
  ; * __U4D                                  *
  ; ******************************************

  __U4D proc near public

    shl     edx,      16
    movzx   eax,      ax
    or      eax,      edx
    xor     edx,      edx

    shl     ecx,      16
    movzx   ebx,      bx
    or      ebx,      ecx
    xor     ecx,      ecx

    div     ebx

    mov     ecx,      edx
    mov     ebx,      edx

    mov     edx,      eax
    shr     edx,      16
    and     eax,      0xffff

    shr     ecx,      16
    and     ebx,      0xffff
    ret  

  __U4D endp

  ; ******************************************
  ; * end __U4D                              *
  ; ******************************************


  ; ******************************************
  ; * __I4D                                  *
  ; ******************************************

  __I4D proc near public

    shl     edx,      16
    movzx   eax,      ax
    or      eax,      edx
    xor     edx,      edx
    test    eax,      0x80000000
    jz      __I4D_1
    not     edx
  __I4D_1:

    shl     ecx,      16
    movzx   ebx,      bx
    or      ebx,      ecx
    xor     ecx,      ecx
    test    ebx,      0x80000000
    jz      __I4D_2
    not     ecx
  __I4D_2:

    idiv    ebx

    mov     ecx,      edx
    mov     ebx,      edx

    mov     edx,      eax
    shr     edx,      16
    and     eax,      0xffff

    shr     ecx,      16
    and     ebx,      0xffff
    ret  

  __I4D endp

  ; ******************************************
  ; * end __I4D                              *
  ; ******************************************

  ; ******************************************
  ; * 64bit / 32bit dvision                  *
  ; ******************************************

  long_64_udiv_32_ proc near public 
    pushad

    mov     di,       ax
    mov     si,       dx

    mov     eax,      dword ptr [di + 0]
    mov     edx,      dword ptr [di + 4]  

    mov     ebx,      dword ptr [si + 0]

    div     ebx

    mov     dword ptr [di + 0],      eax
    mov     dword ptr [di + 4],      edx

    popad
    ret
  long_64_udiv_32_ endp

  ; ===============================================

  long_64_idiv_32_ proc near public 
    pushad

    mov     di,       ax
    mov     si,       dx

    mov     eax,      dword ptr [di + 0]
    mov     edx,      dword ptr [di + 4]  

    mov     ebx,      dword ptr [si + 0]

    idiv    ebx

    mov     dword ptr [di + 0],      eax
    mov     dword ptr [di + 4],      edx

    popad
    ret
  long_64_idiv_32_ endp

_TEXT ends

end