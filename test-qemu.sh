#!/bin/bash
#ndisasm -b 16 workspace/coloader.bin

cmd.exe /c "C:\Devel\qemu\qemu-system-i386.exe" -machine isapc -serial file:qemu/com1.log -cdrom ./build/cornel.iso -m 512 -vga cirrus -boot d