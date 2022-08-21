OUTPUT_FORMAT(binary)
ENTRY(NET_main)
SECTIONS
{
  . = 0x7c00;

  .preamble : 
  {
    *preamble.s.o(.text)
  }

  .netmain :
  {
    *netmain.c.o(.text)
  }

  .text :
  {
    _TEXT_START_ = .;
    *(.text)
    _TEXT_END_ = .;
  }

  .data :
  {
    _DATA_START_ = .;
    *(.data)
    *(.rodata)
    _DATA_END_ = .;
  }

  .bss :
  {
    _BSS_START_ = .;
    *(.bss)
    _BSS_END_ = .;
  }

  /DISCARD/ : 
  {    
    *(.eh_frame)
    *(.comment)
    *(.note.GNU-stack)
  }
}
