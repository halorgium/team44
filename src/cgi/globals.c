/*
 *
 * globals.c
 * ==========
 * provides functions which deal with globals in the cgi
 * 
*/

/*===================Preprocessor Statments===============================*/


#include <stdio.h>
#include <string.h>
#include <time.h>

#include "cgic.h"
#include "globals.h"
#include "../shared/defines.h"

/*
 * Function: printAlbumComment
 * Parameters: funcName stuct
 * Returns: (void)
 *
 * This method prints a link to user info if the curruser is a librarian
 * otherwise it just prints the name of the user
 */
void userLink(char *extra, int userid, const char *content, FILE * output)
{
    if (isUserLibrarian(_currUserLogon) == TRUE) {
        fprintf(output,
                "<a %shref=\"./?page=user&amp;userid=%d&amp;hash=%d\">%s</a>",
                extra, userid, _currUserLogon, content);
    }
    else {
        fprintf(output, "<b>%s</b>", content);
    }
}

void printTime(long thectime, FILE * output)
{
    if (thectime == -1) {
        fprintf(output, "unknown time");
        return;
    }
    fprintf(output, "%s", ctime(&thectime));
}
