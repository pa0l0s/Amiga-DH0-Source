#include "level.h"

#include "tile.h"
#include "window.h"
#include "fileIO.h"

#include <proto/graphics.h>
#include <proto/dos.h>

#include <stdlib.h>
//============================================================================

static void fillLevel(int tile);
//============================================================================
UBYTE* g_pAllLevels = NULL;

UBYTE* g_pLevel = NULL;

int g_nAmountOfLvl = 2;

int g_nLvlTWidth;
int g_nLvlTHeight;

int g_nLvlMaxTWidth;
int g_nLvlMaxTHeight;

int g_nLvlNumber = 0;

//============================================================================
int InitLevels(void)
{
	g_nLvlMaxTWidth = 320/g_nTileWidth;
	g_nLvlMaxTHeight = 192/g_nTileHeight;

	g_nLvlTWidth = g_nLvlMaxTWidth;
	g_nLvlTHeight = g_nLvlMaxTHeight;

	g_pLevel = (UBYTE*)malloc(g_nLvlMaxTWidth*g_nLvlMaxTHeight);

	if (NULL == g_pLevel)
	{
		return RT_FAILED_MEM_LEVEL;
	}

	int sizeAllLevels = GetFileSizeByName("levels.bin");

	g_pAllLevels = (UBYTE*)malloc(sizeAllLevels);

	if (NULL == g_pAllLevels)
	{
		return RT_FAILED_MEM_ALL_LEVELS;
	}

	ReadFile("levels.bin", g_pAllLevels, sizeAllLevels);

	return RT_OK;
}
//============================================================================
void KillLevels(void)
{
	if (g_pAllLevels)
	{
		free(g_pAllLevels);
	}

	if (g_pLevel)
	{
		free(g_pLevel);
	}
}
//============================================================================
void LvlToWin(void)
{
	int x;
	int y;
	UBYTE* p = g_pLevel;

	int nLvlTopX = 0;
	int nLvlTopY = 0;
	int nLvlBotX = g_nLvlTWidth * g_nTileWidth;
	int nLvlBotY = g_nLvlTHeight * g_nTileHeight;

	for (y = nLvlTopY; y < nLvlBotY; y+= g_nTileHeight)
	{
		for (x = nLvlTopX; x < nLvlBotX; x+= g_nTileWidth)
		{
			UBYTE a = *p++;
			BltBitMapRastPort(g_pTileBMap, a*g_nTileWidth, 0, g_pRpMain, x,y, g_nTileWidth, g_nTileHeight, 0xc0);
		}

		Delay(2);
	}
}
//============================================================================
void CleanLevel(void)
{
	fillLevel(TILE_EMPTY);
	LvlToWin();
}
//============================================================================
void RockLevel(void)
{
	fillLevel(TILE_ROCK);
	LvlToWin();
}

//============================================================================
void CopyLvl(int nLvlNumber)
{
	int x;
	int y;

	g_nLvlTWidth = g_nLvlMaxTWidth;
	g_nLvlTHeight = g_nLvlMaxTHeight;

	UBYTE* pSrc = g_pAllLevels + g_nLvlNumber * (g_nLvlTWidth*g_nLvlTHeight);
	UBYTE* pDst = g_pLevel;

	for (y = 0; y < g_nLvlTHeight; y++)
	{
		for (x = 0; x < g_nLvlTWidth; x++)
		{
			*pDst++ = *pSrc++;
		}
	}
}

//============================================================================
static void fillLevel(int tile)
{
	int x;
	int y;
	UBYTE* p = g_pLevel;

	for (y = 0; y < g_nLvlMaxTHeight; y++)
	{
		for (x = 0; x < g_nLvlMaxTWidth; x++)
		{
			*p++ = tile;
		}
	}
}
//============================================================================
