CC=gcc
CFLAGS=-g  -ansi -Wall
AR=ar
OFILES=lib.o load.o save.o ../../shared/read_line.o

all: ../bin/database

../bin/database: $(OFILES)
	gcc -o ../bin/database $? -lm

lib.o:  ../../shared/defines.h ../../shared/structs.h ../../shared/lib.h globals.h save.h

load.o: ../../shared/defines.h ../../shared/structs.h ../../shared/read_line.h globals.h load.h

save.o: ../../shared/defines.h globals.h save.h

../../shared/read_line.o: ../../shared/read_line.h

globals.h: ../../shared/structs.h
save.h: ../../shared/defines.h

clean:
	rm -f  *.o ../bin/database ../../shared/read_line.o
