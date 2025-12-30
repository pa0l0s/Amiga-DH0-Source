#
# portrend
#

CC = vc
CFLAGS =-c99 -IWork:vbcc/targets/m68k-amigaos/include/ -IWork:vbcc/include_h/ -fpu=68040
IDIR = dh0:SDL/include
BIN = portrend
SRCDIR = src
ODIR = obj
#LDIR =../lib
LIBS =-LC:/vbcc/targets/m68k-amigaos/lib -lvc -lamiga
#LIBS = -lauto -lamiga
#LINK = vlink -bamigahunk -Bstatic -Cvbcc -nostdlib -s -x C:/vbcc/targets/m68k-amigaos/lib/startup.o

_DEPS = 
DEPS = $(patsubst %,$(IDIR)/%,$(_DEPS))

_OBJ = portrend.o #main.o window.o timer.o input.o cube.o engine.o
OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))
#LINKOBJ = boxo.o window.o timer.o input.o level.o tile.o game.o

RM = Delete

$(ODIR)/%.o:  $(SRCDIR)/%.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS) -I$(IDIR)

$(BIN): $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS) $(LIBS)

.PHONY: all clean

all: $(BIN)

clean:
	$(RM) $(OBJ) $(BIN)