#ifndef _DEFINES_H
#define _DEFINES_H 1

#define TRUE 1
#define FALSE 0
typedef int Boolean;

/* Data limits */

#define MAXSIZE_USERCODE 16
#define MAXSIZE_USERNAME 50
#define MAXSIZE_USEREMAIL 100

#define MAXSIZE_ALBUMNAME 50

#define MAXSIZE_ARTISTNAME 50

#define MAXSIZE_USERCOMMENT 1000
#define MAXSIZE_ARTISTCOMMENT 1000
#define MAXSIZE_ALBUMCOMMENT 1000

#define E_INVALID_PARAM -1
#define E_MALLOC_FAILED -2
#define E_NOALBUM -3
#define E_NOUSER -4
#define LAST_ID_IN_ARRAY -5
#define ALREADY_ADDED -6
#define SAVE_FAILURE -7
#define USER_LOAD_FAILURE -8
#define ALBUM_LOAD_FAILURE -9
#define ARTIST_LOAD_FAILURE -10
#define LOAN_LOAD_FAILURE -11
#define ART_COM_LOAD_FAILURE -12
#define ALB_COM_LOAD_FAILURE -13
#define USR_COM_LOAD_FAILURE -14


#endif
