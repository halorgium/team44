CC=gcc
AR=ar
CFLAGS=-g -ansi -Wall -pedantic
LIBS=-Lcgi/ -lcgic -lm

##############################################
# Object files

CGI_OBJS=cgi/cgic.o cgi/globals.o cgi/main.o cgi/login.o cgi/home.o cgi/contact.o cgi/user.o cgi/artist.o cgi/album.o cgi/loan.o cgi/news.o
DATABASE_OBJS=database/get.o database/load.o database/save.o database/add.o database/read_line.o

OBJS=$(CGI_OBJS) $(DATABASE_OBJS)

CGI_FILES=index.cgi musiclib.css 1px.gif .htaccess logo.png
DB_FILES=../var/nextid ../var/user ../var/album ../var/artist ../var/usrcom ../var/albcom ../var/artcom ../var/loan ../var/loanret ../var/.htaccess
DEPENDS=$(CGI_FILES) $(DB_FILES)

INSTALL_DIR=$(HOME)/public_html/testweb

all: index.cgi

##############################################
# Shared

shared/lib.h: shared/defines.h

##############################################
# CGI

cgi/libcgic.a: cgi/cgic.o cgi/cgic.h
	rm -f cgi/libcgic.a
	(cd cgi; $(AR) rc libcgic.a cgic.o)

cgi/cgic.o: cgi/cgic.h
cgi/contact.o: cgi/globals.h shared/defines.h
cgi/globals.o: cgi/cgic.h cgi/globals.h shared/defines.h
cgi/home.o: cgi/cgic.h cgi/globals.h shared/defines.h
cgi/login.o: cgi/cgic.h cgi/globals.h shared/defines.h
cgi/main.o: cgi/cgic.h cgi/globals.h shared/defines.h
cgi/news.o: cgi/cgic.h cgi/globals.h shared/defines.h
cgi/user.o: cgi/cgic.h cgi/globals.h shared/defines.h
cgi/artist.o: cgi/cgic.h cgi/globals.h shared/defines.h
cgi/album.o: cgi/cgic.h cgi/globals.h shared/defines.h
cgi/loan.o: cgi/cgic.h cgi/globals.h shared/defines.h

##############################################
# Database

database/get.o:  shared/defines.h database/structs.h shared/lib.h database/globals.h
database/load.o: shared/defines.h database/structs.h database/read_line.h database/globals.h 
database/save.o: shared/defines.h database/globals.h database/save.h
database/add.o: shared/defines.h database/structs.h database/globals.h database/save.h
database/read_line.o: database/read_line.h

database/globals.h: database/structs.h
database/save.h: shared/defines.h
database/structs.h: shared/defines.h

##############################################
# Clean up

clean: 
	rm -f index.cgi cgi/libcgic.a $(OBJS)

##############################################
# Main executable

index.cgi: $(OBJS) cgi/libcgic.a
	gcc $(OBJS) -o index.cgi ${LIBS}

##############################################
# Installation

install: install-cgi
#	rm -fr $(INSTALL_DIR)

install-cgi: $(CGI_FILES)
#	mkdir $(INSTALL_DIR)
	cp $(CGI_FILES) $(INSTALL_DIR)
	chmod -R o+rX $(INSTALL_DIR)

install-db: $(DB_FILES)
	mkdir $(INSTALL_DIR)/var
	cp $(DB_FILES) $(INSTALL_DIR)/var
	chmod u+w $(INSTALL_DIR)/var/*
