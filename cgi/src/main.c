#include <stdio.h>
#include <string.h>

#include "cgic.h"
#include "globals.h"
#include "../../shared/structs.h"
#include "../../shared/defines.h"

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

    result = cgiFormStringNoNewlines("page", pageName, MAXSIZE_PAGENAME);
    if(result != cgiFormSuccess) {
	strcpy(pageName, "login");
    }
    result = cgiFormStringNoNewlines("userCode", userCode, MAXSIZE_USERCODE);
    
    fprintf(cgiOut, "%s<br />", pageName);

    printFooter();
    return 0;
}

void printHeader() {
    cgiHeaderContentType("text/html");
    fprintf(cgiOut, "<!DOCTYPE html PUBLIC \"-//W3C//DTD XHTML 1.0 Strict//EN\" \"http://www.w3.org/TR/xhtml1/DTD/xhtml1-strict.dtd\">\n");
    fprintf(cgiOut, "<html>\n");
    fprintf(cgiOut, "<head>\n");
    fprintf(cgiOut, "<title>Music Library of Team44</title>\n");
    fprintf(cgiOut, "<meta http-equiv=\"Content-Style-Type\" content=\"text/css\" />\n");
    fprintf(cgiOut, "<link rel=\"stylesheet\" type=\"text/css\" href=\"/musiclib.css\" />\n");
    fprintf(cgiOut, "<link rel=\"made\" href=\"mailto:c204g18@cosc.canterbury.ac.nz\" />\n");
    fprintf(cgiOut, "</head>\n");
    fprintf(cgiOut, "<body>\n\n");
}

void printFooter() {
    fprintf(cgiOut, "\n</body>\n</html>\n");
}
