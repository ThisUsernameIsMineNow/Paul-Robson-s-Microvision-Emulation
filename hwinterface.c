
//*******************************************************************************************************
//*******************************************************************************************************
//
//      Name:       HWInterface.C 
//      Purpose:    Emulates the functionality of LCD/Keyboard/Speaker, seperates to physical implementation
//      Author:     Paul Robson
//      Date:       3rd January 2014
//
//*******************************************************************************************************
//*******************************************************************************************************

#include <stdlib.h>
#include <stdio.h>
#include "general.h"
#include "core11.h"
#include "debugsc11.h"
#include "hwinterface.h"

#define LATENCY 	(50) 												// LCD latency in milliseconds.

static BYTE8 dataLine = 0;												// D0-D3 Status
static BYTE8 latchPulse = 0;											// Latch Pulse line
static BYTE8 notDataClock = 0;											// Current Data Clock line
static BYTE8 addressedLatchCounter = 0; 								// 3 bit Addressed Latch counter
static BYTE8 addressedLatches[8] = { 0 };								// 8 x 4 bit Addressed Latches
static BYTE8 polarity = 0;												// LCD Polarity
static WORD16 lcdDisplay[16] = { 0 } ;									// LCD Display, 0 = top row, MSB = left pixel
static short lcdDisplayLatency[16*16] = { 0 };							// Time left for each row (pixel) before latency clears pixels.
																		// should be done on a pixel basis really (!)
static BYTE8 latchUpdateCount = 0;										// No. of times latches have been updated this frame.

static int polaritySwitchCount = 0;										// Number of polarity switches
static int elapsedMilliseconds = 0;										// Elapsed CPU time

static long timeFirstLowHighSoundTransition = 0;						// First low->high sound transition (ms)
static long timeLastLowHighSoundTransition = 0;							// Last low->high sound transition (ms)
static int countLowHighSoundTransition = 0; 							// Number of transitions
static int levelSoundLine = 0; 											// Level on sound line.
static int levelChargePin = 0;											// Level on charging pin
static long chargeEndTime = 0L;											// Time when charging ends.
static BOOL bOptimisedRendering = FALSE;								// True to use fast rendering.

static void HWITransferAddressedLatchesToLCDScreen();

//*******************************************************************************************************
//									Get Hardware Interface Status
//*******************************************************************************************************

#ifdef DEBUGGABLE

static HWI_STATUS stat;

HWI_STATUS *HWIGetStatus(HWI_STATUS *s) {
	if (s == NULL) s = &stat;
	s->dataBus = dataLine;s->latchPulse = latchPulse;s->notDataClock = notDataClock;
	s->addressedLatchCounter = addressedLatchCounter;s->polarity = polarity;
	s->addressedLatches = addressedLatches;
	s->polaritySwitchFrequency = 0;
	if (polaritySwitchCount > 0) 
		s->polaritySwitchFrequency = polaritySwitchCount * 1000 / elapsedMilliseconds;
	return s;
}

#endif

//*******************************************************************************************************
//											Read Display Line
//*******************************************************************************************************

int HWIReadLCDRow(int row) {
	ASSERT(RANGE(row,0,15));
	return lcdDisplay[row] & 0xFFFF;
}

//*******************************************************************************************************
//				Synchronise : Dispatches Synchronisation to Hardware Implementation
//*******************************************************************************************************

