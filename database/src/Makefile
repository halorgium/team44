CC=gcc
CFLAGS=-g  -ansi -Wall
AR=ar
HFILES=lib.h ../../shared/read_line.h load.h save.h 
CFILES=lib.c ../../shared/read_line.c load.c save.c 
OFILES=lib.o read_line.o load.o save.o 

all:	lib

lib:	$(OFILES)
	$(LINK.c) -lm -o ../bin/lib $(OFILES)

lib.o: lib.c ../../shared/structs.h ../../shared/defines.h $(HFILES)
	$(CC) $(CFLAGS) $(LDFLAGS) -c lib.c -o lib.o

read_line.o: ../../shared/read_line.c ../../shared/read_line.h
	$(CC) $(CFLAGS) $(LDFLAGS) -c ../../shared/read_line.c -o read_line.o

load.o: load.c load.h lib.h ../../shared/read_line.h
	$(CC) $(CFLAGS) $(LDFLAGS) -c load.c -o load.o

save.o: save.c lib.h save.h ../../shared/defines.h
	$(CC) $(CFLAGS) -c save.c -o save.o

clean:
	rm  ../bin/lib *.o
