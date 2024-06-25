    format binary    
    use16

    LOAD_ADDRESS equ 0x7c00

    org     LOAD_ADDRESS
    jmp     preamble

	header:
		.version db "BOOT-PXE v0.1", 0		

		align 16		
	strings:
		.hello   db "Hello, World!", 0

	include   "console.asi"
			
	preamble:

		mov		 	ax, 	cs
		mov     ds, 	ax
		mov     es, 	ax
		mov     fs, 	ax
		mov     gs, 	ax

		mov     ax, 	0x0500
		mov     ss, 	ax
		mov     sp, 	0x7700

		call		coninit

		mov     si,   strings.hello

		call		putstr
		cli
		hlt

	PADDING   equ 	times ((512 - (($ - $$) mod 512)) - 2)
	PADDING 	db    0
		        dw    0xAA55
	




