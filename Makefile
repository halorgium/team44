include main_vars

all: cgi_all

clean:
	@(cd src; make clean)
	@(cd help; make clean)

cgi_all:
	@(cd src; make all)

help_all:
	@(cd help; make all)

install:
	@./do_install.sh

