// ***********************************************************************************************************************************************
// ***********************************************************************************************************************************************
//
//							BOMBER : Implementation of classic down the screen game for MB Microvision
//
// 	Written by Paul Robson January 2014.
// ***********************************************************************************************************************************************
// ***********************************************************************************************************************************************
//
//	Bank 0 : System Code Memory
//
//	Bank 1 : Height of the buildings (0-7) x 16 columns, actual physical height
//	
//	Bank 2 : 	0 					Y position of Plane 
//			 	1 					X position of Plane
// 				2 					Plane speed
//				3 					Plane speed counter
//
//			 	4 					Y position of Bomb ($F = don't draw)
//				5 					X position of Bomb
//				6 					Bomb Speed
// 				7 					Bomb Speed Counter
//
//				8 					Skill Level.
//				9 					Column being rendered.
// 				10 					Temporary value
//				11 					Win-Lose Flag
//
//				12-15 				Column Data (12 = top column) creating graphic for each row.

//
//	Bank 3 :	0-11 				Tune space.
//				12-15 				Bit Patterns for 0,1,2,3 heights.
//
//  Bank 4-7 : 	0-6 				Score, Score Graphics.
// 									(1 point for each block dropped)
//

	include 	system.asm 										// Include System Library

// ***********************************************************************************************************************************************
//
//											New Game : Reset Level and Score, Create Lookup Table
//
// ***********************************************************************************************************************************************

ResetGame
	ldx 	2 													// Set the level to 1
	tcy 	8 
	tcmiy 	1

	ldx 	3 				
	tcy 	12 													// Set up the height bars for each column
	tcmiy 	0 													// Bits are upside down remember :)
	tcmiy 	8
	tcmiy 	12
	tcmiy 	14

	lcall 	OSClearDisplaySpace 								// Clear the display space, which resets the score.

// ***********************************************************************************************************************************************
//
//															New Game Level
//
// ***********************************************************************************************************************************************

NextLevel 
	ldx 	2 													// Update the current level.
	tcy 	8 
	imac
	tam

	tcy 	0 													// Set X and Y position to zero
	tcmiy 	0
	tcmiy 	0
	tam 														// Store plane speed in offsest 2

	tcy 	4 													// Set Bomb X and Y position to zero,15 meaning don't draw yet
	tcmiy 	15
	tcmiy 	15
	tcmiy 	10   												// Default speed to 10

	tcy 	HWKeyMiddleCol 										// Set the middle key column line, only use one key 'bomb'
	setr

	tcy 	15 													// Generate the bar heights.
ResetGameSpace
	lcall 	OSRandomNumber 										// Random number 0-15
	ldx 	1 													// Bank 1 holds the bar heights
	tam 														// Store in memory
	rbit 	3 													// Make the number 0-7
	dman 														// Load and Decrement. S will be clear if was zero
	br 		RGSNotZero  										// A will be 15,0-6, skip if S set e.g. not 15
	cla  														// A to zero if 15, gives us 0,0,1,2,3,4,5,6 as heights
RGSNotZero
	tamdyn 														// Write back decrement Y
	br 		ResetGameSpace 										// Do for all of 16 columns.

// ***********************************************************************************************************************************************
//
//														Main Game Screen Rendering Loop
//
// ***********************************************************************************************************************************************

MainRenderLoop
	lcall 	OSSwitchPolarity		
	ldx 	2 													// Reset the current column being rendered to zero.
	tcy 	9
	tcmiy 	0 		
	lbr 	RenderColumn

	page

RenderColumn
	ldx 	2 													// Point to the data row
	tcy 	12 													// Clear the column buffer
	tcmiy 	0
	tcmiy 	0
	tcmiy 	0
	tcmiy 	0
	tcy 	9 													// Point to the current column
	tmy 														// Read the current column
	ldx 	1 													// Read the height of the current column.
	tma 														// into A
	a12aac 														// Will set S if > 4
	call 	SetLowerColumn 										// Only set if > 4 call set lower column

	tay
	ldx 	3 								
	tma 														// Read the bit pattern into A.
	ldx 	2 													// Point to the bottom byte of the column
	tcy 	15 	
	mnez 														// If non-zero, need to use 14 not 15 as there's a bar below
	call 	DecrementY	
	tam 														// Store the bit pattern in the lower entry.

	ldx 	2 													// Draw pixel at (2,0) (e.g. the bomber)
	tcy 	0 													
	lcall 	DrawPixelAlways

	ldx 	2 													// Draw pixel at (2,4) (e.g. the bomb)
	tcy 	4
	lcall 	DrawPixel

	ldx 	2 													// Now start to output the 4 bits of data
	tcy 	12
