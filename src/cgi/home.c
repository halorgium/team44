#include <stdio.h>
#include <string.h>

#include "cgic.h"
#include "globals.h"
#include "../shared/defines.h"

void printHome(void) {
    fprintf(cgiOut, "<p>This is the Team44 Music Library and we strive to give you the latest music free of charge!</p>\n");
    fprintf(cgiOut, "<p>If you wish to contact the developers, go to the <a href=\"./?page=contact&amp;hash=%d\">contacts</a> page for information on how to do this.</p>\n", _currUserLogon);
}
