#ifndef _DEFINES_H
#define _DEFINES_H 1

#define TRUE 1
#define FALSE 0
typedef int Boolean;

/* Data limits */

#define MAXSIZE_USERCODE 16
#define MAXSIZE_USERNAME 50
#define MAXSIZE_USEREMAIL 100

#define MAXSIZE_ALBUMTITLE 50

#define MAXSIZE_ARTISTNAME 50

#define MAXSIZE_USERCOMMENT 1000
#define MAXSIZE_ARTISTCOMMENT 1000
#define MAXSIZE_ALBUMCOMMENT 1000

#define E_INVALID_PARAM -1
#define E_MALLOC_FAILED -2
#define LAST_ID_IN_ARRAY -4
#define ALREADY_ADDED -5

#define DB_SAVE_FAILURE -6

#define DB_LOAD_SUCCESS 1

#define NEXTID_LOAD_FAILURE -7
#define USER_LOAD_FAILURE -8
#define ALBUM_LOAD_FAILURE -9
#define ARTIST_LOAD_FAILURE -10
#define USR_COM_LOAD_FAILURE -11
#define ALB_COM_LOAD_FAILURE -12
#define ART_COM_LOAD_FAILURE -13
#define LOAN_LOAD_FAILURE -14

#define DB_LOAD_FAILURE -15
#define DB_ALREADY_LOADED -16

#define E_NOUSER -101
#define E_NOALBUM -102
#define E_NOARTIST -103
#define E_NOUSRCOM -104
#define E_NOALBCOM -105
#define E_NOARTCOM -106
#define E_NOLOAN -107

#endif