OutputColumnPixels
	tma 														// Read it 
	tdo 														// Send to LCD out
	tya 														// Save Y in A
	tcy 	HWNotDataClock 										// Pulse ! DataClock
	rstr
	setr
	tay 														// Restore Y
	iyc 														// Bump it
	ynec 	0 													// If not zero then haven't finished
	br 		OutputColumnPixels 									// Output the column pixels.
	tcy 	9 													// XY now points to M(2,9) which is the column number
	tmy 														// Read it
	ldx 	0 													// Use Page 0 for write bits (not mandatory)
	lcall 	OSWriteBitsY 										// Output the column data
	lcall 	OSUpdateDisplay 									// Update the latches.

	ldx 	2 													// Point to column number and increment
	tcy 	9	
	imac 														// Bump it
	tam 
	ldp 	RenderColumn 										// Loop round to render next column if nonzero
	mnez
	br 		RenderColumn

	lbr 	CheckMoving

DecrementY
	dyn
	retn

SetLowerColumn
	ldx 	2 													// Solid block in bottom column
	tcy 	15
	tcmiy 	15
	a12aac 														// Unpicks the a4aac following the call so it's 0-3
	retn

	page

// ***********************************************************************************************************************************************
//
//					(2,Y) points to a (Y,X) pixel position. Draw to buffer at (2,12-15). Do not draw if Y = $0F
//
// ***********************************************************************************************************************************************

DrawPixelAlways
	tya
	tcy 	10
	tam
	tay
	br 		_DPMain

DrawPixel
	tya 														// Y->A 
	tcy 	10 													// Save Y in (2,10)
	tam
	tay 														// Fix Y back up to point to the Y coordinate.
	imac 														// Load the current Y into A and increment, will cause S if 15 (don't draw)
	br 		DPExit 												// If not drawing then exit.
_DPMain
	iyc 														// Point to X (column position)
	tma 														// Load column position into A
	tcy 	9 													// Point M(X,Y) to (2,9), the actual column position.
	mnea 														// If they are different.
	br 		DPExit 												// Exit without drawing.
	tcy 	10 													// Reread Y from (2,10) into Y
	tmy
	tma 														// Read vertical position into A
	tcy 	11 													// Where it goes to (1,11) onwards
DPFindNibble
	iyc
	a12aac 														// Will cause S *if* >= 4 0-3 will return 12,13,14,15 for A 0-3
	br 		DPFindNibble 

	iac 	 													// Set appropriate bit depending on A 
	br 		DPSet3
	iac
	br 		DPSet2
	iac 	
	br 		DPSet1
	sbit 	0
	retn
DPSet3
	sbit 	3
	retn
DPSet2
	sbit 	2
	retn
DPSet1
	sbit 	1
DPExit
	retn

// ***********************************************************************************************************************************************
//
//								(2,Y) points to a speed/counter pairing. Returns Y != 0 if time to move, 0 otherwise.
//	
// ***********************************************************************************************************************************************

SpeedCheck
	ldx 	2 													// Check in page 2
	tma 														// Read speed
	iyc 														// Point to counter
	amaac 														// add speed to counter
	br 		SCTimeout 											// If true it's move time.
	tam 														// write counter back
RetnY0
	tcy 	0 													// Return Y = 0
	retn
SCTimeout
	tam 														// write back
RetnY1
	tcy 	1 													// Return Y = 1
	retn 

// ***********************************************************************************************************************************************
//	
//												Has the object at (X,Y) collided with the buildings ?
//
// ***********************************************************************************************************************************************

HeightCheck
	tma 														// Read the height into A
	iyc 														// Point to X
	tmy 														// Read horizontal position into Y
	ldx 	1 													// Point to the height table
	amaac 														// Add height to vertical height
	br 		RetnY1 												// If S then reached the collision point
	br 		RetnY0 												// otherwise no collision

	page

// ***********************************************************************************************************************************************
//
//															Check if objects are moving.
//
// ***********************************************************************************************************************************************

CheckMoving
	tcy 	6 													// Check if bomb is moving
	lcall 	SpeedCheck 											// Bump the counter
	ynec 	0 													// Is it moving
	br 		CMMoveBomb 											// If so move the bomb.
CMExit1
	tcy 	2 													// Check if the bomber is moving.
	lcall 	SpeedCheck
	ldp 	CMMoveBomber
	ynec 	0
	br 		CMMoveBomber
