#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "lib.h"

char *toLowerCase(char *mixedCase)
{
    char *lowerCase = NULL;
    int currChar = 0;

    if (mixedCase == NULL) {
        return NULL;
    }

    lowerCase = mixedCase;

    for (currChar = 'A'; currChar <= 'Z'; currChar++) {
        char *temp = replaceCharWithChar(lowerCase, currChar, currChar + 32);
        lowerCase = temp;
    }

    return lowerCase;
}

char *replaceCharWithChar(char *unpacked, char replacee, char replacement)
{
    char *temp = NULL;
    char *packed = NULL;

    if (unpacked == NULL) {
        return NULL;
    }

    packed = unpacked;

    for (temp = unpacked; *temp != '\0'; temp++) {
        /* If current char is "replacee" */
        if (temp[0] == replacee) {
            /* Replace with char "replacement" */
            temp[0] = replacement;
        }
    }

    return packed;
}

char *replaceCharWithString(char *data, char replacee,
                            const char *replacement)
{
    char *temp = strchr(data, replacee);
    while (temp != NULL) {
        int lenToCopy = 0;
        char *tempA = NULL;
        char *tempB = NULL;
        char *newdata =
            calloc(sizeof(char), (strlen(data) + strlen(replacement)));
        if (newdata == NULL)
            return NULL;

        tempA = data;
        tempB = newdata;

        /* copy pre-string */
        lenToCopy = strlen(tempA) - strlen(temp);
        if (lenToCopy > 0) {
            strncpy(tempB, tempA, lenToCopy);
            tempB = tempB + lenToCopy;
            tempA = tempA + lenToCopy;
        }
        tempA++;

        /* copy replacement */
        strncpy(tempB, replacement, strlen(replacement));
        tempB = tempB + strlen(replacement);

        /* copy post-string */
        lenToCopy = strlen(tempA) + 1;
        if (lenToCopy > 0) {
            strncpy(tempB, tempA, lenToCopy);
        }

        /* free old data */
        free(data);
        /* set to newdata */
        data = newdata;

        temp = strchr(tempB, replacee);
    }

    return data;
}
