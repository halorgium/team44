CC=gcc
AR=ar
CFLAGS=-g -ansi -Wall -pedantic
LIBS=-Lcgi/ -lcgic -lm

CGI_OBJS=cgi/cgic.o cgi/globals.o cgi/main.o cgi/login.o cgi/home.o cgi/contact.o cgi/user.o cgi/album.o cgi/artist.o cgi/usercomment.o cgi/albumcomment.o cgi/artistcomment.o cgi/loan.o
DATABASE_OBJS=database/get.o database/load.o database/save.o database/add.o database/read_line.o

CGI_H=cgi/cgic.h cgi/globals.h shared/defines.h

OBJS=$(CGI_OBJS) $(DATABASE_OBJS)

all: index.cgi

index.cgi: $(OBJS) cgi/libcgic.a
	gcc $(OBJS) -o index.cgi ${LIBS}

clean: 
	rm -f index.cgi cgi/libcgic.a $(OBJS)

# Shared
shared/lib.h: shared/defines.h

# CGI
cgi/libcgic.a: cgi/cgic.o cgi/cgic.h
	rm -f cgi/libcgic.a
	(cd cgi; $(AR) rc libcgic.a cgic.o)

cgi/cgic.o: cgi/cgic.h
cgi/globals.o: $(CGI_H)
cgi/main.o: $(CGI_H)
cgi/login.o: $(CGI_H)
cgi/home.o: $(CGI_H)
cgi/contact.o: $(CGI_H)
cgi/user.o: $(CGI_H)
cgi/album.o: $(CGI_H)
cgi/artist.o: $(CGI_H)
cgi/usercomment.o: $(CGI_H)
cgi/albumcomment.o: $(CGI_H)
cgi/artistcomment.o: $(CGI_H)
cgi/loan.o: $(CGI_H)

cgi/globals.h: shared/defines.h shared/lib.h

# Database
database/get.o:  shared/defines.h database/structs.h shared/lib.h database/globals.h
database/load.o: shared/defines.h database/structs.h database/read_line.h database/globals.h 
database/save.o: shared/defines.h database/globals.h database/save.h
database/add.o: shared/defines.h database/structs.h database/globals.h database/save.h
database/read_line.o: database/read_line.h

database/globals.h: database/structs.h
database/save.h: shared/defines.h
database/structs.h: shared/defines.h
