//*******************************************************************************************************
//*******************************************************************************************************
//
//      Name:       Core11.C (TMS1100)
//      Purpose:    CPU Core
//      Author:     Paul Robson
//      Date:       2nd January 2014
//
//*******************************************************************************************************
//*******************************************************************************************************

#include <stdlib.h>
#include <stdio.h>
#include "general.h"
#include "core11.h"
#include "debugsc11.h"
#include "hwinterface.h"
											
#define CYCLES_PER_SECOND 		(cycleSpeed)										// Clock Frequency / 6 (24k Res, 100nf Cap)
#define FRAMES_PER_SECOND		(30) 												// No of renderings per second
#define CYCLES_PER_FRAME 		(CYCLES_PER_SECOND/FRAMES_PER_SECOND) 				// Cycles between renderings.

#ifndef ARDUINO
static BYTE8 codeMemory[2048];														// ROM Memory
#else 
#include "rom.h"																	// Arduino Version (PROGMEM)
#endif

static int frameCounter = 0;														// Cycle counter for synchronisation.
static int breakPoint = 0xFED;														// Breakpoint
static int cycleCounter = 0;														// Cycle counter
static int fileCheckSum = -1; 														// ROM Check Sum
static LONG32 cycleSpeed = 500000/6;												// Run Speed.										
static BOOL rotaryControlEnabled = FALSE;											// TRUE if using rotary control.

static int hardwareIO(char operation,int p1,int p2);

#include "code11.h"																	// This is the system generated code 

#define PCTR   ((ca << 10) | (pa << 6) | pc)										// 11 Bit program pointer.

//*******************************************************************************************************
//											Reset the TMS1100 Processor
//*******************************************************************************************************

void C11_Reset() {
	int i;
	cycleCounter = codeMemory[0];													// Is this an optimising bug ?????? without this Windows zeros codeMemory[0]
	tms1100reset();																	// Generated code.
	#ifdef DEBUGGABLE
	DB11_SetAddress(0x3C0);															// Set the default display address
	#endif
	cycleCounter = 0;																// Reset the cycle counter
	for (i = 0;i < 128;i++) dataMemory[i] = 0;
}

//*******************************************************************************************************
//										Detect usage of rotary control
//*******************************************************************************************************

void C11_DetectHardware(void) {
	int c = C11_ROMCheckSum();														// Identify the ROM. Default = no paddle 500 Khz.

	rotaryControlEnabled = FALSE;
	if (c == CSUM_PINBALL || c == CSUM_BLOCKBUSTER ||								// Paddle supported ?
		c == CSUM_BASEBALL || c == CSUM_CONNECTFOUR || 
		c == CSUM_ALIENRAIDERS || c == CSUM_SUPERBLOCKBUSTER) rotaryControlEnabled = TRUE;	

	if (c == CSUM_SEADUEL) cycleSpeed = 300000/6; 									// Sea Duel runs at 300Khz Clock.
}

//*******************************************************************************************************
//		Hardware Interface. Connect I/O to hardware emulation which dispatches to physical emulation
//*******************************************************************************************************

