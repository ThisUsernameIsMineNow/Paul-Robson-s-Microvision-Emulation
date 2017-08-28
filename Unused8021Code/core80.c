//*******************************************************************************************************
//*******************************************************************************************************
//
//      Name:       Core80.C (i8021)
//      Purpose:    CPU Core
//      Author:     Paul Robson
//      Date:       3rd January 2014
//
//*******************************************************************************************************
//*******************************************************************************************************

#include <stdlib.h>
#include <stdio.h>
#include "general.h"
#include "core80.h"
#include "debugsc80.h"


#define TIMER_DIVIDER			(32) 												// Timer is inst cycle/32 frequency
#define CYCLES_PER_SECOND 		(3580000/30)										// Clock Speed / 30 (30 Cycles per Instruction Cycle)
#define FRAMES_PER_SECOND		(30)
#define CYCLES_PER_FRAME 		(CYCLES_PER_SECOND/FRAMES_PER_SECOND)


static unsigned char codeMemory[2048];												// ROM Memory
static int frameCounter = 0;														// Cycle counter for synchronisation.
static int breakPoint = 0xFED;														// Breakpoint

//*******************************************************************************************************
//										Hardware Interface to Microvision 
//*******************************************************************************************************

static int hardwareIO(char operation,int p1,int p2) {
	return 0;
}

#include "code80.h"																	// This is the system generated code 

//*******************************************************************************************************
//											Reset the i8021 Processor
//*******************************************************************************************************

void C80_Reset() {
	i8021reset();																	// Generated code.
	DB80_SetAddress(0x0);															// Set the default display address
}

//*******************************************************************************************************
//								Execute one or more instructions on the i8021
//*******************************************************************************************************

BOOL C80_Execute(int count) {
	if (count < 0) count = CYCLES_PER_FRAME;										// Do a frame if required.
	do {		
		if (timerCount < 0) {														// Divide by 32 timer is out ?
			timerCount += TIMER_DIVIDER;											// Set it to it isn't any more.
			totalElapsedCycles += TIMER_DIVIDER;									// Add to total elapsed cycles
			if (timerEnabled) {														// Is the timer register enabled.
				timerRegister++;													// Bump the timer register
				if (timerRegister == 256) {											// Has it overflowed ?
					timerRegister = 0;												// Reset it to zero
					timerOverflow = 1;												// And set the overflow flag.
				}
			}
		}
		switch(fetch()) {															// Execute the opcode
			#include "opcodes80.h"
		}
		if (frameCounter -- <= 0) { 												// Frame completed ?
			hardwareIO('S',0,0);													// Hardware synchronise
			frameCounter = CYCLES_PER_FRAME;										// Reset counter
		}
		count--;																	// Decrement times till break
	}
	while (count > 0 && pctr != breakPoint);										// Execute until breakpoint or counted out
	return (pctr == breakPoint);													// Return true if at breakpoint.
}

//*******************************************************************************************************
//												Set Breakpoint
//*******************************************************************************************************

void C80_SetBreakPoint(int nbp) {
	breakPoint = nbp;
}

//*******************************************************************************************************
//											  Load a Binary File
//*******************************************************************************************************

void C80_Load(char *binaryFile) {
	FILE *handle = fopen(binaryFile,"rb");
	fread(codeMemory,2048,1,handle);
	fclose(handle);
}

//*******************************************************************************************************
//										Return the Status of the i8021
//*******************************************************************************************************

static C80_STATUS stat;

C80_STATUS *C80_GetStatus(C80_STATUS *s) {
	if (s == NULL) s = &stat;
	return s;
}
