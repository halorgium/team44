CC=gcc
CFLAGS = -g  -ansi -Wall
AR=ar
CFILES=lib.c read_line.c #loan.c save.c 
OFILES=lib.o read_line.o #loan.o save.o 

all:	lib

lib:	$(OFILES)
	$(LINK.c) -o ../bin/lib $(OFILES)

lib.o: lib.c lib.h ../../shared/structs.h ../../shared/defines.h read_line.h
	$(CC) $(CFLAGS) $(LDFLAGS) -c lib.c -o lib.o

read_line.o: read_line.c read_line.h
	$(CC) $(CFLAGS) $(LDFLAGS) -c read_line.c -o read_line.o

#loan.c: 

clean:
	rm  ../bin/lib *.o
