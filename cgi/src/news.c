#include <stdio.h>
#include <string.h>

#include "cgic.h"
#include "globals.h"
#include "../../shared/structs.h"
#include "../../shared/defines.h"

void printNews() {
    FILE *newsPage=NULL;

    newsPage=fopen(HTML_SRC_ROOT"/.shared/news.src", "r");
    echoFile(newsPage, cgiOut);
}
