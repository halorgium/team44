#include <stdio.h>
#include <string.h>

#include "cgic.h"
#include "globals.h"
#include "../../shared/structs.h"
#include "../../shared/defines.h"

void printContact() {
    FILE *contactPage=NULL;

    contactPage=fopen(HTML_SRC_ROOT"/.shared/contact.src", "r");
    echoFile(contactPage, cgiOut);
}
