CFLAGS=-Wall -ansi -pedantic
CC=gcc
AR=ar
OBJS=globals.o main.o login.o home.o user.o news.o ../../shared/read_line.o
LIBS=-L./ -lcgic

INSTALL_DIR=$(HOME)/public_html/testweb

all: libcgic.a ../bin/index.cgi

libcgic.a: cgic.o cgic.h
	rm -f libcgic.a
	$(AR) rc libcgic.a cgic.o

../bin/index.cgi: $(OBJS) libcgic.a globals.h
	gcc $(OBJS) -o ../bin/index.cgi ${LIBS}

install: 
	rm -fr $(INSTALL_DIR)
	cp -r ../html $(INSTALL_DIR)
	cp ../bin/* $(INSTALL_DIR)
	chmod -R o+rX $(INSTALL_DIR)

test:
	(cd $(INSTALL_DIR); ./index.cgi)

clean:
	rm -f *.o *.a ../bin/index.cgi

