#include <stdio.h>
#include "cgic.h"
#include "../../structs.h"

int cgiMain() {
#if DEBUG
    /* Load a saved CGI scenario if we're debugging */
    cgiReadEnvironment("/home/boutell/public_html/capcgi.dat");
#endif
    cgiHeaderContentType("text/html");
    fprintf(cgiOut, "<HTML><HEAD>\n");
    fprintf(cgiOut, "<TITLE>cgic test</TITLE></HEAD>\n");
    fprintf(cgiOut, "<BODY><H1>cgic test</H1>\n");
    Name();
    Address();
    Hungry();
    Temperature();
    Frogs();
    Color();
    Flavors();
    NonExButtons();
    RadioButtons();
    fprintf(cgiOut, "</BODY></HTML>\n");
    return 0;
}

