CC=gcc
CFLAGS=-g  -ansi -Wall
AR=ar
#HFILES=lib.h ../../shared/read_line.h load.h save.h 
#CFILES=lib.c ../../shared/read_line.c load.c save.c 
OFILES=main.o lib.o load.o save.o net.o ../../shared/read_line.o

all:	database

database: $(OFILES)
	$(LINK.c) -lm -o ../bin/database $(OFILES)

main.o: ../../shared/defines.h lib.h load.h net.h

lib.o:  ../../shared/defines.h ../../shared/structs.h lib.h save.h

load.o: ../../shared/defines.h ../../shared/structs.h ../../shared/read_line.h lib.h load.h

save.o: ../../shared/defines.h lib.h save.h

net.o: ../../shared/defines.h lib.h net.h

../../shared/read_line.o: ../../shared/read_line.h

clean:
	rm -f  *.o ../bin/database
