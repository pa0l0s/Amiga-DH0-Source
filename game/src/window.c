#include "window.h"
#include "input.h"

#include <proto/exec.h>
#include <proto/intuition.h>
#include <proto/graphics.h>
#include <intuition/intuition.h>
#include <proto/utility.h>
//============================================================================
#define KEY_ESC 0x45
#define KEY_A 0x20
#define KEY_CURSOR_UP 0x4c
#define KEY_CURSOR_DOWN 0x4d
#define KEY_CURSOR_RIGHT 0x4e
#define KEY_CURSOR_LEFT 0x4f
//--------------------------------------------------------
ULONG g_nWindowSignal;
int g_nWinWidth;
int g_nWinHeight;
struct ViewPort* g_pViewPort = NULL;
struct RastPort* g_pRpMain = NULL;
static struct Window* m_pWin = NULL;
//--------------------------------------------------------
static struct TagItem m_windowTags[] =
{
	{WA_Left, 200},
	{WA_Top, 0},
	{WA_InnerWidth, 0},
	{WA_InnerHeight, 0},
	{WA_CloseGadget, TRUE},
	{WA_Title, (ULONG)"boxo"},
	{WA_Activate, TRUE},
	{WA_DragBar, TRUE},
	{WA_GimmeZeroZero, TRUE},
	{WA_IDCMP, IDCMP_CLOSEWINDOW | IDCMP_RAWKEY},
	{TAG_END, TAG_END}
};

static void setWindowTag(Tag name, Tag value);
//============================================================================
int initWindow(void)
{
	g_nWinWidth = 320;
	g_nWinHeight = 192;

	setWindowTag(WA_InnerWidth, g_nWinWidth);
	setWindowTag(WA_InnerHeight, g_nWinHeight);

	m_pWin = (struct Window*)OpenWindowTagList(NULL, m_windowTags);

	if (NULL == m_pWin)
	{
		return -1;
	}

	g_nWindowSignal = 1L << m_pWin->UserPort->mp_SigBit;
	g_pRpMain = m_pWin->RPort;
	g_pViewPort = &m_pWin->WScreen->ViewPort;

	return RT_OK;
}
//============================================================================
void killWindow(void)
{
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
static void setWindowTag(Tag name, Tag value)
{
	struct TagItem* pTag = FindTagItem(name, m_windowTags);

	if (pTag)
	{
		pTag->ti_Data = value;
	}
}
//============================================================================
 