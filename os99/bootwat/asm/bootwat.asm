    format binary    
    use16
    
		include "config.asi"
		include	"errorid.asi"	
				
		jmp			short __start
		align		2
	__stack:	dd			0
	__entry:  dd			0
	__start:

		include "main.asi"
		include "console.asi"
		include "serial.asi"
		include "fakedos.asi"
		include "a20line.asi"
		include	"flatreal.asi"
		include "desctbl.asi"
		include "fakepsp.asi"
		align		16
	__payload:


