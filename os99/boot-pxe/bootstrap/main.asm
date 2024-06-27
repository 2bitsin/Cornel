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

		xchg		bx,		bx
	.patch_reloc:
		
	; Compute the segment to be patched
		mov			ax,		[MZ_SEGMENT:bx + 2]
		add     ax,		__payload / 16
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
		
		mov			ax,		__PSP / 16
		mov			ds,		ax
		mov			es, 	ax
		
		mov			ax, 	MZ_INITIALSS
		add			ax,		__payload / 16
		mov			ss,		ax
		mov			sp,		MZ_INITIALSP

	; RETF to initial CS:IP
		mov			ax, 	MZ_INITIALCS
		add			ax, 	__payload / 16
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
		xchg		bx,		bx
		mov			si, 	strings.corrupted
		call		putstr

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

		align 	256
	__PSP:
		.exit_vector:	
			dw		0
		.mem_top_seg:
			dw		0
		; reserved
			db		0
		; ??????
			db    0
		.com_seg_size:
			dw 		0
		; ?????
			dw		0
		.exit_addr:
			dd		0
		.cbrk_addr:
			dd 		0
		.cerr_addr:
			dd		0
		.parent_psp_seg:
			dw		0
		.job_file_tbl:
			times 20 db 0
		.env_seg:
			dw 		0
		.temp_ss_sp:
			dd		0
		.jft_size:
			dw		0
		.jft_pointer:
			dd		0
		.prev_psp_ptr:
			dd		0
		; reserved
			dd		0
		.dos_version:
			dw		0x610
		; reserved
			times 14 db 0
		.call_far_dos:
			int		0x21
			retf
		; reserved
			dw 		0
		; reserved, fcb?
			dd		0
			dw 		0
			db		0
		.fcbs_1:
			times	16 db 0
		.fcbs_2:
			times 20 db 0

		.cmdline_size:
			db		0
		.cmdline_str:
			times 127 db 0xd

	PADSIZE   equ   512
	PADDING   equ 	times ((PADSIZE - (($ - $$) mod PADSIZE)))
	PADDING 	db    1	
		align		16
	__payload:


