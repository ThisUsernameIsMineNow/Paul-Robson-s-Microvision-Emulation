//*******************************************************************************************************
//*******************************************************************************************************
//
//      Name:       General.H
//      Purpose:    General Constants, Includes
//      Author:     Paul Robson
//      Date:       1st January 2014
//
//*******************************************************************************************************
//*******************************************************************************************************

#ifndef _GENERAL_H
#define _GENERAL_H

typedef unsigned char BOOL;
typedef unsigned char BYTE8;                                                        // Type definitions used in CPU Emulation
typedef unsigned short WORD16;
typedef unsigned int LONG32;
typedef signed short INT16;

#define BUILD   	(14) 															// Build Number.

#define FALSE       (0)                                                             // Boolean type
#define TRUE        (!(FALSE))

#define CSUM_MINDBUSTER			(0xC757)											// Known ROM Images Checksums
#define CSUM_PHASERSTRIKE		(0x08F0) 											// Used to identify paddle hardware.
#define CSUM_VEGASSLOTS 		(0x8F27)
#define CSUM_BLOCKBUSTER		(0x5091)
#define CSUM_BOWLING       	 	(0x49B1)
#define CSUM_PINBALL 			(0xFD21)
#define CSUM_SEADUEL 			(0xA3DC)
#define CSUM_ALIENRAIDERS 	 	(0x81AB)
#define CSUM_BASEBALL 			(0xC725)
#define CSUM_CONNECTFOUR 		(0x55BA)
#define CSUM_COSMICHUNTER		(0x6337)
#define CSUM_SUPERBLOCKBUSTER 	(0x1D93)

//#define ASSERT(x) if (!(x)) exit(fprintf(stderr,"Assert: %s %d\n",__FILE__,__LINE__));
#define ASSERT(x) {}

#define RANGE(v,f,l) ((v >= f) && (v <= l))
#endif


