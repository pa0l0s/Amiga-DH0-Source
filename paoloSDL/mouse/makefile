mouse: mouse.o
	g++ -o mouse mouse.o libs/c2p8_040_asm.o libs/c2p8_040_dbl_asm.o -LSYS:SDL_AGA/SDL/bin/ -lSDL_AGA_040 -ldebug -lamiga -lpthread -noixemul
mouse.o: mouse.cpp
	g++ -o mouse.o -c mouse.cpp -I SYS:SDL_AGA/SDL/include/
