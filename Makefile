include main_vars

all: cgi

clean:
	@(cd src; make clean)
	@(cd help; make clean)

cgi:
	@(cd src; make)

help:
	@(cd src; make)

install:
	@./do_install.sh

