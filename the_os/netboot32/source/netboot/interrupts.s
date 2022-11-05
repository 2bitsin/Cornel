  .section ".text.interrupts", "ax" 
  .global ISR_handler

ISR_prologue_and_epilogue:
  pushl   %esp
  addl    $8,     (%esp)
  pushl   %ebp
  pushl   %eax
  pushl   %ebx
  pushl   %ecx
  pushl   %edx
  pushl   %esi
  pushl   %edi
  pushl   %ds
  pushl   %es
  pushl   %fs
  pushl   %gs
  pushl   %ss
  movl    $0x10,  %eax
  movl    %eax,   %ds
  movl    %eax,   %es
  movl    %eax,   %fs
  movl    %eax,   %gs
  pushl   %esp        
  call    ISR_handler
  addl    $24,    %esp        
  popl    %edi
  popl    %esi
  popl    %edx
  popl    %ecx
  popl    %ebx
  popl    %eax
  popl    %ebp
  popl    %esp  
  iret
  
  .macro ISR_make_entry isr_number, skip_errc=0
  ISR_entry\isr_number :    
    //.global ISR_entry\isr_number
    .if !\skip_errc
      pushl $0
    .endif  
    pushl $\isr_number
    jmp   ISR_prologue_and_epilogue
  .endm

  .macro ISR_emit_table_entry isr_number
  // TODO: figure out how to fix this, otherwise we must assume that ISR entry points reside in the first 64k of memory
    .short ISR_entry\isr_number
    .short 0x0008
    .byte 0x00
    .byte 0x8e
    .short 0x0000
  .endm

  .altmacro

  .macro ISR_make_n_entries x, n, w
    .set Q,x
    .rept \n 
      ISR_make_entry %Q,\w
      .set Q,Q+1
    .endr  
  .endm

  .macro ISR_emit_n_table_entries x, n
    .set Q,x
    .rept \n 
      ISR_emit_table_entry %Q
      .set Q,Q+1
    .endr
  .endm

  ISR_make_n_entries 0,  8, 0 
  ISR_make_n_entries 8,  1, 1 
  ISR_make_n_entries 9,  1, 0 
  ISR_make_n_entries 10, 5, 1 
  ISR_make_n_entries 15, 2, 0 
  ISR_make_n_entries 17, 1, 1 
  ISR_make_n_entries 18, 3, 0 
  ISR_make_n_entries 21, 1, 1
  ISR_make_n_entries 22, 7, 0
  ISR_make_n_entries 29, 2, 1
  ISR_make_n_entries 31, 1, 0
  ISR_make_n_entries 32, 16, 0


  .section ".data.interrupts", "aw"
  
  .global ISR_table_begin
  .global ISR_table_end

ISR_table_begin:
  ISR_emit_n_table_entries 0,48
ISR_table_end:

  .global G_idtr
G_idtr:
  .short ISR_table_end - ISR_table_begin
  .long  ISR_table_begin