#ifndef _CGI_GLOBALS_H
#define _CGI_GLOBALS_H 1

#include "../shared/lib.h"
#include "../shared/defines.h"

/* This variable holds the userid of the currently logged in user */
int _currUserLogon;

/* This enum identifies the current page */
typedef enum {
    PAGE_UNKNOWN=-1, 
    PAGE_LOGIN,
    PAGE_CONTACT,
    PAGE_USER,
    PAGE_ALBUM,
    PAGE_ARTIST,
    PAGE_USERCOMMENT,
    PAGE_ALBUMCOMMENT,
    PAGE_ARTISTCOMMENT,
    PAGE_LOAN, 
    PAGE_HOME
} pageName_t;

/* This enum identifies the function of the page */
typedef enum {
    FUNC_UNKNOWN=-1,
    FUNC_ADD, /* Adding an entity */
    FUNC_BORROW, /* Borrowing an album */
    FUNC_RETURN, /* Returning an album */
    FUNC_CURRLOAN, /* Viewing current user loans */
    FUNC_PREVLOAN, /* Viewing previous user loans */
    FUNC_VIEW /* View some entities */
} funcName_t;

void userLink(char *extra, int userid, const char *content, FILE *output);
void printTime(long ctime, FILE *output);

void printLogin(void);
void printHome(void);
void printContact(void);
void printUser(funcName_t func);
void printArtist(funcName_t func);
void printAlbum(funcName_t func);
void printUserComment(funcName_t func);
void printAlbumComment(funcName_t func);
void printArtistComment(funcName_t func);
void printLoan(funcName_t func);

#endif
