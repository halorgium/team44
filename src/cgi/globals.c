#include <stdio.h>
#include <string.h>

#include "cgic.h"
#include "globals.h"
#include "../shared/defines.h"

/* This method prints a link to user info if the curruser is a librarian */
/* otherwise it just prints the name of the user */
void userLink(int userid, const char *content,  FILE *output) {
  if(isUserLibrarian(_currUserLogon) == TRUE) {
    fprintf(output, "<a href=\"./?page=user&amp;userid=%d&amp;hash=%d\">%s</a>", userid, _currUserLogon, content);
  }
  else {
    fprintf(output, "%s", content);
  }
}
