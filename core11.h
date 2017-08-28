//*******************************************************************************************************
//*******************************************************************************************************
//
//      Name:       Core11.H (TMS1100)
//      Purpose:    CPU Core Header
//      Author:     Paul Robson
//      Date:       2nd January 2014
//
//*******************************************************************************************************
//*******************************************************************************************************

#ifndef _CORE11_H
#define _CORE11_H

#ifdef DEBUGGABLE

typedef struct _C11_Status {
	int a,x,y,status,lastStatus,pa,pb,pc,ca,cb,cs,sr,cl,sl,o;
	unsigned char *dataMemory, *codeMemory, *rLatches;
	int xy,memxy,pctr,opcode,breakPoint;
} C11_STATUS;

C11_STATUS *C11_GetStatus(C11_STATUS *s);
void C11_SetBreakPoint(int nbp);
void C11_Load(char *binaryFile);
void C11_LoadAndReorganise(char *binaryFile);
void C11_GenerateHFile(void);

#endif

void C11_Reset();
BOOL C11_Execute(int count);
int C11_ROMCheckSum(void);
void C11_DetectHardware(void);

#endif
