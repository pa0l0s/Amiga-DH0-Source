enum Tiles
{
	TILE_EMPTY = 0,	/* przestrzeń kosmiczna */
	TILE_WALL = 1,	/* niezniszczalna asteroida */
	TILE_ROCK = 2,	/* skała, która rozpada się pod wpływem uderzenia statku 
			 * (zamiana na TILE_EMPTY)
			*/
	TILE_SHIP = 3,	/* nasz kosmiczny pojazd */
	TILE_EXIT = 4,	/* wyjście z planszy */
};
