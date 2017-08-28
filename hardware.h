
//*******************************************************************************************************
//*******************************************************************************************************
//
//      Name:       Hardware.H
//      Purpose:    Hardware Interface Headers
//      Author:     Paul Robson
//      Date:       1st January 2014
//
//*******************************************************************************************************
//*******************************************************************************************************

#include "general.h"

#ifndef _HARDWARE_H
#define _HARDWARE_H

void IF_Initialise(int w,int h);
BOOL IF_Render(BOOL debugMode);
void IF_Terminate(void);
void IF_Write(int x,int y,char ch,int colour);
BOOL IF_KeyPressed(char ch);
void IF_SetSound(BOOL isOn);
void IF_SetPitch(int frequency);
int  IF_GetTime(void);
void IF_RenderScreen(BOOL debugMode); 
void IF_ProcessRotaryControl();
void IF_LoadOverlay(char *binFile);

#endif
