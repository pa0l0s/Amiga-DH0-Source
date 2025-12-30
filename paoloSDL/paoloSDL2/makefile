paoloSDL: paoloSDL.o
	g++ -o paoloSDL paoloSDL.o libs/c2p8_040_asm.o libs/c2p8_040_dbl_asm.o -LSYS:SDL_AGA/SDL/bin/ -lSDL_AGA_040 -ldebug -lamiga -lpthread -noixemul
paoloSDL.o: paoloSDL.cpp
	g++ -o paoloSDL.o -c paoloSDL.cpp -I SYS:SDL_AGA/SDL/include/
