#ifndef _CGI_GLOBALS_H
#define _CGI_GLOBALS_H 1

#define HTML_SRC_ROOT "."

#define MAXSIZE_PAGENAME 10

void openFile(FILE *toecho, const char *name);
void echoFile(FILE *input, FILE *output);
void printLink(const char *href, const char* title, const char *userCode, FILE *output);

void printLogin();
void printHome();
void printNews();
void printUser();

#endif
