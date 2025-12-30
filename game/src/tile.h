#ifndef BOXO_TILE_H
#define BOXO_TILE_H
//============================================================================
#include "types.h"
//--------------------------------------------------------

enum Tiles
{
	TILE_EMPTY = 0,
	TILE_WALL = 1,
	TILE_ROCK = 2,
	TILE_SHIP = 3,
	TILE_EXIT = 4,
};

extern struct BitMap* g_pTileBMap;

extern int g_nTileWidth;
extern int g_nTileHeight;


extern int initTiles(void);
extern void killTiles(void);


extern void EraseShip(int x, int y);
extern void PasteShip(int x, int y);

extern void PasteTile(int x, int y, UBYTE tile);

extern void PrintTxt(char* string, int x, int y);

//============================================================================
#endif // BOXO_TILE_H
