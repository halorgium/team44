

/*===================== Preprocessor statements===========================*/

#include <stdio.h>
#include <string.h>

#include "cgic.h"
#include "globals.h"
#include "../shared/defines.h"


/*
 * Function: printHome
 * Parameters: (none)
 * Returns: (void)
 *
 * Prints the dynamic section of the page as the home page
 * just any new information on the database or new features can be 
 * printed here for the user to see.
 */
void printHome(void)
{
    fprintf(cgiOut,
            "<p>This is the Team44 Music Library and we strive to give you the latest music free of charge!</p>\n");
    fprintf(cgiOut,
            "<p>If you wish to contact the developers, go to the <a href=\"./?page=contact&amp;hash=%d\">contacts</a> page for information on how to do this.</p>\n",
            _currUserLogon);
}
