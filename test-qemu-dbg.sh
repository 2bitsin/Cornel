#!/bin/bash
#ndisasm -b 16 workspace/coloader.bin

cmd.exe /c "C:\Devel\qemu\qemu-system-i386.exe" -S -gdb tcp::1234 -machine isapc -serial file:qemu/com1.log -cdrom ./build/cornel.iso -m 512 -vga cirrus -boot d &

gdb -ix gdb/init_real_mode.txt            \
  -ex "target remote 192.168.1.1:1234"    \
  -ex "set tdesc filename gdb/target.xml" \
  -ex "set architecture i8086"            \
  -ex "set disassembly-flavor intel"      \
  -ex "break *0x8000"                     \
  -ex "continue"                          \
  -ex "x /5i 0x8000"
taskkill.exe /IM qemu-system-i386.exe

