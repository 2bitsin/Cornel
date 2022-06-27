#!/bin/bash
cmd.exe /c "C:\Devel\qemu\qemu-system-i386.exe" -S -gdb tcp::1234 -machine isapc -cdrom ./build/image.iso -m 512 -vga cirrus -boot d &