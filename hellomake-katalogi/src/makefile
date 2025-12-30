IDIR =../include
CC=vc
CFLAGS=-I$(IDIR) -c99

ODIR=obj
LDIR =../lib

#LIBS=-lm no libs
LIBS=

_DEPS = hellomake.h
DEPS = $(patsubst %,$(IDIR)/%,$(_DEPS))

_OBJ = hellomake.o hellofunc.o 
OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))


$(ODIR)/%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

hellomake: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS) $(LIBS)

.PHONY: clean

clean:
	delete -f $(ODIR)/*.o *~ core $(INCDIR)/*~ 