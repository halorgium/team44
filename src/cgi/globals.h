#ifndef _CGI_GLOBALS_H
#define _CGI_GLOBALS_H 1

#include "../shared/lib.h"
#include "../shared/defines.h"

#define MAXSIZE_PAGENAME 10

int _currUserLogon;

void userLink(int userid, const char *content, FILE *output);
void printTime(long ctime, FILE *output);

void printLogin(void);
void printHome(void);
void printNews(void);
void printContact(void);
void printUser(void);
void printArtist(void);
void printAlbum(void);
void printLoan(void);

#endif