void HWISynchronise(int framesPerSecond) {
	int row,col;
	short elapsedTime = 1000/framesPerSecond;							// Elapsed time in ms this frame.
	elapsedMilliseconds += elapsedTime;									// Update the timer
	XHWISynchronise(elapsedTime);										// Synchronise the system.

	//
	// Process LCD latency. This should really be done on a pixel by pixel basis, but a row basis is
	// reasonable approximation and more efficient. It will work fine provided there is a typical 
	// 'update the screen loop' running.
	//

	if (latchUpdateCount == 0) { 										// If no latch updates, then don't
		elapsedTime = 0;												// reduce the latency as the LCD driver
	}																	// will refresh it.

	for (row = 0;row < 16;row++) {										// Check for LCD latency
		if (bOptimisedRendering) {
			lcdDisplayLatency[row] -= elapsedTime;						// Deduct time from latency timer
			if (lcdDisplayLatency[row] <= 0) {							// Has it timed out, e.g. turn off pixels		
				lcdDisplayLatency[row] = 0;								// Clear that counter
				lcdDisplay[row] = 0;									// And clear that row.
			}
		} else { 														// Slow latency processing, each individually.
			int rm = row * 16;
			if (lcdDisplay[row] != 0) {									// Optimisation, don't if all pixels are off.
				for (col = 0;col < 16;col++) { 							// Process latency on each pixel individually.
					if (lcdDisplayLatency[rm+col] >= 0) {
						lcdDisplayLatency[rm+col] -= elapsedTime;
						if (lcdDisplayLatency[rm+col] <= 0) { 			// Pixel off due to latency time elapsed.
							lcdDisplayLatency[rm+col] = 0;
							lcdDisplay[row] &= ((0x8000 >> col) ^ 0xFFFF);
						}
					}
				}
			}
		}
	}
	latchUpdateCount = 0; 												// Clear the count of latch updates.

	if (countLowHighSoundTransition >= 2) {								// Has there been at least two pulses (e.g. there's a first and last time)
		int us = (int)(timeLastLowHighSoundTransition - 				// Period (time in microseconds)
								timeFirstLowHighSoundTransition);
		int freq = (countLowHighSoundTransition - 1) * 1000000 / us;	// Calculate the pitch. -1 because if there are two, that's one cycle.
		if (freq >= 50 && freq < 2400) XHWISetPitch(freq);				// Output it to the hardware interface.
		else XHWISetPitch(0);
	} else {
		XHWISetPitch(0);												// Clear sound if no transitions.
	}
	countLowHighSoundTransition = 0;									// Clear the low->high transition count.
}

//*******************************************************************************************************
//				Update LCD Status as if transfer to holding latches has taken place
//*******************************************************************************************************

static void HWITransferAddressedLatchesToLCDScreen() {

	#ifdef ARDUINO
	bOptimisedRendering = TRUE;
	#endif

	WORD16 rowData = (addressedLatches[0] << 12) | 						// Get row data as a 16 bit value
					(addressedLatches[1] << 8) | (addressedLatches[2] << 4) | (addressedLatches[3]);
	WORD16 colData = (addressedLatches[4] << 12) | 						// Get col data as a 16 bit value
					(addressedLatches[5] << 8) | (addressedLatches[6] << 4) | (addressedLatches[7]);

	latchUpdateCount++; 												// The laatches have been transferred.
	if (rowData == 0 || colData == 0) return;							// Has to be at least one bit on in each.

	int row = 0;
	while (rowData != 0) {												// Bit shift through the rows
		ASSERT(RANGE(row,0,15));
		if (rowData & 0x8000) {											// If the MSB is set
			if (bOptimisedRendering) {
				lcdDisplay[row] = colData;								// Copy the column data into the current row
				lcdDisplayLatency[row] = LATENCY;						// Set the LCD latency - how long it stays on for.
			} else {
				lcdDisplay[row] |= colData; 							// Slow pixel based latency.
				int c = colData;										// Copy Latency value where all the pixels are set.
				int p = row * 16;
				while (c != 0) {
					if (c & 0x8000) lcdDisplayLatency[p] = LATENCY;
					c = (c << 1) & 0xFFFF;
					p++;
				}
			}
		}																// done at row level for efficiency.
		rowData = (rowData << 1) & 0xFFFF;								// Bit shift for the next row
		row++;
	}
}

//*******************************************************************************************************
//	Handle the 'normal' LCD processing - things that are dependent on LP and !DC states, not on 
//	transitions, these are handled in the individual methods for setting those line states.
//*******************************************************************************************************

static void HWIStandardProcessing() {
	if (notDataClock == 0) {											// Note 1 : If !DataClock low, then load addressed latches
		addressedLatches[addressedLatchCounter] = dataLine;				
	}

	if (latchPulse != 0 && notDataClock != 0) {							// Note 3 : Address->Holding latch occurs when LP is high and !DC is high
		HWITransferAddressedLatchesToLCDScreen();						// The holding latches are not emulated, it goes to the LCD screen.
	}

	if (latchPulse != 0) {												// Note 5 : If Latch pulse is high, reset the addressed Latch Counter
		addressedLatchCounter = 0;
	}
}

//*******************************************************************************************************
//					Sets the current value on the Hughes0488 Data Input (4 bits)
//*******************************************************************************************************

unsigned char bitReverse[16] = { 0,8,4,12,2,10,6,14,1,9,5,13,3,11,7,15 };

