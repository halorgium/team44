CFLAGS=-Wall -ansi -pedantic
CC=gcc
AR=ar
OBJS=contact.o globals.o home.o login.o main.o news.o user.o ../../shared/read_line.o
LIBS=-L./ -lcgic

INSTALL_DIR=$(HOME)/public_html/testweb

all: ../bin/index.cgi

libcgic.a: cgic.o cgic.h
	rm -f libcgic.a
	$(AR) rc libcgic.a cgic.o

../bin/index.cgi: $(OBJS) libcgic.a globals.h
	gcc $(OBJS) -o ../bin/index.cgi ${LIBS}

cgic.c: cgic.h
contact.c: globals.h ../../shared/structs.h ../../shared/defines.h
globals.c: cgic.h globals.h ../../shared/structs.h ../../shared/defines.h
home.c: cgic.h globals.h ../../shared/structs.h ../../shared/defines.h
login.c: cgic.h globals.h ../../shared/structs.h ../../shared/defines.h
main.c: cgic.h globals.h ../../shared/structs.h ../../shared/defines.h
news.c: cgic.h globals.h ../../shared/structs.h ../../shared/defines.h
user.c: cgic.h globals.h ../../shared/structs.h ../../shared/defines.h

../../shared/read_line.o: 

install: 
	rm -fr $(INSTALL_DIR)
	cp -r ../html $(INSTALL_DIR)
	cp ../bin/* $(INSTALL_DIR)
	chmod -R o+rX $(INSTALL_DIR)

test:
	(cd $(INSTALL_DIR); ./index.cgi)

clean:
	rm -f *.o *.a ../bin/index.cgi

