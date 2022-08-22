OUTPUT_FORMAT(binary)
ENTRY(Netboot_main)
SECTIONS
{
  . = 0x7c00;

  .preamble : 
  {
    _PREAMBLE_BEGIN_ = .;
    *preamble.s.o(.text)
    _PREAMBLE_END_ = .;
  }

  .Netboot_main :
  {
    _TEXT_BEGIN_ = .;
    *netmain.cpp.o(.text)
  }

  .text :
  {
    *(.text)
    _TEXT_END_ = .;
  }

  .data :
  {
    _DATA_BEGIN_ = .;
    *(.data)
    *(.rodata)
    _DATA_END_ = .;
  }

  .bss :
  {
    _BSS_BEGIN_ = .;
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
