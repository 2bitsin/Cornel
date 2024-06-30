    format binary    
    use16

    LOAD_ADDRESS 	equ 0x7c00
		MZ_SEGMENT		equ gs
	
    org     LOAD_ADDRESS
    jmp     preamble

		include	"strings.asi"

		align 16			
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
		call		dosinit
		jmp			execute_mz

	__selfcheck_fail:		
		mov			si, 	strings.corrupted
		call		putstr

	__halt:
		mov			si,		cs
		mov			ds,		si
		and			bx,		3
		shl			bx,		1
		mov			si, 	[cs:strings.error_tbl+bx]
		call		putstr
		cli
		hlt

		include "console.asi"
		include "mzhead.asi"
		include "execmz.asi"
		include "fakeenv.asi"		
		include "fakedos.asi"		
		include "fakepsp.asi"
		align		16
	__payload:


