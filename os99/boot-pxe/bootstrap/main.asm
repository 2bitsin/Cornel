    format binary    
    use16

		include "config.asi"
	
    org     LOAD_ADDRESS

    cli

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
		and			si, 	3
		shl			si,		2
		add			si, 	strings.eindex
		call		putstr
		cli
		hlt

		include "console.asi"
		include "mzhead.asi"
		include "execmz.asi"
		include "fakedos.asi"
		include "reboot.asi"		
		include	"strings.asi"
		include "fakepsp.asi"
		align		16
	__payload:


