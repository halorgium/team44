#include <stdio.h>
#include <string.h>

#include "cgic.h"
#include "globals.h"
#include "../../shared/structs.h"
#include "../../shared/defines.h"

void printUser() {
    FILE *userPage=NULL;

    userPage=fopen(HTML_SRC_ROOT"/.shared/user.src", "r");
    echoFile(userPage, cgiOut);
}
