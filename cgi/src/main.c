#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "cgic.h"
#include "globals.h"
#include "../../shared/structs.h"
#include "../../shared/defines.h"

void printLinks();
void printHeader();
void printFooter();

int cgiMain() {
    int result=0;
    
    char userCode[MAXSIZE_USERCODE]={'\0'};
    char pageName[MAXSIZE_PAGENAME]={'\0'};

#if DEBUG
    /* Load a saved CGI scenario if we're debugging */
    cgiReadEnvironment("/home/boutell/public_html/capcgi.dat");
#endif

    printHeader();

    /* At this point we start the dynamic content, error page may be shown */
    
    result = cgiFormStringNoNewlines("user", userCode, MAXSIZE_USERCODE);
    if(result != cgiFormSuccess) {
        /* No user, now do the login page */
	printLogin();
    }
    else {
	/* Open net conns */
	/* Check & get user */
	/* Make sure logged in */

	result = cgiFormStringNoNewlines("page", pageName, MAXSIZE_PAGENAME);
	if(result != cgiFormSuccess) {
	    strcpy(pageName, "home");
	}

	if(pageName == "logout") {
	    /* Set user to be inactive in mem on DB */
	    printLogin();
	}
	else {
	    FILE *stdlinks;
	    stdlinks=fopen(HTML_SRC_ROOT"/.shared/links_std.info", "r");
	    printLinks(stdlinks, cgiOut);

	    if(FALSE/* currUser->isLibrarian */) {
		FILE *liblinks;
		liblinks=fopen(HTML_SRC_ROOT"/.shared/links_lib.info", "r");
		printLinks(liblinks, cgiOut);
	    }

	    if(pageName == "home") {
		printHome();
	    }
	}
    }

    printFooter();
    return 0;
}

void printLinks(FILE *input, FILE *output) {
    char *href=NULL;
    char *title=NULL;

    href=malloc(100*sizeof(char));
    title=malloc(100*sizeof(char));
    
    while(!feof(input)) {
	while(fscanf(input, "%s%%%s\n", &href, &title) == 2) {
	    printLink(href, title, output);
	}
    }
}

void printHeader() {
    FILE *headerPage;

    cgiHeaderContentType("text/html");

    headerPage=fopen(HTML_SRC_ROOT"/.shared/header.src", "r");
    echoFile(headerPage, cgiOut);
}

void printFooter() {
    FILE *footerPage;
    
    footerPage=fopen(HTML_SRC_ROOT"/.shared/footer.src", "r");
    echoFile(footerPage, cgiOut);
}