CMExit2
	ldx 	2 													// Check if bomb is live (e.g. != 15)
	tcy 	4 
	ldp 	CheckDropBomb
	imac
	br 		CheckDropBomb 										// If it is $F check dropping.
CDBExit
	lbr 	MainRenderLoop

// ***********************************************************************************************************************************************
//																	Move the Bomb
// ***********************************************************************************************************************************************

CMMoveBomb
	tcy 	4 													// Point to Y
	imac 														// Bump Y position
	br 		CMExit1 											// If was 15 (not in use) S will be set so don't move.  
	tam  
	lcall 	HeightCheck 										// Check if collided with building top.
	ynec 	1 													// If Y != 1 (Collision)
	br 		CMExit1 											// End of moving bomb

	ldx 	2 													// Move to vertical position
	tcy 	4 
	tcmiy 	15 													// Disable bomb and point at X position
	tmy 														// Put in Y
	ldx 	1 													// Point to height entry in the table
	tcmiy 	0 													// Write 0 there, e.g. building gone.

	ldx 	0 													// Point to sound timer
	tcy 	OSDSoundTimer
	tcmiy 	5 													// Play bomb sound.
	tcmiy 	2

	lbr 	OSIncrementOne  									// Bump score, then come back to the label below.

OSExternalIncrementExit
	ldx 	1
	tcy 	15
CheckAllFlattened
	mnez
	br 		CMExit1
	dyn
	br 		CheckAllFlattened
	ldx 	2
	tcy 	2
	tcmiy 	15
	br 		CMExit1 											// End of Ship Move.

// ***********************************************************************************************************************************************
//																Move the Bomber
// ***********************************************************************************************************************************************

	page

CMMoveBomber
	tcy 	1 													// Point to X
	imac 														// Bump X and write back
	tam
	dan 														// S will be set unless zero
	br 		CMMBCheckHeight 									// if non-zero, then check height.
	tcy 	0 													// looped round, go down one.
	imac 														// Bump the vertical position.
	br 		WonGame 											// If carried over, then level is complete.
	tam
CMMBCheckHeight
	tcy 	0
	lcall 	HeightCheck 										// Check for crash.
	ldp 	CMExit2
	ynec 	1 													// If returned 0
	br 		CMExit2 											// end of turn, no crash.

																// Crash occurred, game over.
LostGame 	
	ldx 	0 													// End game sound.
	tcy 	OSDSoundTimer
	tcmiy 	15
	tcmiy 	2
	tcy 	0 													// Win-lose flag will be 0 (lost)
	lbr  	WonGame2
WonGame
	ldx 	0 													// Beat level sound.
	tcy 	OSDSoundTimer
	tcmiy 	15
	tcmiy 	7
	tcy 	1 													// Win-lose flag will be 1 (won)
WonGame2
	tya 														// Flag in A.
	ldx 	2 													// Save win-lose flag.
	tcy 	11
	tam
	lbr 	RenderScore

// ***********************************************************************************************************************************************
//															Check if bomb should be dropped
// ***********************************************************************************************************************************************
	
CheckDropBomb
	knez 														// Drop Bomb if key pressed
	br 		DropBomb
	lbr		CDBExit
DropBomb
	tcy 	0 													// Copy Bomber X+Y to Bomb X+Y
	tma
	tcy 	4
	tam
	tcy 	1
	tma
	tcy 	5
	tam
	ldx 	0 													// Point to sound timer
	tcy 	OSDSoundTimer
	tcmiy 	2													// Play bomb drop sound.
	tcmiy 	6
	lbr 	CDBExit

// ***********************************************************************************************************************************************
//										Level completed, or died (2,11) is '1' if the player compleeted the level.
// ***********************************************************************************************************************************************

	page

RenderScore
	tcy 	HWKeyMiddleCol
	rstr
	tcy 	HWKeyLeftCol										// Select the left column which has the continue button.
	setr
	tcy 	5 													// Render the score at row 5 e.g. basically in the middle
	lbr 	OSUpdateAllAndRenderTextDisplay
OSExternalRenderTextExit 										// And return here.
	knez 														// Is a key pressed ?
	br 		EndScore
	br 		RenderScore
EndScore
	tcy 	HWKeyLeftCol										// Select the left column which has the continue button.
	rstr
	ldx 	2 													// Look at win-lose
	tcy 	11
	ldp		NextLevel 											// If win-lose set then next level
	mnez
	br 		NextLevel
	br 		ResetGame 											// otherwise restart.

