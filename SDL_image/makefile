showp: showimage.o
	gcc -o showp showimage.o libs/c2p8_040_asm.o libs/c2p8_040_dbl_asm.o -LSYS:SDL_AGA/SDL/bin/ -Llib/ -lSDL_AGA_040 -ldebug -lamiga -lpthread -lpng -lz -ljpeg -lSDL_image -noixemul
showimage.o: showimage.c
	gcc -o showimage.o -c showimage.c -I SYS:SDL_AGA/SDL/include/ -I include/SDL
