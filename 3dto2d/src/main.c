#include "main.h"
#include "input.h"
#include "window.h"
#include "cube.h"
#include <proto/graphics.h>
//============================================================================
PVF g_pFnc = NULL;

static void titleLoop(void);

//============================================================================
void Title(void)
{

	//CleanLevel();

	printText("3d to 2d", getTextXCenter("3d to 2d"),64,2);

	//PrintTxt("press key a to start", 64, 96);
	g_pFnc = &titleLoop;
}
//============================================================================
static void titleLoop(void)
{
	if (g_bFire)
	{
		g_bFire = FALSE;
		g_pFnc = &Cube;
	}
}

void Cube(void)
{
	runCube();
}
