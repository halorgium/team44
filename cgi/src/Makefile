CFLAGS=-Wall -ansi -pedantic
CC=gcc
AR=ar
LIBS=-L./ -lcgic

all: libcgic.a ../bin/cgictest.cgi ../bin/capture.cgi

libcgic.a: cgic.o cgic.h
	rm -fr ../bin
	mkdir ../bin
	rm -f libcgic.a
	$(AR) rc libcgic.a cgic.o

../bin/cgictest.cgi: cgictest.o libcgic.a
	gcc cgictest.o -o ../bin/cgictest.cgi ${LIBS}

../bin/capture.cgi: capture.o libcgic.a
	gcc capture.o -o ../bin/capture.cgi ${LIBS}

install: 
	rm -fr ../../testweb
	cp -r ../html ../../testweb
	cp ../bin/* ../../testweb
	chmod g+rX ../../testweb

clean:
	rm -f *.o *.a ../bin/cgictest ../bin/capture

