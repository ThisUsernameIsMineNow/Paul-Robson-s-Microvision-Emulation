//*******************************************************************************************************
//*******************************************************************************************************
//
//      Name:       System.C
//      Purpose:    Emulator Control
//      Author:     Paul Robson
//      Date:       2nd January 2014
//
//*******************************************************************************************************
//*******************************************************************************************************

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "general.h"
#include "hardware.h"
#include "core11.h"
#include "system.h"
#include "hwinterface.h"
#include "debugsc11.h"

static BOOL inDebugMode = FALSE;
static char lastKey = '\0';

//*******************************************************************************************************
//										Process a key stroke
//*******************************************************************************************************

static void processKey(char command) {
	switch(command) {

		case 'K':																				// (K) Set Break Point
			C11_SetBreakPoint(DB11_GetAddress());
			break;

		case 'H':																				// (H) Go Home
			DB11_SetAddress(C11_GetStatus(NULL)->pctr);
			break;

		case 'S':																				// (S) Single Step
			C11_Execute(1);																		// One Instruction
			DB11_SetAddress(C11_GetStatus(NULL)->pctr);
			break;

		case 'V':																				// (V) Step Over.
			C11_SetBreakPoint(C11_GetStatus(NULL)->pctr+1); 									// Break after next instruction.
			inDebugMode = FALSE;
			break;

		case 'G':																				// (G) Go to Breakpoint or Monitor
			inDebugMode = FALSE;
			break;

		default:																				// Check 0-9, A-F
			if (isxdigit(command)) {
				DB11_UpdateAddress(isdigit(command) ? command - '0' : command - 'A' + 10);		// update the displayed address
			}
			break;
	}
}

//*******************************************************************************************************
//											Process a Frame
//*******************************************************************************************************

BOOL SYS_Process() {
	if (!inDebugMode) {																			// In run mode
		inDebugMode = C11_Execute(-1);															// Do a single frame.
		if (IF_KeyPressed('M')) inDebugMode = TRUE;												// If (M) Monitor pressed the Break to Debug Mode.
		if (inDebugMode) DB11_SetAddress(C11_GetStatus(NULL)->pctr);
	} else {
		XHWISetPitch(0);																		// Sound off.
		char currentKey = '\0';
		for (char c = '0';c <= 'Z';c++) if (IF_KeyPressed(c)) currentKey = c;					// Figure out what is currently pressed
		if (currentKey != lastKey) {															// Key changed ?
			if (currentKey == '\0') processKey(lastKey);										// If Key-release then do key released.
			lastKey = currentKey;
		}
	}
	return inDebugMode;
}

void SYS_SetMode(BOOL debugMode) {
	inDebugMode = debugMode;
}