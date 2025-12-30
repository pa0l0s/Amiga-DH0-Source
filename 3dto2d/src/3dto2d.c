#include "timer.h"
#include "input.h"
#include "window.h"
#include "main.h"
//#include "types.h"

#include <proto/exec.h>
#include <proto/graphics.h>
#include <dos/dos.h>
//============================================================================
struct IntuitionBase* IntuitionBase = NULL;
struct GfxBase* GfxBase = NULL;
//struct Library* DataTypesBase = NULL;
struct Library* UtilityBase = NULL;

//---------------------------------------------------------
static int init(void);
static void loop(void);
static void close(void);


static int initLibs(void);
static void closeLibs(void);
//---------------------------------------------------------
//============================================================================
int main(void)
{
	int iResult = RT_FAILED;
	iResult = init();
	if (RT_OK == iResult)
	{
		g_pFnc = &Title;
		loop();
	}

	close();

	printf("%i",iResult);

	return iResult;
}

//============================================================================

static int init(void)
{
	int iResult = initLibs();

	if (RT_OK != iResult)
	{
		return iResult;
	}

	iResult = initDefaultWindow();

	if (RT_OK != iResult)
	{
		return iResult;
	}

	iResult = initTimer();

	if (RT_OK != iResult)
	{
		return iResult;
	}

	iResult = InitInput();

	if (RT_OK != iResult)
	{
		return iResult;
	}

	//iResult = initTiles();

	if (RT_OK != iResult)
	{
		return iResult;
	}

		//iResult = initCube();

	if (RT_OK != iResult)
	{
		return iResult;
	}

	return RT_OK;
}

//============================================================================

static void close(void)
{
	//KillLevels();
	//killIntro();
	//killTiles();
	//killCube();
	KillInput();
	killTimer();
	killWindow();
	closeLibs();
}

//============================================================================

static void loop(void)
{
	BOOL bEnd = FALSE;

	while (!bEnd)
	{
		ULONG signals = Wait(g_nWindowSignal | g_nTimerSignal | g_nGamePortSignal | SIGBREAKF_CTRL_C);

		if (signals & SIGBREAKF_CTRL_C)
		{
			bEnd = TRUE;
		}

		if (signals & g_nWindowSignal)
		{
			bEnd = signalsWindow();
		}

		if (signals & g_nGamePortSignal)
		{
			bEnd = SignalsInput();
		}

		if (signals & g_nTimerSignal)
		{
			signalsTimer();
			(*g_pFnc)();
		}
	}
}

//============================================================================

static int initLibs(void)
{
	IntuitionBase = (struct IntuitionBase*)OpenLibrary("intuition.library", 34);
	if (NULL == IntuitionBase)
	{
		return RT_FAILED_OPEN_INTUITION;
	}

	GfxBase = (struct GfxBase*)OpenLibrary("graphics.library", 34);
	if (NULL == GfxBase)
	{
		return RT_FAILED_OPEN_GRAPHICS;
	}

//	DataTypesBase = OpenLibrary("datatypes.library",0);
//	if (NULL == DataTypesBase)
//	{
//		return RT_FAILED_OPEN_DATATYPES;
//	}

	UtilityBase = OpenLibrary("utility.library", 0);
	if (NULL == UtilityBase)
	{
		return RT_FAILED_OPEN_UTILITY;
	}

	return RT_OK;
}
//============================================================================
static void closeLibs(void)
{
	if (UtilityBase)
	{
		CloseLibrary(UtilityBase);
	}
//	if (DataTypesBase)
//	{
//		CloseLibrary(DataTypesBase);
//	}
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
