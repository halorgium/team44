#include <stdio.h>
#include <string.h>

#include "cgic.h"
#include "globals.h"
#include "../shared/defines.h"

void printNews() {
  fprintf(cgiOut, "<ul>\n");
  fprintf(cgiOut, "  <li>It has been discovered that Jeremy has a small penis</li>\n");
  fprintf(cgiOut, "</ul>\n");
}
