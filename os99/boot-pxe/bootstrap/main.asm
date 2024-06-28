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
		xchg		bx,		bx
		cli
		hlt

		include "mzhead.asi"
		include "console.asi"
		include "execmz.asi"
		include "fakedos.asi"
		include "fakepsp.asi"

	PADSIZE   equ   512
	PADDING   equ 	times ((PADSIZE - (($ - $$) mod PADSIZE)))
	PADDING 	db    1	
		align		16
	__payload:


