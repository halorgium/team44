#include <stdio.h>
#include <string.h>

#include "cgic.h"
#include "globals.h"
#include "../shared/structs.h"
#include "../shared/defines.h"

void printLogin() {
    FILE *loginPage=NULL;

    loginPage=fopen(HTML_SRC_ROOT"/.shared/login.src", "r");
    echoFile(loginPage, cgiOut);
}