void HWISetDataLines(int nibble) {
	dataLine = bitReverse[nibble & 0xF]; 								// Update the data bus values
	int c = C11_ROMCheckSum();
	if (c == CSUM_VEGASSLOTS || c == CSUM_BOWLING || 					// Different outputs.
		c == CSUM_SEADUEL || c == CSUM_COSMICHUNTER || c == CSUM_CONNECTFOUR ||
		c == CSUM_SUPERBLOCKBUSTER|| c == CSUM_ALIENRAIDERS) dataLine = nibble & 0xF;	

	//printf("%04x %x\n",C11_GetStatus(NULL)->pctr,dataLine);
	HWIStandardProcessing();											// And process it.
}

//*******************************************************************************************************
//			Read a keypad column (0-2) as a 4 bit value. The LSB is the top most key
//*******************************************************************************************************

int HWIReadKeypadColumn(int column) {
	ASSERT(RANGE(column,0,2));
	return (XHWIReadKeypad(column,0) != 0 ? 8 : 0) | 
					(XHWIReadKeypad(column,1) != 0 ? 4 : 0) | (XHWIReadKeypad(column,2) != 0 ? 2 : 0) | (XHWIReadKeypad(column,3) != 0 ? 1 : 0);
}

//*******************************************************************************************************
//						Set the level on the Hughes0488 'Latch Pulse' pin.
//*******************************************************************************************************

void HWISetLatchPulse(int level) {
	level = level ? 1 : 0;												// Set level to 1 or 0
	if (latchPulse != 0 && level == 0 && notDataClock == 0) {			// Note 4: Falling edge on Latch Pulse when !DC = 0
		polarity = (polarity != 0) ? 0 : 1;								// Inverts the LCD polarity.
		polaritySwitchCount++;											// Count them.
	}
	latchPulse = level;													// Update latch pulse value.
	HWIStandardProcessing();											// And process it.	
}

//*******************************************************************************************************
//						Set the level on the Hughes0488 'Not Data Clock' pin.
//*******************************************************************************************************

void HWISetNotDataClock(int level) {
	level = level ? 1 : 0;												// Process level value

	if (notDataClock == 0 && level != 0) { 								// Note 7D: A rising edge on !Data Clock
		addressedLatchCounter = (addressedLatchCounter + 1) & 7;		// Bumps the three bit addressed latch counter
	}
	notDataClock = level;												// Update new value
	HWIStandardProcessing();											// And process it.	
}

//*******************************************************************************************************
//								Set the level on the speaker line.
//*******************************************************************************************************

void HWISetSpeakerLine(int level,long microseconds) {
	if (level != levelSoundLine && level != 0) {						// 0->1 transition on the sound line.
		if (countLowHighSoundTransition == 0)							// Store in first or last dependent on count.
			timeFirstLowHighSoundTransition = microseconds;
		else
			timeLastLowHighSoundTransition = microseconds;
		countLowHighSoundTransition++;									// Bump the transition counter.
	}
	levelSoundLine = level;												// Update current level.
}

//*******************************************************************************************************
//									Process changes to the R2 line
//*******************************************************************************************************


void HWISetRotaryControlLine(int level,long microseconds) {
	level = (level != 0) ? 1 : 0;										// Make value 0 or 1.
	if (level != 0 && levelChargePin == 0) { 							// Is this a 0->1 transition, e.g. start of a charge ?
		int csum = C11_ROMCheckSum();									// Idenitify the ROM Image
		chargeEndTime = microseconds + 									// Trial and error here 
					600 + 65 * XHWIGetRotaryControl() / 10;				// Blockbuster, Alien Raiders and Super BlockBuster
		if (csum == CSUM_PINBALL)										// Pinball
			chargeEndTime = microseconds + 650 + 80 * XHWIGetRotaryControl() / 10;
		if (csum == CSUM_CONNECTFOUR) 									// Connect Four
			chargeEndTime = microseconds+600+50 * XHWIGetRotaryControl() / 10;
		if (csum == CSUM_BASEBALL)										// Baseball
			chargeEndTime = microseconds + 750 + 120 * XHWIGetRotaryControl() / 10;

	}	
	levelChargePin = level;												// Save the charge level.
}

//*******************************************************************************************************
//						  Check to see if the charging circuit has timed out.
//*******************************************************************************************************

BOOL HWIRotaryControlTimeOut(long microseconds) {
	if (levelChargePin == 0) return FALSE;								// If R2 = 0 then it never times out.
	return (microseconds > chargeEndTime);								// If charging, return TRUE if timed out.
}
