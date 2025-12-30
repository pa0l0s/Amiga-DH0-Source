#include "timer.h"

#include <proto/exec.h>
#include <devices/timer.h>
//============================================================================
ULONG g_nTimerSignal;

static BOOL m_bTimerWasSent = FALSE;
static struct MsgPort* m_pMsgTimerPort = NULL;
static struct timerequest* m_pTimerIO = NULL;
static struct timeval m_tv;
//--------------------------------------------------------
static void sendTimerReq(void);
//============================================================================
int initTimer(void)
{
	m_pMsgTimerPort = CreateMsgPort();

	if (0 == m_pMsgTimerPort)
	{
		return -1;
	}

	m_pTimerIO = (struct timerequest*) 
		CreateIORequest(m_pMsgTimerPort, sizeof(struct timerequest));

	if (0 == m_pTimerIO)
	{
		return -1;
	}

	LONG error = OpenDevice(TIMERNAME, UNIT_VBLANK,
		(struct IORequest*)m_pTimerIO, 0);

	if (0 != error)
	{
		return -1;
	}

	g_nTimerSignal = 1L << m_pMsgTimerPort->mp_SigBit;

	m_pTimerIO->tr_node.io_Command = TR_ADDREQUEST;
	m_pTimerIO->tr_time = m_tv;

	sendTimerReq();
	m_bTimerWasSent = TRUE;

	return 0;	//ok
}
//============================================================================
void killTimer(void)
{
	if (m_pTimerIO)
	{
		if (m_bTimerWasSent)
		{
			AbortIO((struct IORequest*)m_pTimerIO);
			WaitIO((struct IORequest*)m_pTimerIO);
		}
		CloseDevice((struct IORequest*)m_pTimerIO);
		DeleteIORequest(m_pTimerIO);
	}

	if (m_pMsgTimerPort)
	{
		DeleteMsgPort((struct MsgPort*)m_pMsgTimerPort);
	}
}
//============================================================================
void signalsTimer(void)
{
	while (TRUE)
	{
		struct IntuiMessage* pMsg =
			(struct IntuiMessage*)GetMsg(m_pMsgTimerPort);

		if (NULL == pMsg)
		{
			break;
		}
	}

	sendTimerReq();
}
//============================================================================
static void sendTimerReq(void)
{
	m_tv.tv_secs = 0;
	m_tv.tv_micro = 20000;
	m_pTimerIO->tr_time = m_tv;

	SendIO((struct IORequest*)m_pTimerIO);
}
//============================================================================
