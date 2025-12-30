#
# boxo
#

CC = vc
CFLAGS =-c99 -IWork:vbcc/targets/m68k-amigaos/include/ -IWork:vbcc/include_h/
#IDIR =../include
BIN = Work:/source/boxo
SRCDIR = src
ODIR = obj
#LDIR =../lib
LIBS =-LC:/vbcc/targets/m68k-amigaos/lib -lvc -lamiga
#LIBS = -lauto -lamiga
#LINK = vlink -bamigahunk -Bstatic -Cvbcc -nostdlib -s -x C:/vbcc/targets/m68k-amigaos/lib/startup.o

_DEPS = 
DEPS = $(patsubst %,$(IDIR)/%,$(_DEPS))

_OBJ = boxo.o window.o timer.o input.o level.o tile.o game.o fileIO.o intro.o
OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))
#LINKOBJ = boxo.o window.o timer.o input.o level.o tile.o game.o

RM = Delete

$(ODIR)/%.o:  $(SRCDIR)/%.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

exe/boxo: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS) $(LIBS)

.PHONY: all clean

#all: $(BIN)

clean:
	$(RM) $(OBJ) $(BIN)