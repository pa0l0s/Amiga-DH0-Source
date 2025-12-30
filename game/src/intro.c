#include "intro.h"
#include "window.h"

#include <proto/exec.h>
#include <proto/graphics.h>
#include <proto/intuition.h>
#include <proto/datatypes.h>
#include <datatypes/pictureclass.h>
#include <proto/dos.h>
//============================================================================
int g_nIntroWidth;
int g_nIntroHeight;
int g_nIntroFrameNr;
int g_nIntroFramesPerRow;

struct BitMap* g_pIntroBMap = NULL;
struct RastPort* g_pIntroRPort = NULL;
//----------------------------------------------------------------------------
struct RastPort m_RPortIntro;
int m_nIntroRowNr;
int m_nIntroMaxRows;
//----------------------------------------------------------------------------

int initIntro(void)
{
	int iResult = RT_FAILED_LOAD_INTROS_PIC;

	g_nIntroWidth = 64;
	g_nIntroHeight = 64;
	g_nIntroFrameNr = 0;
	g_nIntroFramesPerRow = 10; //640/32=20;
	m_nIntroRowNr=0;
	m_nIntroMaxRows=4;

	struct Screen* pWB = LockPubScreen(NULL);

	Object* o = NewDTObject("intro.pic",
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

		g_pIntroBMap = AllocBitMap(nWidth, nHeight, nDepth, BMF_DISPLAYABLE|BMF_CLEAR, 0);

		if (NULL != g_pIntroBMap)
		{
			BltBitMap(bm, 0, 0, g_pIntroBMap, 0, 0, nWidth, nHeight, 0xC0, 0xFF, NULL);

			g_pIntroRPort = &m_RPortIntro;
			InitRastPort(g_pIntroRPort);
			g_pIntroRPort->BitMap = g_pIntroBMap;

			iResult = RT_OK;
		}

		DisposeDTObject(o);
	}

	UnlockPubScreen(NULL, pWB);

	return iResult;
}
void killIntro(void)
{
	FreeBitMap(g_pIntroBMap);
}
int runIntro(void) //intro loop
{
	//PrintTxt("run intro", 2,16);
	//PrintTxt("a", 16,32+(16*g_nIntroFrameNr));
	int iResult = RT_FAILED_INTRO;
	int x=60; //intro position
	int y=30; //intro position
	int irow;
	//irow = g_nIntroFramesPerRow/g_nIntroFrameNr;

	BltBitMapRastPort(g_pIntroBMap, g_nIntroWidth * g_nIntroFrameNr, m_nIntroRowNr * g_nIntroHeight, g_pRpMain, x,y, g_nIntroWidth, g_nIntroHeight, 0xc0);
	BltBitMapRastPort(g_pIntroBMap, g_nIntroWidth * g_nIntroFrameNr, m_nIntroRowNr * g_nIntroHeight, g_pRpMain, x+120,y, g_nIntroWidth, g_nIntroHeight, 0xc0);

	g_nIntroFrameNr++;

	if (g_nIntroFrameNr >= g_nIntroFramesPerRow )
	{
		g_nIntroFrameNr = 0;
		m_nIntroRowNr++;
	}
	if (m_nIntroRowNr >= m_nIntroMaxRows )
	{
		m_nIntroRowNr = 0;
	}

	//Delay(2);

	//CleanLevel();

	iResult = RT_OK;

	return iResult;
}
