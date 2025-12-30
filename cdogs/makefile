#
# boxo
#

CC = gcc
CFLAGS =-Isrc/include
#IDIR =../include
BIN = cdogs
SRCDIR = src
ODIR = obj
#LDIR =../lib
LIBS = -lvc -lauto -lamiga
#LIBS = -lauto -lamiga
#LINK = vlink -bamigahunk -Bstatic -Cvbcc -nostdlib -s -x C:/vbcc/targets/m68k-amigaos/lib/startup.o

_DEPS = 
DEPS = $(patsubst %,$(IDIR)/%,$(_DEPS))

_OBJ = cdogs.o actors.o ai.o automap.o defs.o draw.o events.o files.o game.o gamedata.o grafx.o hiscores.o input.o joystick.o keyboard.o mainmenu.o map.o menu.o mission.o objs.o password.o pics.o prep.o sounds.o sprcomp.o text.o triggers.o
OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))
#LINKOBJ = boxo.o window.o timer.o input.o level.o tile.o game.o

RM = Delete

$(ODIR)/%.o:  $(SRCDIR)/%.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

cdogs: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS) $(LIBS)

.PHONY: all clean

#all: $(BIN)

clean:
	$(RM) $(OBJ) $(BIN)