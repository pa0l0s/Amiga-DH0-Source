#include "window.h"
#include "input.h"

#include <proto/exec.h>
#include <proto/intuition.h>
#include <proto/graphics.h>
#include <intuition/intuition.h>
//============================================================================
#define KEY_ESC 0x45
#define KEY_A 0x20
#define KEY_CURSOR_UP 0x4c
#define KEY_CURSOR_DOWN 0x4d
#define KEY_CURSOR_RIGHT 0x4e
#define KEY_CURSOR_LEFT 0x4f
//--------------------------------------------------------
ULONG g_tabPens[PENS_AMOUNT];
ULONG g_nWindowSignal;
struct RastPort* g_pRpMain = NULL;
static struct Window* m_pWin = NULL;
static BOOL m_bPensObtained = FALSE;
//--------------------------------------------------------
static void getPens(void);
static void freePens(void);
//============================================================================
int initWindow(void)
{
	m_pWin = (struct Window*)OpenWindowTags(NULL,
		WA_Left, 0, WA_Top, 0,
		WA_Width, 320, WA_Height, 256,
		WA_CloseGadget, TRUE,
		WA_Title, (ULONG)"boxo",
		WA_Activate, TRUE,
		WA_DragBar, TRUE,
		WA_GimmeZeroZero, TRUE,
		WA_IDCMP, IDCMP_CLOSEWINDOW | IDCMP_RAWKEY,
		TAG_END);

	if (NULL == m_pWin)
	{
		return -1;
	}

	getPens();

	g_nWindowSignal = 1L << m_pWin->UserPort->mp_SigBit;
	g_pRpMain = m_pWin->RPort;

	return 0;	//OK
}
//============================================================================
void killWindow(void)
{
	freePens();

	if (m_pWin)
	{
		CloseWindow(m_pWin);
	}
}
//============================================================================
BOOL signalsWindow(void)
{
	BOOL bEnd = FALSE;

	while (TRUE)
	{
		struct IntuiMessage* pMsg = (struct IntuiMessage*)
		GetMsg(m_pWin->UserPort);

		if (NULL == pMsg)
		{
			break;
		}

		ULONG msg_class = pMsg->Class;
		UWORD msg_code = pMsg->Code;

		ReplyMsg((struct Message*)pMsg);

		if (IDCMP_CLOSEWINDOW == msg_class)
		{
			bEnd = TRUE;
		}
		else if (IDCMP_RAWKEY == msg_class)
		{
			switch (msg_code)
			{
				case KEY_ESC: bEnd = TRUE; break;
				case KEY_A: g_bFire = TRUE; break;
				case KEY_CURSOR_UP: g_bUp = TRUE; break;
				case KEY_CURSOR_DOWN: g_bDown = TRUE; break;
				case KEY_CURSOR_RIGHT: g_bRight = TRUE; break;
				case KEY_CURSOR_LEFT: g_bLeft = TRUE; break;
				default: break;
			}
		}
	}

	return bEnd;
}
//============================================================================
static void getPens(void)
{
	typedef struct
	{
		ULONG red;
		ULONG green;
		ULONG blue;
	} Color;

	Color colors[PENS_AMOUNT] =
	{
		{0x00000000, 0x00000000, 0x00000000},	//BLACK
		{0x7fffffff, 0x7fffffff, 0x7fffffff},	//GREY
	};

	int i;
	struct ColorMap* cm = m_pWin->WScreen->ViewPort.ColorMap;

	for (i = 0; i < PENS_AMOUNT; i++)
	{
		ULONG red = colors[i].red;
		ULONG green = colors[i].green;
		ULONG blue = colors[i].blue;
		ULONG pen = ObtainPen(cm, 0xffffffff, red, green, blue, PEN_EXCLUSIVE);

		if( -1 == pen )
		{
			pen = ObtainBestPenA(cm, red, green, blue, NULL);
		}
		g_tabPens[i] = pen;
	}

	m_bPensObtained = TRUE;
}
//============================================================================
static void freePens(void)
{
	if (m_bPensObtained)
	{
		int i;
		for (i = 0; i < PENS_AMOUNT; i++)
		{
			ReleasePen(m_pWin->WScreen->ViewPort.ColorMap, g_tabPens[i]);
		}
	}
}
//============================================================================
