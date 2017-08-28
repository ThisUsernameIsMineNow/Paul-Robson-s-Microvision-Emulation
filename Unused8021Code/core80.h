//*******************************************************************************************************
//*******************************************************************************************************
//
//      Name:       Core80.H (i8021)
//      Purpose:    CPU Core Header
//      Author:     Paul Robson
//      Date:       3rd January 2014
//
//*******************************************************************************************************
//*******************************************************************************************************

#ifndef _CORE80_H
#define _CORE80_H

typedef struct _C80_Status {
	int a,x,y,status,lastStatus,pa,pb,pc,ca,cb,cs,sr,cl,sl,o;
	unsigned char *dataMemory, *codeMemory, *rLatches;
	int xy,memxy,pctr,opcode,breakPoint;
} C80_STATUS;

C80_STATUS *C80_GetStatus(C80_STATUS *s);
void C80_Reset();
BOOL C80_Execute(int count);
void C80_SetBreakPoint(int nbp);
void C80_Load(char *binaryFile);

#endif
