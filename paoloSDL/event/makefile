event: event.o
	g++ -o event event.o libs/c2p8_040_asm.o libs/c2p8_040_dbl_asm.o -LSYS:SDL_AGA/SDL/bin/ -lSDL_AGA_040 -ldebug -lamiga -lpthread -noixemul
event.o: event.cpp
	g++ -o event.o -c event.cpp -I SYS:SDL_AGA/SDL/include/
