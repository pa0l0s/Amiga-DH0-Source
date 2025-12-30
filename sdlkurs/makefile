CC=gcc
CFLAGS=-ISYS:SDL/include -amiga -noixemul
#-amiga -fpu=68040 -noixemul
DEPS =
OBJ = pierwszysdl.o

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

pierwszysdl: $(OBJ)
	$(CC) -o $@ $^ SYS:SDL/bin/SDL_AGA_040.a SYS:SDL/support_libs/c2p8_040_dbl_asm.o SYS:SDL/support_libs/c2p8_040_asm.o $(CFLAGS) 
	

clean:
	delete pierwszysdl $(OBJ)