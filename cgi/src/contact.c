#include <stdio.h>
#include <string.h>

#include "cgic.h"
#include "globals.h"
#include "../../shared/structs.h"
#include "../../shared/defines.h"

void printContact() {
  fprintf(cgiOut, "<div class=\"head1\">Contact Us!</div>\n");
  fprintf(cgiOut, "<form method=\"post\" action=\"./\">\n");
  fprintf(cgiOut, "<table>\n");
  fprintf(cgiOut, "<tbody>\n");
  fprintf(cgiOut, "  <tr>\n");
  fprintf(cgiOut, "    <td class=\"describe\">\n");
  fprintf(cgiOut, "      <label for=\"name\" title=\"Your Name\">Your Name: </label>\n");
  fprintf(cgiOut, "    </td>\n");
  fprintf(cgiOut, "    <td class=\"field\">\n");
  fprintf(cgiOut, "      <input type=\"text\" id=\"name\" name=\"name\" />\n");
  fprintf(cgiOut, "    </td>\n");
  fprintf(cgiOut, "  </tr>\n");
  fprintf(cgiOut, "  <tr>\n");
  fprintf(cgiOut, "    <td class=\"describe\">\n");
  fprintf(cgiOut, "      <label for=\"email\" title=\"Your Email Address\">Your Email Address: </label>\n");
  fprintf(cgiOut, "    </td>\n");
  fprintf(cgiOut, "    <td class=\"field\">\n");
  fprintf(cgiOut, "      <input type=\"text\" id=\"email\" name=\"email\" />\n");
  fprintf(cgiOut, "    </td>\n");
  fprintf(cgiOut, "  </tr>\n");
  fprintf(cgiOut, "  <tr>\n");
  fprintf(cgiOut, "    <td class=\"describe\" colspan=\"2\">\n");
  fprintf(cgiOut, "      <label for=\"message\" title=\"Enter Your Message Here\">Enter Your Message Here: </label>\n");
  fprintf(cgiOut, "    </td>\n");
  fprintf(cgiOut, "  </tr>\n");
  fprintf(cgiOut, "  <tr>\n");
  fprintf(cgiOut, "    <td class=\"field\" colspan=\"2\">\n");
  fprintf(cgiOut, "      <textarea id=\"message\" name=\"message\" rows=\"4\" cols=\"40\"></textarea>\n");
  fprintf(cgiOut, "    </td>\n");
  fprintf(cgiOut, "  </tr>\n");
  fprintf(cgiOut, "  <tr>\n");
  fprintf(cgiOut, "    <td colspan=\"2\">\n");
  fprintf(cgiOut, "      <input type=\"submit\" value=\"Submit Request\" />\n");
  fprintf(cgiOut, "      <input type=\"reset\" value=\"Reset Fields\" />\n");
  fprintf(cgiOut, "    </td>\n");
  fprintf(cgiOut, "  </tr>\n");
  fprintf(cgiOut, "</tbody>\n");
  fprintf(cgiOut, "</table>\n");
  fprintf(cgiOut, "</form>\n");
}
