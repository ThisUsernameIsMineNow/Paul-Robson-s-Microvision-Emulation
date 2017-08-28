//*******************************************************************************************************
//*******************************************************************************************************
//
//      Name:       Main.C
//      Purpose:    Main program.
//      Author:     Paul Robson
//      Date:       1st January 2014
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

//*******************************************************************************************************
//                                              Main Program
//*******************************************************************************************************

int main(int argc,char *argv[])
{
    BOOL quit = FALSE;
    int w = 0,h = 0;                                                                    // Quick hack to scale screen size.
    if (argc == 3) {
        char c;
        sscanf(argv[2],"%d%c%d",&w,&c,&h);
        argc--;
    }

    IF_Initialise(w,h);                                                                 // Initialise the hardware
    IF_SetSound(FALSE);
    if (argc != 2) 
        exit(fprintf(stderr,"Microvision Emulator : Build %d : mvem <binary file> [<width>x<height>]\n",BUILD));
    char *file = argv[1];
    BOOL bDebug = file[0] == '@';                                                       // @ dumps to rom.h
    SYS_SetMode(bDebug);                                                                // Set start up mode.
    if (bDebug) file++;                                                                 // Dump at character.
    C11_LoadAndReorganise(file);                                                        // Load ROM Image
    IF_LoadOverlay(argv[1]);                                                            // Load overlay if it exists.
    int checksum = C11_ROMCheckSum();                                                   // Get the ROM checksum
    //printf("%x\n",checksum);
    C11_DetectHardware();                                                               // Detect the control.
    C11_Reset();                                                                        // Reset the TMS1100 CPU.
    while (!quit)                                                                       // Keep running till finished.
    {
        IF_ProcessRotaryControl();
        quit = IF_Render(SYS_Process());
    }
    IF_Terminate();
    return 0;
}

/*

    Fixes:

    8-1-14     1    Released.
    8-1-14     2    Fixed bug where sound not turned off if < 2 transitions to speaker line in a frame.
    9-1-14     3    Uprated speed to 500Khz Clock. Fixed overflow bug in sound frequency.
    11-1-14    4    Fixed to load LFSR-ordered binaries. Fixed LDX and BIT operand problem.
    13-1-14    5    Fixed Keypad wiring. Added checksum to detect cartridge variations.
    14-1-14    6    Added Rotary Control support.
    16-1-14    7    Fixed so handles column/row write, with adjustable rendering and latency row/colum switching.
    18-1-14    8    Added ASSERT() checking to attempt to fix wierd Windows problem.
    23-1-14    9    Removed switchable rendering, it doesn't work. Added Bowling Checksum.
    23-1-14    10   Added Pixel-based Latency stuff.
    24-1-14    11   Added Pinball Checksum (uses Rotary control). Added Mouse Control of Paddle. Changed non-debug display. Added overlays. 
                    Added Pinball-compatible rotary control.
    18-3-14    13   Added checksums for all known cartridges. 
                    Fixed Sea Duel opening display (no updating of latches in a frame)
                    Memory assumed clear on start by Sea Duel
                    Latency not reduced when no transfers from address latches to output latches in a frame.
                    Added variable speed, primarily for Sea Duel
                    Added paddle controls for those carts known to use them.
                    Now starts in run mode, @ switches to debugger on start up.
                    Added option to change screen size
                    Set up Paddle variables for paddle games.

    TODO: Mouse support in Windows does not work.

*/