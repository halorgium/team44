CGI_FILES=src/index.cgi src/musiclib.css src/1px.gif src/.htaccess src/logo.png
DB_FILES=var/nextid var/user var/album var/artist var/usrcom var/albcom var/artcom var/loan var/loanret var/.htaccess

INSTALL_DIR=$(HOME)/public_html/testweb
DB_INSTALL_DIR=$(INSTALL_DIR)/var
HELP_INSTALL_DIR=$(INSTALL_DIR)/help

all: 
	@(cd src; make)

clean: 
	@(cd src; make clean)

install: install-cgi install-db install-help
#	rm -fr $(INSTALL_DIR)
#	mkdir -p $(INSTALL_DIR)

install-cgi: 
	cp $(CGI_FILES) $(INSTALL_DIR)
	chmod u+rw $(INSTALL_DIR)/*
	chmod o+rX $(INSTALL_DIR)/*

install-db: install-cgi
#	mkdir $(DB_INSTALL_DIR)
	cp $(DB_FILES) $(DB_INSTALL_DIR)
	chmod u+rw $(DB_INSTALL_DIR)/*
	chmod og-rwx $(DB_INSTALL_DIR)/*

install-help: 
#	mkdir $(HELP_INSTALL_DIR)/help
