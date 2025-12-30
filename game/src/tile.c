#include "tile.h"
#include "window.h"
#include "level.h"

#include <proto/exec.h>
#include <proto/graphics.h>
#include <proto/intuition.h>
#include <proto/datatypes.h>
#include <datatypes/pictureclass.h> 
//============================================================================
int g_nTileWidth;
int g_nTileHeight;

struct BitMap* g_pTileBMap = NULL;
struct RastPort* g_pTileRPort = NULL;
//----------------------------------------------------------------------------
struct RastPort m_RPortTile;
//----------------------------------------------------------------------------

int initTiles(void)
{
	int iResult = RT_FAILED_LOAD_TILES_PIC;

	g_nTileWidth = 16;
	g_nTileHeight = 16;

	struct Screen* pWB = LockPubScreen(NULL);

	Object* o = NewDTObject("tile.pic",
		DTA_GroupID, GID_PICTURE,
		PDTA_Remap, TRUE,
		PDTA_Screen, pWB,
		TAG_END);

	if (o)
	{
		struct BitMap* bm;
		DoDTMethod(o, NULL, NULL, DTM_PROCLAYOUT,NULL,TRUE);

		GetDTAttrs(o,	
			PDTA_DestBitMap, &bm,
			TAG_END);

		ULONG nWidth = GetBitMapAttr(bm, BMA_WIDTH);
		ULONG nHeight = GetBitMapAttr(bm, BMA_HEIGHT);
		ULONG nDepth = GetBitMapAttr(bm, BMA_DEPTH);

		g_pTileBMap = AllocBitMap(nWidth, nHeight, nDepth, BMF_DISPLAYABLE|BMF_CLEAR, 0);

		if (NULL != g_pTileBMap)
		{
			BltBitMap(bm, 0, 0, g_pTileBMap, 0, 0, nWidth, nHeight, 0xC0, 0xFF, NULL);

			g_pTileRPort = &m_RPortTile;
			InitRastPort(g_pTileRPort);
			g_pTileRPort->BitMap = g_pTileBMap;

			iResult = RT_OK;
		}

		DisposeDTObject(o);
	}

	UnlockPubScreen(NULL, pWB);

	return RT_OK;
}

void killTiles(void)
{
	FreeBitMap(g_pTileBMap);
}

//============================================================================
void EraseShip(int x, int y)
{
	BltBitMapRastPort(g_pTileBMap, 0, 0, g_pRpMain, x,y, 16, 16, 0xc0);
}
//============================================================================
void PasteShip(int x, int y)
{
	BltBitMapRastPort(g_pTileBMap, 48, 0, g_pRpMain, x,y, 16, 16, 0xc0);
}
//============================================================================
void PasteTile(int x, int y, UBYTE tile)
{
	BltBitMapRastPort(g_pTileBMap, tile*16, 0, g_pRpMain, x*16,y*16, 16, 16, 0xc0);
}
//============================================================================
void PrintTxt(char* string, int x, int y)
{
	if(NULL == string)
	{
		return;
	}

	while(TRUE)
	{
		char c = *string;
		if (0 == c)
		{
			return;
		}

		LONG xSrc = ('z'-'a' + 1)*8;

		if (' ' != c)
		{
			xSrc = (c - 'a')*8;
		}


		string++;
		BltBitMapRastPort(g_pTileBMap, xSrc, 16, g_pRpMain, x, y, 8, 8, 0xC0);
		x += 8;
	}
}
//============================================================================
