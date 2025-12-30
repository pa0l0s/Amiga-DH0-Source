int initTiles(void)
{
	/* zakładamy, że nie uda nam się załadować obrazka*/
	int iResult = RT_FAILED_LOAD_TILES_PIC;	

	g_nTileWidth = 16;	/* ustalamy rozmiar kafla, tu szerokość */
	g_nTileHeight = 16;	/* wysokość kafla */

	/* zakładamy kłódkę na ekran Workbencha, tak by nie mógł być zamknięty, kiedy my chcemy użyć
	 * jego właściwości, które przydadzą się w trakcie remapowania obrazka
	*/
	struct Screen* pWB = LockPubScreen(NULL);

	/* tworzymy obiekt, który zgodnie z DTA_GroupID ma być obrazkiem, i będzie
	 * przetworzony (zremapowany) na podstawie ekranu Workbencha w tym przypadku.
	 * Dalsze objaśnienia argumentów:
	 * "tile.pic" - nazwa pliku, który ma się znajdować w tej samej lokalizacji co plik wykonywalny
	 * DTA_GroupID, GID_PICTURE - ten tag, określa typ obiektu
	 * PDTA_Remap, TRUE - tag specyficzny dla picture.datatype, mówiący że obrazek ma być remapowany
	 * PDTA_Screen, pWb - tag zawierający wskaźnik do ekranu do którego będziemy prztwarzać obrazek
	*/
	Object* o = NewDTObject("tile.pic",
		DTA_GroupID, GID_PICTURE,
		PDTA_Remap, TRUE,
		PDTA_Screen, pWB,
		TAG_END);


	if (o)
	{
		struct BitMap* bm;
		
		/* przeprowadzamy działanie na obiekcie, w tym przypadku wykonujemy
		 * metodę DTM_PROCLAYOUT, która właśnie remapuje obrazek
		*/
		DoDTMethod(o, NULL, NULL, DTM_PROCLAYOUT,NULL,TRUE);

		/* dobieramy się do zremapowanej bitmapy poprzez pobieranie 
		 * atrybutu obiektu
		*/
		GetDTAttrs(o,
			PDTA_DestBitMap, &bm,
			TAG_END);

		/* bierzemy szerokość, wysokość i głębokość bitmapy tejże bitmapy.
		 * Dane te posłużą nam do stworzenia kopii bitmapy, bo trzeba 
		 * pamiętać, że bitmapa pobrana za pomocą GetDTAttrs zostanie
		 * automatycznie zwolniona po skasowaniu obiektu.
		*/

		ULONG nWidth = GetBitMapAttr(bm, BMA_WIDTH);
		ULONG nHeight = GetBitMapAttr(bm, BMA_HEIGHT);
		ULONG nDepth = GetBitMapAttr(bm, BMA_DEPTH);

		g_pTileBMap = AllocBitMap(nWidth, nHeight, nDepth, BMF_DISPLAYABLE|BMF_CLEAR, 0);

		if (NULL != g_pTileBMap)
		{
			/* robimy kopię naszej bitmapy */
			BltBitMap(bm, 0, 0, g_pTileBMap, 0, 0, nWidth, nHeight, 0xC0, 0xFF, NULL);

			/* i tworzymy rastport, który ją zawiera */
			g_pTileRPort = &m_RPortTile;
			InitRastPort(g_pTileRPort);
			g_pTileRPort-&gt;BitMap = g_pTileBMap;

			/* a jednak nam się udało pomyślnie załadować obrazek */
			iResult = RT_OK;
		}

		/* kasujemy obiekt*/
		DisposeDTObject(o);
	}

	/* zdejmujemy naszą kłódkę z ekranu Workbencha */
	UnlockPubScreen(NULL, pWB);

	return RT_OK;
}

/*
Funkcja killTiles, jak łatwo się domyślić, zwalnia bitmapę, którą otrzymaliśmy w funkcji initTiles. Trzy kolejne funkcje służą do kopiowania kafli na ekran i nie wymagają większego komentarza - jest to zwyczajowe wykorzystanie funkcji BltBitMapRastPort.
*/

void EraseShip(int x, int y)
{
	BltBitMapRastPort(g_pTileBMap, TILE_EMPTY*16, 0, g_pRpMain, x,y, 16, 16, 0xc0);
}
//============================================================================
void PasteShip(int x, int y)
{
	BltBitMapRastPort(g_pTileBMap, TILE_SHIP*16, 0, g_pRpMain, x,y, 16, 16, 0xc0);
}
//============================================================================
void PasteTile(int x, int y, UBYTE tile)
{
	BltBitMapRastPort(g_pTileBMap, tile*16, 0, g_pRpMain, x*16,y*16, 16, 16, 0xc0);
}

/*
Należy wspomnieć, że w obrazek sam upichciłem i należy go traktować jako dzieło eklektyczne i pole do popisu dla grafików. Kafle znajdują się w górnym lewym rogu i tak blok od (0,0) do (15,15) to TILE_EMPTY, kostka od (15,0) do (31,0) to TILE_WALL i tak dalej. Oprócz kafli znajdują się czcionki (fonty) 8x8, które także stworzyłem w całe 10 minut. Jako miły dodatek zrobiłem też cyfry, a także sam stworzyłem dwie plansze. W każdym razie zachęcam do eksperymentów nad obrazkiem, planszami i grą. Dodałem też funkcję, która wyświetla napis na ekranie gry - napis ograniczony jest do liter łacińskich i spacji.
*/

void PrintTxt(char* string, int x, int y)
{
	/* pustego napisu nie pokazuj */
	if(NULL == string)
	{
		return;
	}
	while(TRUE)
	{
		/* weź znak i sprawdz czy to koniec napisu */
		char c = *string;
		if (0 == c)
		{
			return;
		}
		/* zakładamy, że litera jest spacją */
		LONG xSrc = ('z'-'a' + 1)*8;
		if (' ' != c)
		{	/* litera jednak nie jest spacją */
			xSrc = (c - 'a')*8;
		}
		string++;
		/* kopiowanie znaku na ekran */
		BltBitMapRastPort(g_pTileBMap, xSrc, 16, g_pRpMain, x, y, 8, 8, 0xC0);
		x += 8;
	}
}
