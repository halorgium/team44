#include <stdio.h>
#include <string.h>

#include "cgic.h"
#include "globals.h"
#include "../shared/defines.h"

void printHome(void) {
  fprintf(cgiOut, "<p>This is the Team44 Music Library and we strive to give you the latest music free of charge!</p>\n");
  fprintf(cgiOut, "<p>Dan has problems with crabs in his <b>vagina</b>. Please avoid sexual contact with this man as you may be itching for a while after <a href=\"./?page=contact&amp;hash=%d\">contacts</a> page you fat hoe and give us a hoolar.</p>\n", _currUserLogon);
  fprintf(cgiOut, "<p>If you came to this page looking for horse pornography please go to <a href=<a href=\"www.webmail.canterbury.ac.nz\">YourAWeirdo.com</a></p>\n");
}
