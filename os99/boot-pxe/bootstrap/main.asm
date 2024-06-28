    format binary    
    use16

    LOAD_ADDRESS 	equ 0x7c00
		MZ_SEGMENT		equ gs

		include "mzhead.asi"

    org     LOAD_ADDRESS
    jmp     preamble

	header:
		.version:
			db 		"BOOT-PXE v0.1", 13, 10, 0		

	strings:
		.int20h:
			db 		"int 0x20", 13, 10, 0
		.int21h:
			db 		"int 0x21", 13, 10, 0
		.corrupted:
			db		"Bootloader corrupted, ", 13, 10
			db 		"system halted.", 13, 10, 0

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

; Setup INT 0x20
		mov			ax,		cs
		mov			[fs:0x20*4 + 0], word __dos_20h
		mov			[fs:0x20*4 + 2], ax

; Setup INT 0x21 
		mov			[fs:0x21*4 + 0], word __dos_21h
		mov			[fs:0x21*4 + 2], ax

; Parse MZ header

		mov 		ax, 	__payload
		shr			ax, 	4
		mov			gs, 	ax

		mov			ax,   MZ_SIGNATURE
		cmp			ax, 	0x5A4D
		jnz			.corrupted

		mov			bx, 	MZ_RELOCOFFS		
		mov			cx, 	MZ_RELOCSIZE
		
	.patch_reloc:
		
	; Compute the segment to be patched
		mov			ax,		[MZ_SEGMENT:bx + 2]
		add     ax,		__payload / 16
		add			ax,		MZ_HEADERLEN
		mov			fs, 	ax

	; Get the offset to be patched
		push		bx
		mov			bx,		[MZ_SEGMENT:bx]

	; Patch the relocation
		mov			ax, 	[fs:bx]
		add			ax,		word __payload / 16
		mov			[fs:bx],ax

	; Restore bx
		pop			bx
		add			bx,		2
		dec			cx
		jnz			.patch_reloc

	; Done patching, setup environment

		int			0x12
		shl			ax, 	6
		mov			[__PSP.mem_top_seg], ax
		
		mov			ax,		__PSP / 16
		mov			ds,		ax
		mov			es, 	ax
		
		mov			ax, 	MZ_INITIALSS
		add			ax,		__payload / 16
		add 		ax, 	MZ_HEADERLEN
		mov			ss,		ax
		mov			sp,		MZ_INITIALSP

	; RETF to initial CS:IP
		mov			ax, 	MZ_INITIALCS
		add			ax, 	__payload / 16
		add			ax,		MZ_HEADERLEN
		push		ax
		mov			ax, 	MZ_INITIALIP
		push		ax
		mov			ax,		0xffff
		retf		

	; .....
		mov     si,   header.version
		call		putstr
		cli
		hlt

	.corrupted:
		
		mov			si, 	strings.corrupted
		call		putstr

	__halt:
		xchg		bx,		bx
		cli
		hlt


; ---------------------------
; DOS Get Version
; ---------------------------

	__dos_getver:
		pop			bx		
		mov			bx,		0xfd2b
		or      al, 	al
		jz			.novflags
		xor			bh,		bh
	.novflags:
		mov			ax, 	0x1606
		iret

; ---------------------------
;	DOS function dispatch table
; ---------------------------
	align 16
	__dos_tbl:
		times 0x30 dw __halt
		dw __dos_getver
		times 0x4F dw __halt

; Dummy INT 0x20
	__dos_20h:
		jmp __halt		

; Dummy INT 0x21
	__dos_21h:
		xchg		bx,		bx
		test		ax,		0x80
		jnz			__halt

		push 		bx
		xor			bh, 	bh
		mov			bl, 	ah		
		shl			bx,		1
		jmp			near [cs:__dos_tbl+bx]

; Terminate handler
	__dos_term:
		xchg		bx,  	bx
		retf

; Ctrl+Brk handler
	__dos_cbrk:
		xchg		bx,  	bx
		retf

; Critical error handler
	__dos_cerr:
		xchg		bx,  	bx
		retf				

; --------------------------
;  Dummy PSP
; --------------------------
		align 	16
	__PSP:
		.exit_vector:	 		; 2 bytes 
			int		0x20    			       ; 2
		.mem_top_seg:  		; 2 bytes
			dw		0			               ; 4
		.fill1:        		; 1 byte
			db		0                    ; 5
		.ps_reentry:   		; 5 bytes
			call far 0x0000:0x00C0		 ; 10
		.exit_addr:		 		; 4 bytes
			dw    __dos_term, LOAD_ADDRESS / 16 ; 14
		.cbrk_addr:		 		; 4 bytes  
			dw 		__dos_cbrk, LOAD_ADDRESS / 16 ; 18
		.cerr_addr:    		; 4 bytes
			dw		__dos_cerr, LOAD_ADDRESS / 16 ; 22
		.parent_psp_seg:	; 2 bytes
			dw		0                    	; 24
		.file_tab:				; 20 bytes
			times 20 db 0              	; 44
		.env_seg:					; 2 bytes
			dw 		0                    	; 46
		.temp_ss_sp:			; 4 bytes
			dd		0                    	; 50
		.max_files:				; 2 bytes
			dw		20                  	; 52
		.file_tab_ptr:		; 4 bytes
			dw		__PSP.file_tab, LOAD_ADDRESS / 16	; 56
		.prev_psp_ptr:    ; 4 bytes
			dw		0xffff, 0xffff       	; 60
		; reserved        ; 4 bytes
			dd		0                    	; 64
		.dos_version:     ; 2 bytes
			dw		0x610                	; 66
		.pdb_next:				; 2 bytes
			dw		0	                   	; 68		
		; reserved				; 4 bytes
			dd    0										 	; 72
		; reserved				; 1 bytes
		  db    0										 	; 73
		; reserved 				; 7 bytes
			times 7 db 0								; 80
		.ps_unix:					; 3 bytes
			int 	0x21									; 83
			retf					
		; reserved				; 9 bytes
			times 9 db 0								; 92		
		.first_fcb:				; 16 bytes
			times 16 db 0								; 108
		.second_fcb:			; 16 bytes
			times 16 db 0								; 124
		; reserved				; 4 bytes
			dd		0											; 128
		.cmdline_size:		; 1 bytes
			db		0											; 129
		.cmdline_str:			; 127 bytes
			times 127 db 0xd						; 256

		.end_of_psp:
	assert ((.end_of_psp - __PSP) = 256)

	PADSIZE   equ   512
	PADDING   equ 	times ((PADSIZE - (($ - $$) mod PADSIZE)))
	PADDING 	db    1	
		align		16
	__payload:


