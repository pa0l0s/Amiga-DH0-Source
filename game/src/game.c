#include "game.h"

#include "level.h"
#include "tile.h"
#include "input.h"
#include "intro.h"
#include <proto/graphics.h>
#include <proto/dos.h>
//============================================================================
PVF g_pFnc = NULL;
int g_nLives;

static LONG m_nPosX = 0;
static int m_nVelocityX = 0;
static int m_nVelocityY = 0;
static LONG m_nPosY = 0;
static BOOL m_bShipStand;
static int m_nHotX = 0;
static int m_nHotY = 0;


static BOOL beyondBounds(void);
static void titleLoop(void);
static void gameOverLoop(void);
static void moveShip(void);
static void findShipPositionOnLevel(void);
//============================================================================
void Title(void)
{
	CleanLevel();

	PrintTxt("boxo", 128,64);

	PrintTxt("press key a to start", 64, 96);
	g_pFnc = &titleLoop;
}
//============================================================================
static void titleLoop(void)
{
	if (g_bFire)
	{
		g_bFire = FALSE;
		Delay(10);
		g_pFnc = &Intro;
	}
}
//============================================================================
void NewGame(void)
{
	g_nLives = 3;
	g_nLvlNumber = 0;

	g_pFnc = &NextLevel;
}
//============================================================================
void GameLoop(void)
{
	if (beyondBounds())
	{
		g_nLives--;

		if (0 == g_nLives)
		{
			g_pFnc = &GameOver;
			return;
		}

		RockLevel();
		
		g_pFnc = &NextLevel;
		return;
	}

	moveShip();
}
//============================================================================
void NextLevel(void)
{
	m_nVelocityX = 0;
	m_nVelocityY = 0;

	m_bShipStand = TRUE;
	m_nHotX = 0;
	m_nHotY = 0;

	if (g_nLvlNumber >= g_nAmountOfLvl)
	{
		g_pFnc = &Outro;
		return;
	}

	CopyLvl(g_nLvlNumber);
	LvlToWin();
	findShipPositionOnLevel();

	g_pFnc = &GameLoop;
}
//============================================================================
void GameOver(void)
{
	CleanLevel();
	PrintTxt("game over", 120, 88);
	g_bFire =FALSE;
	g_pFnc = &gameOverLoop;
}

static void gameOverLoop(void)
{
	if (g_bFire)
	{
		g_bFire = FALSE;
		g_pFnc = &Title;
	}
}
//============================================================================
void Intro(void)
{
	int r;
	PrintTxt("intro", 128,128);

	if (g_bFire)
	{
	    PrintTxt("intro gbfire", 128,128);
		g_bFire = FALSE;
		CleanLevel();
		g_pFnc = &NewGame;
	}
	else
	{
	    PrintTxt("intro go", 128,128);
		r = runIntro();
	}
}
//============================================================================
void Outro(void)
{
	g_pFnc = &Title;
}
//============================================================================
static BOOL beyondBounds(void)
{
	if (m_nPosX < 1 || m_nPosX > 319)
	{
		return TRUE;
	}

	if (m_nPosY < 1 || m_nPosY > 191)
	{
		return TRUE;
	}

	return FALSE;
}
//============================================================================
static void moveShip(void)
{
	UBYTE tile;

	if (m_bShipStand)
	{
		if (g_bLeft)
		{
			m_bShipStand = FALSE;
			g_bLeft = FALSE;
			m_nHotX = 0;
			m_nVelocityX = -4;
			m_nVelocityY = 0;
		}
		else if (g_bRight)
		{
			m_nHotX = 15;
			m_bShipStand = FALSE;
			g_bRight = FALSE;
			m_nVelocityX = 4;
			m_nVelocityY = 0;
		}

		if (g_bUp)
		{
			m_nHotY = 0;
			m_bShipStand = FALSE;
			g_bUp = FALSE;
			m_nVelocityX = 0;
			m_nVelocityY = -4;
		}
		else if (g_bDown)
		{
			m_nHotY = 15;
			m_bShipStand = FALSE;
			g_bDown = FALSE;
			m_nVelocityX = 0;
			m_nVelocityY = 4;
		}
	}

	int nPosX = (m_nPosX + m_nHotX + m_nVelocityX )/16;
	int nPosY = (m_nPosY + m_nHotY + m_nVelocityY)/16;

	int nTabPos = nPosX + nPosY*20;
	tile = g_pLevel[nTabPos];

	if (TILE_WALL == tile)
	{
		m_nVelocityX = 0;
		m_nVelocityY = 0;
		m_bShipStand = TRUE;
		g_bLeft = FALSE;
		g_bRight = FALSE;
		g_bUp = FALSE;
		g_bDown = FALSE;
	}
	else if (TILE_ROCK == tile)
	{
		m_nVelocityX = 0;
		m_nVelocityY = 0;
		m_bShipStand = TRUE;
		g_bLeft = FALSE;
		g_bRight = FALSE;
		g_bUp = FALSE;
		g_bDown = FALSE;
		g_pLevel[nTabPos] = 0;
		PasteTile(nPosX, nPosY, TILE_EMPTY);
	}
	else if (TILE_EXIT == tile)
	{
		m_bShipStand = TRUE;
		g_bLeft = FALSE;
		g_bRight = FALSE;
		g_bUp = FALSE;
		g_bDown = FALSE;
		g_nLvlNumber++;
		g_pFnc = &NextLevel;
		return;
	}

	EraseShip(m_nPosX, m_nPosY);

	m_nPosX += m_nVelocityX;
	m_nPosY += m_nVelocityY;

	PasteShip(m_nPosX, m_nPosY);

}
//============================================================================
static void findShipPositionOnLevel(void)
{
	int x;
	int y;
	UBYTE* p = g_pLevel;

	for (y = 0; y < g_nLvlTHeight; y++)
	{
		for (x = 0; x < g_nLvlTWidth; x++)
		{
			UBYTE tile = *p;

			if (TILE_SHIP == tile)
			{
				m_nPosX = x * 16;
				m_nPosY = y * 16;
				*p = TILE_EMPTY;
				return;
			}
			p++;
		}
	}
}
//============================================================================
