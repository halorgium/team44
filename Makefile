include main_vars

all: cgi

debug: cgi-debug 

clean:
	@(cd src; make clean)
	@(cd help; make clean)

cgi:
	@(cd src; make)

cgi-debug:
	@(cd src; make debug)

help:
	@(cd src; make)

install:
	@./do_install.sh

