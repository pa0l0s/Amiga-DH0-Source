#include "window.h"
#include "timer.h"
#include "input.h"

#include <proto/exec.h>
#include <proto/graphics.h>
#include <dos/dos.h>
//============================================================================
struct IntuitionBase* IntuitionBase;
struct GfxBase* GfxBase;
PVF g_pFnc = NULL;
//---------------------------------------------------------
static int init(void);
static void loop(void);
static void close(void);
static void putShip(void);
//---------------------------------------------------------
static LONG m_nPosX = 0;
static int m_nVelocityX = 0;
static int m_nVelocityY = 0;
static LONG m_nPosY = 0;
static BOOL m_bShipStand;
//============================================================================
int main(void)
{
	if (0 == init())
	{
		m_nPosX = 200;
		m_nPosY = 120;
		m_nVelocityX = 0;
		m_nVelocityY = 0;
		g_pFnc = &putShip;
		m_bShipStand = TRUE;
		SetRast(g_pRpMain,g_tabPens[0]);
		SetAPen(g_pRpMain,g_tabPens[0]);
		SetBPen(g_pRpMain,g_tabPens[1]);
		SetOutlinePen(g_pRpMain, g_tabPens[1]);
		RectFill(g_pRpMain,16,16,288,224);
		SetOutlinePen(g_pRpMain, g_tabPens[0]);

		loop();
	}

	close();

	return RETURN_OK;	//ok
}
//============================================================================
static int init(void)
{
	IntuitionBase = (struct IntuitionBase*)OpenLibrary("intuition.library", 34);
	if (NULL == IntuitionBase)
	{
		return -1;
	}
	GfxBase = (struct GfxBase*)OpenLibrary("graphics.library", 34);
	if (NULL == GfxBase)
	{
		return -1;
	}
	//-----------------------------------------------------
	if ( 0 != initWindow())
	{
		return -1;
	}
	if ( 0 != initTimer())
	{
		return -1;
	}
	//-----------------------------------------------------
	return 0;	//ok
}
//============================================================================
static void close(void)
{
	killTimer();
	killWindow();
	//-----------------------------------------------------
	if (IntuitionBase)
	{
		CloseLibrary((struct Library*)IntuitionBase);
	}
	if (GfxBase)
	{
		CloseLibrary((struct Library*)GfxBase);
	}
}
//============================================================================
static void loop(void)
{
	BOOL bEnd = FALSE;

	while (!bEnd)
	{
		ULONG signals = Wait(g_nWindowSignal | g_nTimerSignal | SIGBREAKF_CTRL_C);

		if (signals & SIGBREAKF_CTRL_C)
		{
			bEnd = TRUE;
		}
		if (signals & g_nWindowSignal)
		{
			bEnd = signalsWindow();
		}
		if (signals & g_nTimerSignal)
		{
			signalsTimer();
			(*g_pFnc)();
		}
	}
}
//============================================================================
static void putShip(void)
{
	SetAPen(g_pRpMain, g_tabPens[0]);
	RectFill(g_pRpMain, m_nPosX, m_nPosY, m_nPosX + 8, m_nPosY + 8);

	if (m_bShipStand)
	{
		if (g_bLeft)
		{
			m_bShipStand = FALSE;
			g_bLeft = FALSE;
			m_nVelocityX = -2;
			m_nVelocityY = 0;
		}
		else if (g_bRight)
		{
			m_bShipStand = FALSE;
			g_bRight = FALSE;
			m_nVelocityX = 2;
			m_nVelocityY = 0;
		}

		if (g_bUp)
		{
			m_bShipStand = FALSE;
			g_bUp = FALSE;
			m_nVelocityX = 0;
			m_nVelocityY = -2;

		}
		else if (g_bDown)
		{
			m_bShipStand = FALSE;
			g_bDown = FALSE;
			m_nVelocityX = 0;
			m_nVelocityY = 2;
		}
	}

	ULONG nX = 4+m_nPosX + 3*m_nVelocityX;
	ULONG nY = 4+m_nPosY + 3*m_nVelocityY;

	ULONG nColor = ReadPixel(g_pRpMain, nX, nY);

	if (g_tabPens[1] == nColor)
	{
		m_nVelocityX = 0;
		m_nVelocityY = 0;
		m_bShipStand = TRUE;
	}
	else
	{
		m_nPosX += m_nVelocityX;
		m_nPosY += m_nVelocityY;
	}

	SetAPen(g_pRpMain, g_tabPens[1]);
	RectFill(g_pRpMain, m_nPosX, m_nPosY, m_nPosX + 8, m_nPosY + 8);
}
//============================================================================
