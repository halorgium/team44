#include <stdio.h>
#include <string.h>

#include "cgic.h"
#include "globals.h"
#include "../../shared/structs.h"
#include "../../shared/defines.h"

void printHome() {
    FILE *homePage=NULL;

    homePage=fopen(HTML_SRC_ROOT"/.shared/home.src", "r");
    echoFile(homePage, cgiOut);
}
