CC=gcc
CFLAGS=-g  -ansi -Wall
AR=ar
OFILES=main.o lib.o load.o save.o net.o ../../shared/read_line.o

all:	database

database: $(OFILES)
	$(LINK.c) -lm -o ../bin/database $(OFILES)

main.o: ../../shared/defines.h ../../shared/lib.h load.h net.h

lib.o:  ../../shared/defines.h ../../shared/structs.h ../../shared/lib.h save.h

load.o: ../../shared/defines.h ../../shared/structs.h ../../shared/read_line.h ../../shared/lib.h load.h

save.o: ../../shared/defines.h ../../shared/lib.h save.h

net.o: ../../shared/defines.h ../../shared/lib.h net.h

../../shared/read_line.o: ../../shared/read_line.h

clean:
	rm -f  *.o ../bin/database