static int hardwareIO(char operation,int p1,int p2) {
	int retVal = 0;
	long usTime = cycleCounter*1000000L / CYCLES_PER_SECOND;						// Elapsed time in microseconds

	ASSERT(RANGE(a,0,15) && RANGE(x,0,7) && RANGE(y,0,15));
	ASSERT(RANGE(pa,0,15) && RANGE(pb,0,15) && RANGE(pc,0,63));
	ASSERT(RANGE(status,0,1) && RANGE(cl,0,1) && RANGE(sl,0,1));
	ASSERT(RANGE(ca,0,1) && RANGE(cb,0,1) && RANGE(cs,0,1) && RANGE(sr,0,63));

	switch(operation) {
		case 'S':	HWISynchronise(FRAMES_PER_SECOND);								// (S)ychronise is passed on to the hardware
					break;															

		case 'O':	HWISetDataLines(p1 & 0xF); 										// (O) sets the LCD lines on D0-D3
					break; 

		case 'K':	retVal = 0;				 										// (K) reads the keypads
					if (r[10] != 0) retVal = retVal | HWIReadKeypadColumn(0);		// R10 connects to left column (top is bit 3)
					if (r[9] != 0) retVal = retVal | HWIReadKeypadColumn(1);		// R9 connects to middle column
					if (r[8] != 0) retVal = retVal | HWIReadKeypadColumn(2);		// R8 connects to right column
					if (rotaryControlEnabled) {										// If rotary control enabled in this game
						retVal = retVal & 0x7;										// Clear K8 line.
						if (HWIRotaryControlTimeOut(usTime)) retVal = retVal | 8;	// If Charging Circuit timed out Set K8 line
					}
					ASSERT(RANGE(retVal,0,15));
					break;

		case 'R':	p2 = (p2 != 0) ? 1 : 0; 										// (R) sets/resets an R Latch
					ASSERT(RANGE(p1,0,10));
					if (p1 == 6) HWISetLatchPulse(p2); 								// R6 is connected to pin 5, Latch Pulse via LCD4
					if (p1 == 7) HWISetNotDataClock(p2);							// R7 is connected to pin 6, the !Data Clk line, via LCD5
					if (p1 == 2) HWISetRotaryControlLine(p2,usTime);				// R2 is connected to the rotary control line.
					if (p1 == 0) HWISetSpeakerLine(p2,usTime); // R0 is connected to the speaker line.

					break;
	}
	return retVal;
}

//*******************************************************************************************************
//								Execute one or more instructions on the TMS1100
//*******************************************************************************************************

BOOL C11_Execute(int count) {
	if (count < 0) count = CYCLES_PER_FRAME;										// Do a frame if required.
	do {

		BYTE8 opcode;
		#ifndef ARDUINO
		opcode = codeMemory[PCTR];													// Get the instruction
		#else
		opcode = pgm_read_byte_near(codeMemory+(PCTR));								// Arduino, get it from program Memory.
		#endif
		//int pcx = PCTR;

		ASSERT(RANGE(PCTR,0,2047) && RANGE(opcode,0,255));
		pc = (pc + 1) & 0x3F;														// Increment the program counter (6 bit)
		lastStatus = status;status = 1;												// Preserve last status and set flag.
		cycleCounter++;																// Bump Cycle Counter
		switch(opcode) {															// Execute the opcode
			#include "opcodes11.h"
		}

		//if (dataMemory[0x25] == 0xE || dataMemory[0x27] == 0xE || dataMemory[0x29] == 0xE) exit(printf("%x\n",pcx));

		if (frameCounter -- <= 0) { 												// Frame completed ?
			hardwareIO('S',0,0);													// Hardware synchronise
			frameCounter = CYCLES_PER_FRAME;										// Reset counter
		}
		count--;																	// Decrement times till break
	}
	while (count > 0 && PCTR != breakPoint);										// Execute until breakpoint or counted out
	return (PCTR == breakPoint);													// Return true if at breakpoint.
}

#ifdef DEBUGGABLE

//*******************************************************************************************************
//												Set Breakpoint
//*******************************************************************************************************

void C11_SetBreakPoint(int nbp) {
	breakPoint = nbp;
}

//*******************************************************************************************************
//											  Load a Binary File
//*******************************************************************************************************

//void C11_Load(char *binaryFile) {
//	FILE *handle = fopen(binaryFile,"rb");
//	if (handle == NULL) exit(fprintf(stderr,"Can't load file '%s'.\n",binaryFile));
//	fread(codeMemory,2048,1,handle);
//	fclose(handle);
//}

