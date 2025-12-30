#
# boxo
#

CC = gcc
CFLAGS = -noixemul
IDIR =SYS:SDL_AGA/SDL/include/
BIN = cdogs
SRCDIR = /src
ODIR = /obj
LDIR =SYS:SDL_AGA/SDL/bin/
LIBS =  -lvc -lSDL_AGA_040 -ldebug -lamiga -lpthread
#LIBS = -lauto -lamiga
#LINK = vlink -bamigahunk -Bstatic -Cvbcc -nostdlib -s -x C:/vbcc/targets/m68k-amigaos/lib/startup.o


# this variable contains any extra compiler options that we might
# want to add, like -O3, -march=native, etc.
# -O3 tells g++ to optimize the code for speed
CC_OPTS:=-O3


# this variable will contain the names of all cpp source files
SRCS:=$(wildcard /src/*.c)

# variable with all header files
HEADERS:=$(wildcard /src/*.h)

# this will contain the names of all intermediate object files
OBJECTS:=$(patsubst /src/%.c,bin/%.o,$(SRCS))

#_DEPS = 
#DEPS = $(patsubst %,$(IDIR)/%,$(_DEPS))

#_OBJ = cdogs.o actors.o ai.o automap.o defs.o draw.o events.o files.o game.o gamedata.o grafx.o hiscores.o input.o joystick.o keyboard.o mainmenu.o map.o menu.o mission.o objs.o password.o pics.o prep.o sounds.o sprcomp.o text.o triggers.o
#OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))
#LINKOBJ = boxo.o window.o timer.o input.o level.o tile.o game.o

RM = Delete


# this rule is fancier now
# $< are the names of all prerequisites (the object files)
# $@ is the name of the target (bin/myprogram in this case)
# $(BIN):	$(OBJECTS)
#     $(CC) $^ -o $@ # must start with TAB character

# but now we have to tell make how to build the object files
# -c option tells g++ to only compile one source file at a tile
#  $< is the name of the first prerequisite (the cpp file in this case)
# $(ODIR)/%.o: src/%.c $(HEADERS)
#     $(CC) $< $(CC_OPTS) -c -o $@ # must start with TAB character


$(ODIR)/%.o:  $(SRCDIR)/%.c $(HEADERS)
	$(CC) $< -o $@ -I $(IDIR)

$(BIN): $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS) -L$(LDIR) $(LIBS)

.PHONY: all clean

#all: $(BIN)

clean:
	$(RM) $(OBJ) $(BIN)