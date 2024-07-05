    format binary    
    use16

		include "config.asi"
		include	"errorid.asi"
	
    org     LOAD_ADDRESS

    cli
		mov		 	ax, 		cs
		mov 		ds,   	ax
		mov     es, 		ax

		xor			ax, 		ax
		mov     fs, 		ax
		mov     gs, 		ax

		mov     ax, 		STACK_BASE/16
		mov     ss, 		ax
		mov     sp,   	STACK_TOP

		call		coninit
		call		dosinit
		call 		flatinit
		call		a20test
		or			ax,			ax
		jz		  .a20good
		call		a20enable	
		call		a20test
		or			ax,			ax
		mov			si,			4
		jnz			__halt

	.a20good:
	
		jmp			execute_mz

	__selfcheck_fail:		
		mov			si, 		CORRUPTED_MZ_HEADER
	
	__halt:	
		mov			si,			cs
		mov			ds,			si
		and			si,			bx
		and			si, 		7
		shl			si,			2
		add			si, 		__PSP.errstr
		call		putstr
		cli
		hlt
				
		include "console.asi"
		include "mzhead.asi"
		include "execmz.asi"
		include "fakedos.asi"
		include "reboot.asi"		
		include "a20line.asi"
		include	"flatreal.asi"
		include "desctbl.asi"
		include "fakepsp.asi"
		align		16
	__payload:


