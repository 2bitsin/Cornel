#!/bin/bash
cmd.exe /c 'cd workspace && start ..\\tools\\bin\\bootpd.exe -C config.ini -O v4_bind_address=10.0.1.1' &
cmd.exe /c "C:\Devel\qemu\qemu-system-x86_64.exe" -machine pc -serial file:qemu/com1.log -m 2048 -vga qxl -netdev tap,id=n0,ifname=NordVPNTapAdapter -device ne2k_pci,netdev=n0 -option-rom "C:\Devel\qemu\share\pxe-ne2k_pci.rom"
taskkill.exe /IM bootpd.exe

