/*
 * contact.c
 * ==========
 * 
 * contains one function which prints information out and
 * creates the contact page.  
*/

/*===================== Preprocessor statements===========================*/

#include <stdio.h>
#include <string.h>

#include "cgic.h"
#include "globals.h"
#include "../shared/defines.h"

/*
 * Function: printContact
 * Parameters: (void)
 * Returns: (void)
 *
 * Prints out each of the programmers names and information who designed
 * this site and their contact details.
 */
void printContact(void)
{
    fprintf(cgiOut, "<div class=\"head1\">Contact Us!</div>\n");
    fprintf(cgiOut,
            "If you have problems with our site or general inquiries please feel free to contact us and we will be happy to hear from you!<br />\n");
    fprintf(cgiOut,
            "To report a bug please email the chief programmer listed below.\n");
    fprintf(cgiOut, "<hr />");

    fprintf(cgiOut, "<table>\n");
    fprintf(cgiOut, "<tbody>\n");
    fprintf(cgiOut, "  <tr>\n");
    fprintf(cgiOut, "    <td class=\"describe\" colspan=\"2\">\n");
    fprintf(cgiOut,
            "      <label title=\"Name\">General Inquiries</label>\n");
    fprintf(cgiOut, "    </td>\n");
    fprintf(cgiOut, "  </tr>\n");
    fprintf(cgiOut, "  <tr>\n");
    fprintf(cgiOut, "    <td class=\"describe\">\n");
    fprintf(cgiOut, "      <label title=\"email address\">Email: </label>\n");
    fprintf(cgiOut, "    </td>\n");
    fprintf(cgiOut, "    <td class=\"email\">\n");
    fprintf(cgiOut,
            "      <a href=\"mailto:c204g18@cosc.canterbury.ac.nz\">c204g18@cosc.canterbury.ac.nz</a>\n");
    fprintf(cgiOut, "    </td>\n");
    fprintf(cgiOut, "  </tr>\n");
    fprintf(cgiOut, "</tbody>\n");
    fprintf(cgiOut, "</table>\n");
    fprintf(cgiOut, "<hr />\n");

    fprintf(cgiOut, "<table>\n");
    fprintf(cgiOut, "<tbody>\n");
    fprintf(cgiOut, "  <tr>\n");
    fprintf(cgiOut, "    <td class=\"describe\" colspan=\"2\">\n");
    fprintf(cgiOut, "      <label title=\"Name\">Tim Carey-Smith</label>\n");
    fprintf(cgiOut, "    </td>\n");
    fprintf(cgiOut, "  </tr>\n");
    fprintf(cgiOut, "  <tr>\n");
    fprintf(cgiOut, "    <td class=\"describe\" colspan=\"2\">\n");
    fprintf(cgiOut,
            "      <label title=\"Position\">Chief Programmer</label>\n");
    fprintf(cgiOut, "    </td>\n");
    fprintf(cgiOut, "  </tr>\n");
    fprintf(cgiOut, "  <tr>\n");
    fprintf(cgiOut, "    <td class=\"describe\">\n");
    fprintf(cgiOut, "      <label title=\"email address\">Email: </label>\n");
    fprintf(cgiOut, "    </td>\n");
    fprintf(cgiOut, "    <td class=\"email\">\n");
    fprintf(cgiOut,
            "      <a href=\"mailto:tsm20@student.canterbury.ac.nz\">tsm20@student.canterbury.ac.nz</a>\n");
    fprintf(cgiOut, "    </td>\n");
    fprintf(cgiOut, "  </tr>\n");
    fprintf(cgiOut, "</tbody>\n");
    fprintf(cgiOut, "</table>\n");
    fprintf(cgiOut, "<hr />\n");

    fprintf(cgiOut, "<table>\n");
    fprintf(cgiOut, "<tbody>\n");
    fprintf(cgiOut, "  <tr>\n");
    fprintf(cgiOut, "    <td class=\"describe\" colspan=\"2\">\n");
    fprintf(cgiOut, "      <label title=\"Name\">Jeremy King</label>\n");
    fprintf(cgiOut, "    </td>\n");
    fprintf(cgiOut, "  </tr>\n");
    fprintf(cgiOut, "  <tr>\n");
    fprintf(cgiOut, "    <td class=\"describe\" colspan=\"2\">\n");
    fprintf(cgiOut, "      <label title=\"Position\">CGI Guru</label>\n");
    fprintf(cgiOut, "    </td>\n");
    fprintf(cgiOut, "  </tr>\n");
    fprintf(cgiOut, "  <tr>\n");
    fprintf(cgiOut, "    <td class=\"describe\">\n");
    fprintf(cgiOut, "      <label title=\"email address\">Email: </label>\n");
    fprintf(cgiOut, "    </td>\n");
    fprintf(cgiOut, "    <td class=\"email\">\n");
    fprintf(cgiOut,
            "      <a href=\"mailto:jbk20@student.canterbury.ac.nz\">jbk20@student.canterbury.ac.nz</a>\n");
    fprintf(cgiOut, "    </td>\n");
    fprintf(cgiOut, "  </tr>\n");
    fprintf(cgiOut, "</tbody>\n");
    fprintf(cgiOut, "</table>\n");
    fprintf(cgiOut, "<hr />\n");

    fprintf(cgiOut, "<table>\n");
    fprintf(cgiOut, "<tbody>\n");
    fprintf(cgiOut, "  <tr>\n");
    fprintf(cgiOut, "    <td class=\"describe\" colspan=\"2\">\n");
    fprintf(cgiOut, "      <label title=\"Name\">Eden Sinclair</label>\n");
    fprintf(cgiOut, "    </td>\n");
    fprintf(cgiOut, "  </tr>\n");
    fprintf(cgiOut, "  <tr>\n");
    fprintf(cgiOut, "    <td class=\"describe\" colspan=\"2\">\n");
    fprintf(cgiOut,
            "      <label title=\"Position\">Database Extraordinaire</label>\n");
    fprintf(cgiOut, "    </td>\n");
    fprintf(cgiOut, "  </tr>\n");
    fprintf(cgiOut, "  <tr>\n");
    fprintf(cgiOut, "    <td class=\"describe\">\n");
    fprintf(cgiOut, "      <label title=\"email address\">Email: </label>\n");
    fprintf(cgiOut, "    </td>\n");
    fprintf(cgiOut, "    <td class=\"email\">\n");
    fprintf(cgiOut,
            "      <a href=\"mailto:ejs73@student.canterbury.ac.nz\">ejs73@student.canterbury.ac.nz</a>\n");
    fprintf(cgiOut, "    </td>\n");
    fprintf(cgiOut, "  </tr>\n");
    fprintf(cgiOut, "</tbody>\n");
    fprintf(cgiOut, "</table>\n");
    fprintf(cgiOut, "<hr />\n");

    fprintf(cgiOut, "<table>\n");
    fprintf(cgiOut, "<tbody>\n");
    fprintf(cgiOut, "  <tr>\n");
    fprintf(cgiOut, "    <td class=\"describe\" colspan=\"2\">\n");
    fprintf(cgiOut, "      <label title=\"Name\">Daniel Bakker</label>\n");
    fprintf(cgiOut, "    </td>\n");
    fprintf(cgiOut, "  </tr>\n");
    fprintf(cgiOut, "  <tr>\n");
    fprintf(cgiOut, "    <td class=\"describe\" colspan=\"2\">\n");
    fprintf(cgiOut, "      <label title=\"Position\">Documenter</label>\n");
    fprintf(cgiOut, "    </td>\n");
    fprintf(cgiOut, "  </tr>\n");
    fprintf(cgiOut, "  <tr>\n");
    fprintf(cgiOut, "    <td class=\"describe\">\n");
    fprintf(cgiOut, "      <label title=\"email address\">Email: </label>\n");
    fprintf(cgiOut, "    </td>\n");
    fprintf(cgiOut, "    <td class=\"email\">\n");
    fprintf(cgiOut,
            "      <a href=\"mailto:djb142@student.canterbury.ac.nz\">djb142@student.canterbury.ac.nz</a>\n");
    fprintf(cgiOut, "    </td>\n");
    fprintf(cgiOut, "  </tr>\n");
    fprintf(cgiOut, "</tbody>\n");
    fprintf(cgiOut, "</table>\n");
}
