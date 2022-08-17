rm -rf workspace/*
cp source/config.ini workspace/
cp build/coreload.sys workspace/
cmd.exe /c "C:\Devel\qemu\qemu-system-x86_64.exe" -machine pc -serial telnet:127.0.0.1:4321,server,nowait -m 512 -vga cirrus -netdev tap,id=n0,ifname=Qemu_Tap -device ne2k_pci,netdev=n0 -option-rom "C:\Devel\qemu\share\pxe-ne2k_pci.rom" &
cmd.exe /c 'cd workspace && start ../tools/bin/bootpd.exe -C config.ini -O v4_bind_address=10.0.1.1'

