#ifndef _CGI_GLOBALS_H
#define _CGI_GLOBALS_H 1

#include "../shared/structs.h"
#include "../shared/lib.h"
#include "../shared/defines.h"

#define HTML_SRC_ROOT "."

#define MAXSIZE_PAGENAME 10

userNode_t *_currUserLogon;

void openFile(FILE *toecho, const char *name);
void echoFile(FILE *input, FILE *output);
void printLink(const char *href, const char* title, FILE *output);

void printLogin(void);
void printHome(void);
void printNews(void);
void printContact(void);
void printUser(void);
void printArtist(void);
void printAlbum(void);
void printLoan(void);

#endif
