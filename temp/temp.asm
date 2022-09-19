use16

  jmp short Q
L: 

  dd 0x90909090 ; edi
  dd 0x90909090 ; esi
  dd 0x90909090 ; ebp
  dd 0x90909090 ; -> esp

  dd 0x90909090 ; ebx
  dd 0x90909090 ; edx
  dd 0x90909090 ; ecx
  dd 0x90909090 ; eax

  dw 0x9090 ; ds
  dw 0x9090 ; es
  dw 0x9090 ; fs
  dw 0x9090 ; gs
  dw 0x9090 ; flags
  dw 0x9090 ; ss
Q:
  mov ax, cs
  mov ss, ax
  mov sp, L
  popad
  pop ds
  pop es
  pop fs
  pop gs
  popf
  pop ss
  mov esp, 0xaaaaaaaa ; esp
  call 0xbbbb:0xcccc ; cs:ip
  mov [cs:L+3*4], esp  
  mov [cs:Q-2], ss
  
  mov sp, cs
  mov ss, sp
  mov sp, Q-2
  pushf
  push gs
  push fs
  push es
  push ds
  pushad
  mov sp, 0xCCCC
  mov ss, sp
  mov sp, 0xDDDD
  jmp 0xAAAA:0xBBBB



  




 
