    format binary    
    use16
    
		include "config.asi"
		org     LOAD_ADDRESS		
		include	"errorid.asi"	
		include "main.asi"
		include "console.asi"
		include "serial.asi"
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


