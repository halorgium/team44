CC=gcc
CFLAGS = -g -pedantic  -ansi -Wall
AR=ar
CFILES=lib.c
OFILES=lib.o

all:	lib

lib:	$(OFILES)
	$(LINK.c) -o ../bin/lib $(OFILES)

lib.o: lib.c lib.h ../../shared/structs.h
	$(CC) $(CFLAGS) $(LDFLAGS) -c lib.c -o lib.o

clean:
	rm  ../bin/lib *.o
