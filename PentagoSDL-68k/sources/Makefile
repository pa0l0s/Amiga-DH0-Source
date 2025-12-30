CC = gcc
OBJ = SDL_gfxPrimitives.o display.o game.o

all: pentagoSDL

pentagoSDL:	$(OBJ)
	$(CC) `sdl-config --libs` $(OBJ) -o $@ main.c -lSDL -lSDL_image -lSDL_ttf

SDL_gfxPrimitives.o:	SDL_gfxPrimitives.c
	$(CC) -c `sdl-config --cflags` $< -o $@

display.o:	display.c
	$(CC) -c `sdl-config --cflags` $< -o $@

game.o:	game.c

clean:
	rm -f $(OBJ) pentagoSDL
