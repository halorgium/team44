#include <stdio.h>
#include <string.h>

#include "cgic.h"
#include "globals.h"
#include "../shared/structs.h"
#include "../shared/defines.h"

void openFile(FILE *toset, const char *name) {
    toset=fopen(name, "r");
}

void echoFile(FILE *input, FILE *output) {
    char buffer[80];

    while(!feof(input)) {
	buffer[0]='\0';

	fgets(buffer, 80, input);
	if(buffer[0]!='\0') {
	    fprintf(output, buffer);
	}
    }
}

void printLink(const char *href, const char *title, FILE *output) {
    fprintf(output, "  <tr>\n    <td class=\"link\"><a class=\"buttonref\" href=\"%s&amp;user=%s\">%s</a></td>\n  </tr>\n", href, userCode, title);
}
