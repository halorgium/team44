#include <stdio.h>
#include <string.h>

#include "cgic.h"
#include "globals.h"
#include "../shared/defines.h"

void printNews() {
  fprintf(cgiOut, "<ul>\n");
  fprintf(cgiOut, "  <li>It has been discovered that Jeremy has <b>saggy man breasts</b> </li>\n");
  fprintf(cgiOut, "</ul>\n");
}
