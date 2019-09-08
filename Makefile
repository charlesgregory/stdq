CC=gcc
CFLAGS=-O3
LIBS=-lzmq
PROGS=stdpub stdsub

all: $(PROGS)

%.o: %.c %.h
	$(CC) -c -o $@ $< $(CFLAGS)

# $@, $^ are L and R sides of : 
stdpub:	stdpub.o jlog.o
	$(CC) -o $@ $^ $(CFLAGS) $(LIBS)

stdsub: stdsub.o jlog.o
	$(CC) -o $@ $^ $(CFLAGS) $(LIBS)

.PHONY: clean

clean:
	rm -f *.o $(PROGS)
