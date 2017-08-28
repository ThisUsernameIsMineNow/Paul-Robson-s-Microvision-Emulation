//
//
//	
	include 	system.asm

OSExternalRenderTextExit
OSExternalIncrementExit


	comc
	lcall		Bank1
	comc
stop
	br 	stop

	page 	16
Bank1
	tcy 	10
Loop1
	dyn
	br 		Loop1
	retn


