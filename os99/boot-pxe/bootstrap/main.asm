    format binary    
    use16

    LOAD_ADDRESS 	equ 0x7c00
		INT21H_TABLE	equ (LOAD_ADDRESS - (0x80*4))
		STACK_BASE		equ 0x0500
		STACK_TOP			equ (INT21H_TABLE - STACK_BASE) 
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
	
		mov     ax, 	STACK_BASE/16
		mov     ss, 	ax
		mov     sp,   STACK_TOP

		call		coninit
		call		dosinit
		jmp			execute_mz

	__selfcheck_fail:		
		mov			si, 	0		

	__halt:
		mov			si,		cs
		mov			ds,		si
		and			si,		bx
		and			si, 	7
		shl			si,		2
		add			si, 	strings.eindex
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