static int sequence[64] = { 
		0x00, 0x01, 0x03, 0x07, 0x0F, 0x1F, 0x3F, 0x3E,
		0x3D, 0x3B, 0x37, 0x2F, 0x1E, 0x3C, 0x39, 0x33,
		0x27, 0x0E, 0x1D, 0x3A, 0x35, 0x2B, 0x16, 0x2C,
		0x18, 0x30, 0x21, 0x02, 0x05, 0x0B, 0x17, 0x2E,
		0x1C, 0x38, 0x31, 0x23, 0x06, 0x0D, 0x1B, 0x36,
		0x2D, 0x1A, 0x34, 0x29, 0x12, 0x24, 0x08, 0x11,
		0x22, 0x04, 0x09, 0x13, 0x26, 0x0C, 0x19, 0x32,
		0x25, 0x0A, 0x15, 0x2A, 0x14, 0x28, 0x10, 0x20
};

static char prgBuffer[2048];

void C11_LoadAndReorganise(char *binaryFile) {
	FILE *handle = fopen(binaryFile,"rb");
	if (handle == NULL) exit(fprintf(stderr,"Can't load file '%s'.\n",binaryFile));
	fread(prgBuffer,2048,1,handle);
	fclose(handle);
	for (int i = 0;i < 2048;i++) {
		codeMemory[i] = prgBuffer[(i & 0x7C0) | sequence[i & 0x3F]];
	}
	for (int i = 0;i < 2048;i++) {
		if ((codeMemory[i] & 0x80) != 0) {												// Is it CALL or BR ?
			int n = -1;
			for (int j = 0;j < 64;j++) if (sequence[j] == (codeMemory[i] & 0x3F)) n = j;
				codeMemory[i] = (codeMemory[i] & 0xC0) | n;	
		}
	}
	fileCheckSum = -1;
	// printf("%x\n",C11_ROMCheckSum());
}

//*******************************************************************************************************
//										Return the Status of the TMS1100
//*******************************************************************************************************

static C11_STATUS stat;

C11_STATUS *C11_GetStatus(C11_STATUS *s) {
	if (s == NULL) s = &stat;
	s->a = a;s->x = x;s->status = status ? 1:0;s->lastStatus = lastStatus ? 1 : 0;		// Copy CPU Data and Memory References into structure
	s->pa = pa;s->pb = pb;s->pc = pc;s->ca = ca;s->cb = cb;s->cs = cs;s->y = y;
	s->sr = sr;s->cl = cl;s->sl = sl;s->o = o;
	s->dataMemory = dataMemory;s->codeMemory = codeMemory;
	s->rLatches = r;s->breakPoint = breakPoint;
	s->xy = x*16+y;s->memxy = dataMemory[s->xy];
	s->pctr = PCTR;s->opcode = codeMemory[s->pctr];
	return s;
}

//*******************************************************************************************************
//								Dump sorted ROM Image as an include file
//*******************************************************************************************************

void C11_GenerateHFile(void) {
	FILE *f = fopen("rom.h","w");
	fprintf(f,"static PROGMEM prog_uchar codeMemory[2048] = { ");
	int i;
	for (i = 0;i < 2048;i++) {
		fprintf(f,"0x%02x",codeMemory[i]);
		if (i != 2047) fprintf(f,",");
	}
	fprintf(f,"};\n");
	fclose(f);
}

#endif

//*******************************************************************************************************
//											Get ROM Checksum
//*******************************************************************************************************

int C11_ROMCheckSum(void) {
	int i,n;
	if (fileCheckSum < 0) {
		fileCheckSum = 0;																// Simple additive checksum.
		for (i = 0;i < 2048;i++) {
			#ifndef ARDUINO
			n = codeMemory[i];															// Get the instruction
			#else
			n = pgm_read_byte_near(codeMemory+i);										// Arduino, get it from program Memory.
			#endif
			fileCheckSum = (fileCheckSum+n) & 0xFFFF;
		}
	}
	return fileCheckSum;
}
