
#include "read_line.h"

char *readLine(FILE *file){

    char *line = NULL;
    int totSize = CHUNK_SIZE+1;
    
   
    /*
     * If no storage is allocated, then allocate an initial buffer of
     * CHUNK_SIZE bytes.
     */
    line = malloc(sizeof(char)*totSize);
    if(line == NULL) return NULL;
   

    for (;;) {
	if (fgets(line + strlen(line), totSize - strlen(line), file) == 0) {
	    /*
	     * End of file or a read error has occurred.  If we've
	     * read something return it, otherwise return error.
	     */
	    if (strlen(line) > 0) {
		return line;
	    }
	    return NULL;
	}

	/*
	 * Have we encountered a newline yet?  If so remove it and return
	 * the line read.
	 */
	if (line[strlen(line) - 1] == '\n') {
	    line[strlen(line) - 1] = '\0';
	    return line;
	}

	/*
	 * Otherwise, we still have more characters to read.  Increase
	 * buffer size if need be.
	 */
	if (strlen(line) + 1 == totSize) {
	    totSize += CHUNK_SIZE;
	    if ((line = realloc(line, totSize)) == 0) {
		totSize = 0;
		return NULL;
	    }
	}
    } /* end of infinite loop to read in parts of a line */
   
}

