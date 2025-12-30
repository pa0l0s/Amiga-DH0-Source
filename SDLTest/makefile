SDLTest: main.o
	g++ -o SDLTest main.o libs/c2p8_040_asm.o libs/c2p8_040_dbl_asm.o -LSYS:SDL_AGA/SDL/bin/ -lSDL_AGA_060 -ldebug -lamiga -lpthread -noixemul
main.o: main.cpp
	g++ -o main.o -c main.cpp -I SYS:SDL_AGA/SDL/include/ 
