#include <stdio.h>
#include "cgic.h"
#include "../../shared/structs.h"

int cgiMain() {
#if DEBUG
    /* Load a saved CGI scenario if we're debugging */
    cgiReadEnvironment("/home/cosc/student/tsm20/public/team44/enviro.dat");
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
    
    fprintf(cgiOut, "</BODY></HTML>\n");
    return 0;
}

void replaceUserName(char *userName, FILE *inStream){
    /*SoFar :(*/
    char *ch = fgets(userName, 150, inStream);
}
		     

