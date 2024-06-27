    format binary    
    use16

    LOAD_ADDRESS equ 0x7c00

    org     LOAD_ADDRESS
    jmp     preamble

	header:
		.version db "BOOT-PXE v0.1 (BUILD 2024-06-26)", 13, 10, 0		

	strings:
		.int20h  db "int 0x20", 13, 10, 0
		.int21h  db "int 0x21", 13, 10, 0

		align 16			

	include   "console.asi"
			
	preamble:

		mov		 	ax, 	cs
		mov 		ds,   ax
		mov     es, 	ax

		xor			ax, 	ax
		mov     fs, 	ax
		mov     gs, 	ax
	
		mov     ax, 	0x0500
		mov     ss, 	ax
		mov     sp, 	0x7700		

		call		coninit

		mov     si,   header.version

		call		putstr

; Setup INT 0x20
		mov			ax,		cs
		mov			[fs:0x20*4 + 0], word __dos_20h
		mov			[fs:0x20*4 + 2], ax

; Setup INT 0x21 
		mov			[fs:0x21*4 + 0], word __dos_21h
		mov			[fs:0x21*4 + 2], ax
		

		cli
		hlt

; Dummy INT 0x20
	__dos_20h:

		pushad 
		push 		ds
		push 		ss
		push 		es
		push 		fs
		push 		gs
		
		mov 		ax, 	cs
		mov 		ds, 	ax
		mov 		es, 	ax

		mov 		si, 	strings.int20h
		call 		putstr

		xchg 		bx, 	bx

		pop 		gs
		pop 		fs
		pop 		es
		pop 		ss
		pop 		ds
		popad

		iret

; Dummy INT 0x21
	__dos_21h:

		pushad 
		push 		ds
		push 		ss
		push 		es
		push 		fs
		push 		gs
		
		mov 		ax, 	cs
		mov 		ds, 	ax
		mov 		es, 	ax

		mov 		si, 	strings.int21h
		call 		putstr

		xchg 		bx, 	bx

		pop 		gs
		pop 		fs
		pop 		es
		pop 		ss
		pop 		ds
		popad

		iret


	PADSIZE   equ   512
	PADDING   equ 	times ((PADSIZE - (($ - $$) mod PADSIZE)) - 2)
	PADDING 	db    0
		        dw    0xAA55
	

	__payload:


