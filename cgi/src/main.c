#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "cgic.h"
#include "globals.h"
#include "../../shared/structs.h"
#include "../../shared/defines.h"
#include "../../shared/read_line.h"

int  printLinks();
void printHeader();
void printFooter();

int cgiMain() {
    int result=0;

    Boolean links=TRUE;
    
    char userCode[MAXSIZE_USERCODE]={'\0'};
    char pageName[MAXSIZE_PAGENAME]={'\0'};

#if DEBUG
    /* Load a saved CGI scenario if we're debugging */
    cgiReadEnvironment("/home/boutell/public_html/capcgi.dat");
#endif

    /* Start outputting the header */
    
    printHeader();
    
    /* At this point we start the dynamic content, error page may be shown */
    
    result = cgiFormStringNoNewlines("user", userCode, MAXSIZE_USERCODE);
    if(result != cgiFormSuccess) {
	strcpy(pageName, "login");
	links=FALSE;
    }
    else {
	/* Open net conns */
	/* Check & get user */
	/* Make sure logged in */
        result = cgiFormStringNoNewlines("page", pageName, MAXSIZE_PAGENAME);
	if(result != cgiFormSuccess) {
	    strcpy(pageName, "home");
	}
	
	if(strncmp(pageName, "logout", MAXSIZE_PAGENAME) == 0) {
	    /* Set user to be inactive in mem on DB */
	    strcpy(pageName, "login");
	    links=FALSE;
	}
    }

    /* If links are neccessary print them */
    if(links) {
	FILE *linksStart;
	FILE *linksEnd;

        Boolean userType=(strncmp(userCode, "admin", MAXSIZE_USERCODE) == 0) ? TRUE : FALSE;

	linksStart=fopen(HTML_SRC_ROOT"/.shared/links_s.src", "r");
	echoFile(linksStart, cgiOut);

	fprintf(cgiOut, "  <tr>\n    <td class=\"username\">You are logged on as <b>%s</b></td>\n  </tr>\n", userCode);
	fprintf(cgiOut, "  <tr>\n    <td class=\"spacer\"><img src=\"images/1px.gif\" height=\"1\" width=\"1\" alt=\"\" /></td>\n  </tr>\n");
	
	if(userType/* currUser->isLibrarian */) {
	    FILE *thelinks;
	    
	    thelinks=fopen(HTML_SRC_ROOT"/.shared/links_lib.info", "r");
	    printLinks(thelinks, userCode, cgiOut);
	}
	else {
	    FILE *thelinks;
	    
	    thelinks=fopen(HTML_SRC_ROOT"/.shared/links_std.info", "r");
	    printLinks(thelinks, userCode, cgiOut);
	}
	
	linksEnd=fopen(HTML_SRC_ROOT"/.shared/links_e.src", "r");
	echoFile(linksEnd, cgiOut);
    }

    {
	/* Start outputting the body */
	FILE *bodyStart;
	FILE *bodyEnd;

	bodyStart=fopen(HTML_SRC_ROOT"/.shared/body_s.src", "r");
	echoFile(bodyStart, cgiOut);

	if(strncmp(pageName, "home", MAXSIZE_PAGENAME) == 0) {
	    printHome();
	}
	else if(strncmp(pageName, "login", MAXSIZE_PAGENAME) == 0) {
	    printLogin();
	}
	else if(strncmp(pageName, "news", MAXSIZE_PAGENAME) == 0) {
	    printNews();
	}
	else if(strncmp(pageName, "contact", MAXSIZE_PAGENAME) == 0) {
	    printContact();
	}
	else if(strncmp(pageName, "user", MAXSIZE_PAGENAME) == 0) {
	    /* Something about a user */
	    printUser();
	}

	bodyEnd=fopen(HTML_SRC_ROOT"/.shared/body_e.src", "r");
	echoFile(bodyEnd, cgiOut);
    }
    
    /* Start outputting the footer */
    
    printFooter();
    return 0;
}

int printLinks(FILE *input, const char *userCode, FILE *output) {
    char *line = NULL;

    while((line = readLine(input)) != NULL ){
	char *href=NULL;
	char *title=NULL;

	char *temp = line;
	char *temp2 = NULL;

	temp2 = strchr(temp, '%');

	if(strlen(temp)-strlen(temp2) == 0) {
	    fprintf(output, "  <tr>\n    <td class=\"spacer\"><img src=\"images/1px.gif\" height=\"3\" width=\"1\" alt=\"\" /></td>\n  </tr>\n");
	    free(line);
	    continue;
	}
	else {

	}
	
	href = malloc(sizeof(char)*(strlen(temp)-strlen(temp2))+1);
	if(href == NULL) return E_MALLOC_FAILED;

	strncpy(href, temp, (strlen(temp)-strlen(temp2)));
	href[strlen(temp)-strlen(temp2)]='\0';
	
	temp = temp2 + 1;  /*temp string getting smaller, also skip the '%'*/

	temp2 = strchr(temp, '%');

	if(strlen(temp)-strlen(temp2) == 0) {
	    free(href);
	    free(line);
	    break;
	}

	title = malloc(sizeof(char)*(strlen(temp)-strlen(temp2))+1);
	if(title == NULL) return E_MALLOC_FAILED;
	
	strncpy(title, temp, (strlen(temp)-strlen(temp2)));
	title[strlen(temp)-strlen(temp2)]='\0';
	
	printLink(href, title, userCode, output);
	
	/*free memory before reiteration*/
	free(href);
	free(title);
	free(line);
    }
    return 1;
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
