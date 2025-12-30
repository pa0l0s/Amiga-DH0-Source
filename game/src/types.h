#ifndef BOXO_TYPES_H
#define BOXO_TYPES_H
//============================================================================
#include <exec/types.h>
#include <stdio.h>
//--------------------------------------------------------
typedef void(*PVF)(void);


enum ReturnCodes
{
	RT_OK = 0,
	RT_FAILED_OPEN_INTUITION,
	RT_FAILED_OPEN_GRAPHICS,
	RT_FAILED_OPEN_DATATYPES,
	RT_FAILED_OPEN_UTILITY,
	RT_FAILED_LOAD_TILES_PIC,
	RT_FAILED_MEM_LEVEL,
	RT_FAILED_OPEN_FILE,
	RT_FAILED_MEM_ALL_LEVELS,
	RT_FAILED_GAMEPORT_MSGPORT,
	RT_FAILED_GAMEPORT_IOREQ,
	RT_FAILED_GAMEPORT_DEVICE,
	RT_FAILED_LOAD_INTROS_PIC,
	RT_FAILED_INTRO,
	RT_FAILED,
};
//============================================================================
#endif // BOXO_TYPES_H
