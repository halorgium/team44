CC=gcc
CFLAGS=-g -ansi -Wall
OFILES=lib.o load.o save.o ../../shared/read_line.o

all: $(OFILES)

lib.o:  ../../shared/defines.h ../../shared/structs.h ../../shared/lib.h globals.h save.h
load.o: ../../shared/defines.h ../../shared/structs.h ../../shared/read_line.h globals.h load.h
save.o: ../../shared/defines.h globals.h save.h

globals.h: ../../shared/structs.h
save.h: ../../shared/defines.h

clean:
	rm -f  *.o 
