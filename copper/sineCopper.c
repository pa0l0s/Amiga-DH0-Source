#include "colorUtil.h"
#include "colorArrayUtil.h"
#include "mathUtil.h"
#include "mathArrayUtil.h"
#include "gfxUtil.h"

#include "scGeo.h"

#include <stdio.h>
#include <hardware/custom.h>
#include <clib/exec_protos.h>

extern struct Custom custom;

// 20 instead of 127 because of input.device priority
#define TASK_PRIORITY           (100)
#define COLOR00                 (0x180)
#define BPLCON0                 (0x100)
#define PRA_FIR0_BIT            (1 << 6)	// http://amigadev.elowar.com/read/ADCD_2.1/Hardware_Manual_guide/node012E.html
#define RMB_BIT		            (1 << 10)	// ?? so not PRA_FIR0_BIT, then ?? is there an include file for these kinds of constants ??
#define BPLCON0_COMPOSITE_COLOR (1 << 9)

#define BG_COLOR				(0x000)		// background color = black



UWORD * copList1;
UWORD * copList2;

void allocCopLists(UWORD copListLen)
{
	copList1 = (UWORD *) AllocMem(copListLen * sizeof(UWORD), MEMF_CHIP);
	copList2 = (UWORD *) AllocMem(copListLen * sizeof(UWORD), MEMF_CHIP);
}

void releaseCopLists(UWORD copListLen)
{
	FreeMem(copList1, copListLen * sizeof(UWORD));
	FreeMem(copList2, copListLen * sizeof(UWORD));
}

void refreshCopList(UWORD * copList, UWORD * linesY, UWORD * colors, UWORD nrLines)
{
	// init
	UWORD iCop = 0;
	copList[iCop++] = BPLCON0;
	copList[iCop++] = BPLCON0_COMPOSITE_COLOR;
	copList[iCop++] = COLOR00;
	copList[iCop++] = BG_COLOR;
	
	// actual list
	for(UWORD iLine = 0; iLine < nrLines; iLine++)
	{
		copList[iCop++] = ((linesY[iLine]) << 8) + 7;
		copList[iCop++] = 0xfffe;
		copList[iCop++] = COLOR00;
		copList[iCop++] = colors[iLine];
	}
	
	// lead out
	copList[iCop++] = ((linesY[nrLines - 1] + 1) << 8) + 7;
	copList[iCop++] = 0xfffe;
	copList[iCop++] = COLOR00;
	copList[iCop++] = BG_COLOR;
	
	copList[iCop++] = 0xffff;
	copList[iCop++] = 0xfffe;
}



UWORD calcBaseGradient(UWORD col0, UWORD col1, UWORD col2, UWORD * dst, UWORD nrDstMax)
{
	UWORD nrDstMax2 = nrDstMax >> 2;
	
	UWORD wCol01 = 3;
	UWORD wCol12 = 6;
	UWORD wCol = wCol01 + wCol12;
	
	UWORD nrCol01 = (nrDstMax * wCol01 / 2) / wCol;
	UWORD nrCol12 = (nrDstMax * wCol12 / 2) / wCol;
	
	InterpolateColors(col0, col1, nrCol01, dst);
	InterpolateColors(col1, col2, nrCol12, dst + nrCol01 - 1);
	MirrorColors(dst, dst + (nrCol01 + nrCol12 - 2), (nrCol01 + nrCol12 - 1));
	
	return ((nrCol01 + nrCol12 - 1) * 2) - 1;
}


extern UWORD minHeight;
extern UWORD maxHeight;
extern UWORD ** colorArrays;

int main(int argc, char **argv)
{
	int nrAnimFrames = 50;		// 2 seconds
	int iAnim = 0;
	
    volatile UBYTE *ciaa_pra = 		(volatile UBYTE *) 0xbfe001;
    volatile UWORD *someLocation = 	(volatile UWORD *) 0xdff016;		// http://www.retrorich.co.uk/2015/04/68k-part-1-easy-now-raster-lines-and.html ?? docs ??
	
	// scGeoInit(0.85f, 8.0f, 8.0f, 25.0f, 25.0f, nrAnimFrames);
	scGeoInit(0.1f, 1.8f, 1.8f, 45.0f, 3.0f, nrAnimFrames);
	// LogParameters();
	
	// init colors
	UWORD col0 = 0x113;
	UWORD col1 = 0x55a;
	UWORD col2 = 0x77f;
	UWORD baseGradient[maxHeight * 2];
	UWORD nrBaseGradient = calcBaseGradient(col0, col1, col2, baseGradient, maxHeight * 2);
	if(!initColorArrays(baseGradient, nrBaseGradient))
	{
		printf("!! ERROR in initColorArrays(baseGradient, nrBaseGradient)\n");
	}
	UWORD * colorArray = colorArrays[0];
		
	// init copper
	UWORD linesY[maxHeight];
	UWORD colors[maxHeight];
	
	UWORD copListLen = 4 + (4 * maxHeight) + 4 + 2;
	allocCopLists(copListLen);
	
	UWORD currHeight = calcLinesAndColors(linesY, colors, iAnim);
	UWORD * currCopList = copList1;
	refreshCopList(currCopList, linesY, colors, currHeight);
	
    SetTaskPri(FindTask(NULL), TASK_PRIORITY);
    BOOL is_pal = initDisplay();
    custom.cop1lc = (ULONG) currCopList;
	
	BOOL cont = TRUE;
	while(cont)		// frame loop
	{
		// don't animate/refresh copper if rmb down
		if((*someLocation & RMB_BIT) != 0)
		{
			iAnim += 1;
			if(iAnim >= nrAnimFrames) iAnim = 0;
			
			currHeight = calcLinesAndColors(linesY, colors, iAnim);
			
			if(currCopList == copList1) currCopList = copList2;
			else currCopList = copList1;
			refreshCopList(currCopList, linesY, colors, currHeight);
		}
		
		// ?? don't swap the following 2 lines ?? how does WaitTOF() work, again ??
		custom.cop1lc = (ULONG) currCopList;
		WaitTOF();
		
		if((*ciaa_pra & PRA_FIR0_BIT) == 0) cont = FALSE;			// quit when lmb pressed
	}
	
	releaseCopLists(copListLen);
	releaseColorArrays();
	scGeoRelease();
	
    resetDisplay();
	
    return 0;
}