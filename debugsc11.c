//*******************************************************************************************************
//*******************************************************************************************************
//
//      Name:       DebugSc11.C
//      Purpose:    Debug Screen Display (TMS1100)
//      Author:     Paul Robson
//      Date:       2nd February 2014
//
//*******************************************************************************************************
//*******************************************************************************************************

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "general.h"
#include "core11.h"
#include "hardware.h"
#include "hwinterface.h"
#include "debugsc11.h"

static void DB11_PrintString(int x,int y,char *text,int fgr);
static void DB11_PrintHex(int x,int y,int n,int fgr,int w);
static int  currentAddress = 0x3C0;

#include "mnemonics11.h"

//*******************************************************************************************************
//                                      Draw the debugger screen
//*******************************************************************************************************

void DB11_Draw()
{
    char *labels[] = { "A","X","Y","XY","M","ST","O","CL","SL","CT","DB",
                        "CA","PA","PC","CB","CS","PB","SR","LP","DC","PO","BK",NULL };
    int i = 0;
    while (labels[i] != NULL)
    {
        DB11_PrintString(12+(i/11)*6,i % 11,labels[i],i == 19 ? 5 : 2);
        i++;
    }
    C11_STATUS *s = C11_GetStatus(NULL);
    DB11_PrintHex(15,0,s->a,3,1);
    DB11_PrintHex(15,1,s->x,3,1);
    DB11_PrintHex(15,2,s->y,3,1);
    DB11_PrintHex(15,3,s->xy,3,2);
    DB11_PrintHex(15,4,s->memxy,3,1);
    DB11_PrintHex(15,5,s->status,3,1);
    DB11_PrintHex(15,6,s->o,3,2);
    DB11_PrintHex(15,7,s->cl,3,1);
    DB11_PrintHex(15,8,s->sl,3,1);
    DB11_PrintHex(21,0,s->ca,3,1);
    DB11_PrintHex(21,1,s->pa,3,1);
    DB11_PrintHex(21,2,s->pc,3,2);
    DB11_PrintHex(21,3,s->cb,3,1);
    DB11_PrintHex(21,4,s->cs,3,1);
    DB11_PrintHex(21,5,s->pb,3,1);
    DB11_PrintHex(21,6,s->sr,3,2);
    DB11_PrintHex(21,10,s->breakPoint,3,3);

    HWI_STATUS *s2 = HWIGetStatus(NULL);
    DB11_PrintHex(15,9,s2->addressedLatchCounter,3,1);
    DB11_PrintHex(15,10,s2->dataBus,3,1);
    DB11_PrintHex(21,7,s2->latchPulse,3,1);
    DB11_PrintHex(21,8,s2->notDataClock,3,1);
    DB11_PrintString(21,9,s2->polarity ? "+":"-",3);

    i = s2->polaritySwitchFrequency;if (i > 99) i = 99;
    DB11_PrintHex(26,10,i,3,3);
    DB11_PrintString(30,10,"Hz",3);

    for (i = 0;i <= 22;i++) {
        int addr = (currentAddress + i - 4) & 0x7FF;
        int opcode = s->codeMemory[addr];
        DB11_PrintHex(0,i,addr,(addr == s->pctr) ? 3 : 2,3);
        if (i == 4) DB11_PrintString(3,i,":",2);
        DB11_PrintString(4,i,_mnemonics[opcode],(addr == s->pctr) ? 3 : 2);
    }
    for (i = 0;i < 16;i++) {
        DB11_PrintHex(15+i,12,i,2,1);
        if (i < 8) DB11_PrintHex(12,i+15,i*16,2,2);
        if (i <= 7) DB11_PrintHex(15+i,13,s2->addressedLatches[i],(i == s2->addressedLatchCounter) ? 3 : 6,1);
        if (i <= 10) DB11_PrintHex(15+i,14,s->rLatches[i] ? 1 : 0,(i == s->y) ? 3 : 6,1);
    }
    for (i = 0;i < 128;i++) {
        DB11_PrintHex(15+i%16,15+i/16,s->dataMemory[i],(i == s->xy) ? 3 : 6,1);
    }
    DB11_PrintString(12,14,"RL",2);
    DB11_PrintString(12,13,"AL",2);
}

//*******************************************************************************************************
//                                          Print a string
//*******************************************************************************************************

static void DB11_PrintString(int x,int y,char *text,int fgr)
{
    while (*text != '\0')
    {
        IF_Write(x++,y,*text++,fgr);
    }
}

//*******************************************************************************************************
//                                  Print a hexadecimal constant
//*******************************************************************************************************

static void DB11_PrintHex(int x,int y,int n,int fgr,int w)
{
    char buffer[9];
    sprintf(buffer,"%0*x",w,n);
    ASSERT(strlen(buffer) <= 8);
    DB11_PrintString(x,y,buffer,fgr);
}

//*******************************************************************************************************
//                                          Set Display Address
//*******************************************************************************************************

void DB11_SetAddress(int addr) {
    currentAddress = addr;
}

//*******************************************************************************************************
//                                          Get Display Address
//*******************************************************************************************************

int DB11_GetAddress() {
    return currentAddress;
}

//*******************************************************************************************************
//                                      Shift digit into current address
//*******************************************************************************************************

void DB11_UpdateAddress(int digit) {
    currentAddress = ((currentAddress << 4) | digit) & 0x7FF;
}
