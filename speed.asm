
//
// a line
//
start
	ldx		0
nextdigit
	tcy 	0										// point to start
carryforward 									 								
	imac 											// increment and load digit
	tam 											// write it back
	ac6ac 											// add 6
	br		carryout 								// if carry set, then carry out
	br 		nextdigit 								// else start again.
carryout
	cla 											// clear the current one
	tam 
	iyc 											// increment Y
	cla 											// in case of carry out, unlikely !
	br 		carryforward

