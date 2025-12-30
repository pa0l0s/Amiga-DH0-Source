#include "input.h"

#include <proto/exec.h>
#include <exec/io.h>
#include <devices/gameport.h>
#include <devices/inputevent.h>
#include <intuition/intuition.h>
//============================================================================
BOOL g_bRight;
BOOL g_bLeft;
BOOL g_bUp;
BOOL g_bDown;
BOOL g_bFire;

ULONG g_nGamePortSignal;
//----------------------------------------------------------------------------
static struct MsgPort* m_pMsgPort = NULL;
static struct IOStdReq* m_pIO = NULL;
static BYTE m_error = 0;
static struct InputEvent m_event;

static void sendGamePortRequest(void);
static BYTE getControllerType(void);
static void setControllerType(BYTE type);
static void setTriggerConditions(void);
static void clearBuffer(void);
//============================================================================
int InitInput(void)
{
	m_pMsgPort = CreateMsgPort();

	if (NULL == m_pMsgPort)
	{
		return RT_FAILED_GAMEPORT_MSGPORT;
	}

	m_pIO = CreateIORequest(m_pMsgPort, sizeof(struct IOStdReq));

	if (NULL == m_pIO)
	{
		return RT_FAILED_GAMEPORT_IOREQ;
	}

	m_error = OpenDevice("gameport.device", 1, (struct IORequest*)m_pIO, 0);

	if (0 != m_error)
	{
		return RT_FAILED_GAMEPORT_DEVICE;
	}

	g_nGamePortSignal = 1L << m_pMsgPort->mp_SigBit;

	Forbid();
	const BYTE type = getControllerType();

	if (GPCT_NOCONTROLLER == type)
	{
		setControllerType(GPCT_ABSJOYSTICK);
	}

	Permit();
	setTriggerConditions();
	clearBuffer();
	sendGamePortRequest();

	return RT_OK;
}
//============================================================================
void KillInput(void)
{
	if (0 == m_error)
	{
		AbortIO((struct IORequest*)m_pIO);
		WaitIO((struct IORequest*)m_pIO);

		setControllerType(GPCT_NOCONTROLLER);

		CloseDevice((struct IORequest*)m_pIO);
	}

	if (m_pIO)
	{
		DeleteIORequest((struct IORequest*)m_pIO);
	}

	if (m_pMsgPort)
	{
		DeleteMsgPort(m_pMsgPort);
	}
}
//============================================================================
BOOL SignalsInput(void)
{
	BOOL bEnd = FALSE;

	while (TRUE)
	{
		struct IntuiMessage* pMsg = (struct IntuiMessage*)GetMsg(m_pMsgPort);

		if (NULL == pMsg)
		{
			break;
		}

		const UWORD button = m_event.ie_Code;

		if (IECODE_LBUTTON == button)
		{
			g_bFire = TRUE;
		}
		else if (IECODE_RBUTTON == button)
		{
			bEnd = TRUE;
		}

		const WORD x = m_event.ie_X;
		const WORD y = m_event.ie_Y;

		if (1 == x)
		{
			g_bRight = TRUE;
		}
		else if (-1 == x)
		{
			g_bLeft = TRUE;
		}

		if (1 == y)
		{
			g_bDown = TRUE;
		}
		else if (-1 == y)
		{
			g_bUp = TRUE;
		}
	}

	sendGamePortRequest();
	return bEnd;
}

//============================================================================
static void sendGamePortRequest(void)
{
	m_pIO->io_Command = GPD_READEVENT;
	m_pIO->io_Flags = 0;
	m_pIO->io_Data = &m_event;
	m_pIO->io_Length = (LONG)sizeof(struct InputEvent);
	SendIO((struct IORequest*)m_pIO);
}
//============================================================================
static BYTE getControllerType(void)
{
	BYTE result;

	m_pIO->io_Command = GPD_ASKCTYPE;
	m_pIO->io_Flags = IOF_QUICK;
	m_pIO->io_Data = (APTR)&result;
	m_pIO->io_Length = 1;
	DoIO((struct IORequest*)m_pIO);

	return result;
}
//============================================================================
static void setControllerType(BYTE type)
{
	BYTE data = type;
	m_pIO->io_Command = GPD_SETCTYPE;
	m_pIO->io_Flags = IOF_QUICK;
	m_pIO->io_Data  = (APTR)&data;
	m_pIO->io_Length = 1;
	DoIO((struct IORequest*)m_pIO);
}
//============================================================================
static void setTriggerConditions(void)
{
	struct GamePortTrigger gpt;
	gpt.gpt_Keys = GPTF_DOWNKEYS;
	gpt.gpt_Timeout = 0;
	gpt.gpt_XDelta = 1;
	gpt.gpt_YDelta = 1;
	m_pIO->io_Command = GPD_SETTRIGGER;
	m_pIO->io_Flags = IOF_QUICK;
	m_pIO->io_Data = &gpt;
	m_pIO->io_Length = (LONG)sizeof(struct GamePortTrigger);
	DoIO((struct IORequest*)m_pIO);
}
//============================================================================
static void clearBuffer(void)
{
	m_pIO->io_Command = CMD_CLEAR;
	m_pIO->io_Flags = IOF_QUICK;
	m_pIO->io_Data = NULL;
	m_pIO->io_Length = 0;
	DoIO((struct IORequest*)m_pIO);
}
//============================================================================
