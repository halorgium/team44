#You may wish to add -DNO_UNISTD and -DNO_SYSTEM to CFLAGS if
#your system lacks the /usr/include/unistd.h header file
#or the system() function, respectively. Non-unix systems
#will also definitely have different linker syntax.

CFLAGS=
CC=gcc
AR=ar
LIBS=-L./ -lcgic

all: libcgic.a ../bin/cgictest ../bin/capture

libcgic.a: cgic.o cgic.h
	rm -f libcgic.a
	$(AR) rc libcgic.a cgic.o

../bin/cgictest: cgictest.o libcgic.a
	gcc cgictest.o -o ../bin/cgictest ${LIBS}

../bin/capture: capture.o libcgic.a
	gcc capture.o -o ../bin/capture ${LIBS}

clean:
	rm -f *.o *.a ../bin/cgictest ../bin/capture

