#include <stdio.h>
#include <string.h>

#include "cgic.h"
#include "globals.h"
#include "../shared/defines.h"

void printHome(void) {
  fprintf(cgiOut, "<p>This is the Team44 Music Library and we strive to give you the latest music free of cost!. </p>\n");
  fprintf(cgiOut, "<p>If you would like to register with Team44 Music Library go to the <a href=\"./?page=contacts\">contacts</a> page you fat hoe and give us a hoolar.</p>\n");
}
