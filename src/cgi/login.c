/*
 * login.c
 * =========
 * 
 * Deals with the login screen: displays text field where user 
 * enters their code
*/


/*===================== Preprocessor statements===========================*/

#include <stdio.h>
#include <string.h>

#include "cgic.h"
#include "globals.h"
#include "../shared/defines.h"


/*
 * Function: printLogin
 * Parameters: (none)
 * Returns: (void)
 *
 * Displays the screen for logging in. Prints the html code to the cgi
 * output stream
 */

void printLogin()
{
    fprintf(cgiOut, "<table style=\"width:100%%; text-align: center;\">\n");
    fprintf(cgiOut, "<tr>\n");
    fprintf(cgiOut, "<td>\n");
    fprintf(cgiOut, "<p>Please login to use the Music Library</p>\n");
    fprintf(cgiOut, "\n");
    fprintf(cgiOut,
            "<p>For help, view the <a href=\"./help/index.html\">user documentation</a></p>\n");
    fprintf(cgiOut, "\n");
    fprintf(cgiOut, "<form method=\"get\" action=\"./?page=home\">\n");
    fprintf(cgiOut, "<p>\n");
    fprintf(cgiOut,
            "<input type=\"hidden\" name=\"dologin\" value=\"1\" />\n");
    fprintf(cgiOut,
            "<label for=\"usercode\" title=\"User Code\"><u>U</u>ser Code: </label>\n");
    fprintf(cgiOut,
            "<input type=\"text\" id=\"usercode\" name=\"usercode\" value=\"\" size=\"%d\" maxlength=\"%d\" tabindex=\"1\" accesskey=\"u\" />\n",
            MAXLEN_USERCODE, MAXLEN_USERCODE);
    fprintf(cgiOut, "<input type=\"submit\" value=\"Login\" />\n");
    fprintf(cgiOut, "</p>\n");
    fprintf(cgiOut, "</form>\n");
    fprintf(cgiOut, "</td>\n");
    fprintf(cgiOut, "</tr>\n");
    fprintf(cgiOut, "</table>\n");
}
