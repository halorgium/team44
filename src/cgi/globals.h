#ifndef _CGI_GLOBALS_H
#define _CGI_GLOBALS_H 1

#define HTML_SRC_ROOT "."

#define MAXSIZE_PAGENAME 10

int bdbcount;

#include "tester.h"

char *userCode;

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
