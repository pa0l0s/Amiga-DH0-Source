
supertux: main.o
	g++ -o /bin/supertux /obj/main.o /libs/c2p8_040_asm.o /libs/c2p8_040_dbl_asm.o -LSYS:SDL_AGA/SDL/bin/ -lSDL_AGA_040 -ldebug -lamiga -lpthread -noixemul
main.o: src/main.cpp
	cd src
	g++ -o /obj/main.o -c src/main.cpp -I SYS:SDL_AGA/SDL/include/
