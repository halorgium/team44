CC=gcc
CFLAGS=-g  -ansi -Wall
AR=ar
HFILES=lib.h read_line.h load.h #save.h 
CFILES=lib.c read_line.c load.c #save.c 
OFILES=lib.o read_line.o load.o #save.o 

all:	lib

lib:	$(OFILES)
	$(LINK.c) -o ../bin/lib $(OFILES)

lib.o: lib.c ../../shared/structs.h ../../shared/defines.h $(HFILES)
	$(CC) $(CFLAGS) $(LDFLAGS) -c lib.c -o lib.o

read_line.o: read_line.c read_line.h
	$(CC) $(CFLAGS) $(LDFLAGS) -c read_line.c -o read_line.o

load.o: load.c load.h lib.h read_line.h
	$(CC) $(CFLAGS) $(LDFLAGS) -c load.c -o load.o

#loan.c: 

clean:
	rm  ../bin/lib *.o
