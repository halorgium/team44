CC=gcc
CFLAGS=-g -ansi -Wall -pedantic
LIBS=-Lcgi/ -lcgic -lm

##############################################
# Object files

SHARED_OBJS=shared/read_line.o
CGI_OBJS=cgi/globals.o cgi/main.o cgi/login.o cgi/home.o cgi/contact.o cgi/user.o cgi/artist.o cgi/album.o cgi/loan.o cgi/news.o cgi/tester.o
DATABASE_OBJS=database/lib.o database/load.o database/save.o

OBJS=$(CGI_OBJS) $(DATABASE_OBJS)

INSTALL_DIR=$(HOME)/public_html/testweb

all: ../bin/index.cgi

##############################################
# Shared

read_line.o: ../shared/read_line.h

lib.h: defines.h
structs.h: defines.h

##############################################
# CGI

cgi/libcgic.a: cgi/cgic.o cgi/cgic.h
	rm -f cgi/libcgic.a
	(cd cgi; $(AR) rc libcgic.a cgic.o)

## Testing ##
cgi/globals.h: cgi/tester.h
cgi/tester.c: cgi/tester.h
## Testing ##
 
cgi/cgic.c: cgi/cgic.h
cgi/contact.c: cgi/globals.h shared/structs.h shared/defines.h
cgi/globals.c: cgi/cgic.h cgi/globals.h shared/structs.h shared/defines.h
cgi/home.c: cgi/cgic.h cgi/globals.h shared/structs.h shared/defines.h
cgi/login.c: cgi/cgic.h cgi/globals.h shared/structs.h shared/defines.h
cgi/main.c: cgi/cgic.h cgi/globals.h shared/structs.h shared/defines.h
cgi/news.c: cgi/cgic.h cgi/globals.h shared/structs.h shared/defines.h
cgi/user.c: cgi/cgic.h cgi/globals.h shared/structs.h shared/defines.h
cgi/artist.c: cgi/cgic.h cgi/globals.h shared/structs.h shared/defines.h
cgi/album.c: cgi/cgic.h cgi/globals.h shared/structs.h shared/defines.h
cgi/loan.c: cgi/cgic.h cgi/globals.h shared/structs.h shared/defines.h

##############################################
# Database

database/lib.c:  shared/defines.h shared/structs.h shared/lib.h database/globals.h database/save.h
database/load.c: shared/defines.h shared/structs.h shared/read_line.h database/globals.h database/load.h
database/save.c: shared/defines.h database/globals.h database/save.h

database/globals.h: shared/structs.h
database/save.h: shared/defines.h

##############################################
# Clean up

clean: 
	rm -f shared/*.o cgi/*.o cgi/*.a database/*.o ../bin/index.cgi

##############################################
# Main executable

../bin/index.cgi: $(OBJS) cgi/libcgic.a
	gcc $(OBJS) -o ../bin/index.cgi ${LIBS}

##############################################
# Installation

install: 
	rm -fr $(INSTALL_DIR)
	cp -r ../html $(INSTALL_DIR)
	cp ../bin/* $(INSTALL_DIR)
	chmod -R o+rX $(INSTALL_DIR)

