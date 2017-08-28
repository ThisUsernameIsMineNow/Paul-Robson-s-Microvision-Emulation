
//*******************************************************************************************************
//*******************************************************************************************************
//
//      Name:       HWInterface.H
//      Purpose:    Header for CPU<->Hardware Interface
//      Author:     Paul Robson
//      Date:       3rd January 2014
//
//*******************************************************************************************************
//*******************************************************************************************************

#ifndef _HWINTERFACE_H
#define _HWINTERFACE_H

#ifdef DEBUGGABLE

typedef struct _HWI_Status {
	int dataBus, latchPulse, notDataClock, addressedLatchCounter,polarity;
	unsigned char *addressedLatches;
	int polaritySwitchFrequency;
} HWI_STATUS;

HWI_STATUS *HWIGetStatus(HWI_STATUS *s);

#endif

void HWISynchronise(int framesPerSecond);
void HWISetDataLines(int nibble);
int HWIReadKeypadColumn(int column);
void HWISetLatchPulse(int level);
void HWISetNotDataClock(int level);
void HWISetSpeakerLine(int level,long microseconds);
int HWIReadLCDRow(int row);
void HWISetRotaryControlLine(int level,long microseconds);
BOOL HWIRotaryControlTimeOut(long microseconds);

BOOL XHWIReadKeypad(int col,int row);
void XHWISetPitch(int frequency);
void XHWISynchronise(int milliseconds);
int XHWIGetRotaryControl(void);

#endif
