CC=gcc
CFLAGS = -g -pedantic  -ansi -Wall
AR=ar
CFILES=lib.c
OFILES=lib.o

all:	lib

lib:	$(OFILES)
	$(LINK.c) -o lib $(OFILES)

lib.o: lib.c lib.h
	$(CC) $(CFLAGS) $(LDFLAGS) -c lib.c -o lib.o

clean:
	rm  lib *.o
