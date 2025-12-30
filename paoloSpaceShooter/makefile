# #
# # paolo shooter
# #

# CC = gcc
# CFLAGS =-Isrc/include
# #IDIR =../include
# BIN = cdogs
# SRCDIR = src
# ODIR = obj
# #LDIR =../lib
# LIBS = -lvc -lauto -lamiga
# #LIBS = -lauto -lamiga
# #LINK = vlink -bamigahunk -Bstatic -Cvbcc -nostdlib -s -x C:/vbcc/targets/m68k-amigaos/lib/startup.o

# _DEPS = 
# DEPS = $(patsubst %,$(IDIR)/%,$(_DEPS))

# _OBJ = cdogs.o actors.o ai.o automap.o defs.o draw.o events.o files.o game.o gamedata.o grafx.o hiscores.o input.o joystick.o keyboard.o mainmenu.o map.o menu.o mission.o objs.o password.o pics.o prep.o sounds.o sprcomp.o text.o triggers.o
# OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))
# #LINKOBJ = boxo.o window.o timer.o input.o level.o tile.o game.o

# RM = Delete

# $(ODIR)/%.o:  $(SRCDIR)/%.c $(DEPS)
# 	$(CC) -c -o $@ $< $(CFLAGS)

# cdogs: $(OBJ)
# 	$(CC) -o $@ $^ $(CFLAGS) $(LIBS)

# .PHONY: all clean

# #all: $(BIN)

# clean:
# 	$(RM) $(OBJ) $(BIN)



#paolo def
EXENAME = PSpace
BUILDDIR = bin
SRCDIR = src
ODIR = obj
IDIR =SYS:SDL_AGA/SDL/include/
LDIR = SYS:SDL_AGA/SDL/bin/
LIBS = -lSDL_AGA_040 -ldebug -lamiga -lpthread
LINKOPT = -noixemul
ASMOBJ = libs/c2p8_040_asm.o libs/c2p8_040_dbl_asm.o

RM = Delete
CP = cp

RESOURCE_FILES = res/i_are_spaceship.png

# we can define variables in a makefile
# variable CC will specify the compiler; feel free to use clang++
CC:=g++

# this variable contains any extra compiler options that we might
# want to add, like -O3, -march=native, etc.
# -O3 tells g++ to optimize the code for speed
CC_OPTS:=-O3

# this variable will contain the names of all cpp source files
SRCS:=$(wildcard $(SRCDIR)/*.cpp)

# variable with all header files
HEADERS:=$(wildcard $(SRCDIR)/*.h)

# this will contain the names of all intermediate object files
OBJECTS:=$(patsubst $(SRCDIR)/%.cpp,$(ODIR)/%.o,$(SRCS))





# this rule is fancier now
# $< are the names of all prerequisites (the object files)
# $@ is the name of the target (bin/myprogram in this case)
$(BUILDDIR)/$(EXENAME): $(OBJECTS)
	$(CC) $^ $(ASMOBJ) $(CC_OPTS) -o $@ -L$(LDIR) $(LIBS) $(LINKOPT) # must start with TAB character

# but now we have to tell make how to build the object files
# -c option tells g++ to only compile one source file at a tile
#  $< is the name of the first prerequisite (the cpp file in this case)
$(ODIR)/%.o: src/%.cpp $(HEADERS)
	$(CC) $< $(CC_OPTS) -c -o $@ -I $(IDIR) # must start with TAB character

.PHONY: all clean

all: $(BIN)
	for u in $(RESOURCE_FILES); do echo $$u; $(CP) -f $$u $(BUILDDIR); done

clean:
	$(RM) $(OBJECTS)

