
	lcall 	OSClearDisplaySpace

	ldx 	0
	tcy 	OSDSoundTimer
	tcmiy 	14
	tcmiy 	7

NewFrame
	lcall 	OSRandomNumber
	tcy 	9
	lbr 	OSUpdateAllAndRenderTextDisplay
OSExternalRenderTextExit
	lbr		OSIncrementOne
OSExternalIncrementExit
	br 		NewFrame
