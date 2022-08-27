#!/bin/bash
cmd.exe /c 'cd workspace && start ..\\tools\\bin\\bootpd.exe -C config.ini -O v4_bind_address=10.0.1.1' &
cmd.exe /c "C:\Devel\qemu\qemu-system-i386.exe" -S -gdb tcp::1234 -machine pc -serial file:qemu/com1.log -m 512 -vga cirrus -netdev tap,id=n0,ifname=Qemu_Tap -device ne2k_pci,netdev=n0 -option-rom "C:\Devel\qemu\share\pxe-ne2k_pci.rom" &

gdb -ix gdb/init_real_mode.txt            \
  -ex "target remote 192.168.1.1:1234"    \
  -ex "set tdesc filename gdb/target.xml" \
  -ex "set architecture i8086"            \
  -ex "set disassembly-flavor intel"      \
  -ex "break *0x7c00"                     \
  -ex "continue"                          \
  -ex "x /5i 0x7c00"

taskkill.exe /IM qemu-system-i386.exe
taskkill.exe /IM bootpd.exe


