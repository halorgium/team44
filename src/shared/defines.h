#ifndef _DEFINES_H
#define _DEFINES_H 1

/* File locations */

#define DATA_LOCATION "./var"

#define TRUE 1
#define FALSE 0
typedef int Boolean;

/* Data limits */

#define MAXLEN_USERCODE 16
#define MAXLEN_USERNAME 50
#define MAXLEN_USEREMAIL 100

#define MAXLEN_ALBUMTITLE 50

#define MAXLEN_ARTISTNAME 50

#define MAXSIZE_USERCOMMENT 200
#define MAXSIZE_ARTISTCOMMENT 200
#define MAXSIZE_ALBUMCOMMENT 200

/* Error codes */

#define E_NOERROR 1

#define E_INVALID_PARAM -1
#define E_MALLOC_FAILED -2
#define LAST_ID_IN_ARRAY -4
#define ALREADY_ADDED -5

#define DB_SAVE_FAILURE -6
#define DB_BAD_TIME -7

#define DB_LOAD_SUCCESS E_NOERROR

#define NEXTID_LOAD_FAILURE -10
#define USER_LOAD_FAILURE -11
#define ALBUM_LOAD_FAILURE -12
#define ARTIST_LOAD_FAILURE -13
#define USR_COM_LOAD_FAILURE -14
#define ALB_COM_LOAD_FAILURE -15
#define ART_COM_LOAD_FAILURE -16
#define LOAN_LOAD_FAILURE -17
#define LOANRET_LOAD_FAILURE -18

#define DB_LOAD_FAILURE -20
#define DB_ALREADY_LOADED -21
#define DB_NEXTID_ERROR -22

#define E_FORM -40
#define E_TOOBIG -45
#define E_UNKNOWN -50

#define E_NOUSER -101
#define E_NOALBUM -102
#define E_NOARTIST -103
#define E_NOUSRCOM -104
#define E_NOALBCOM -105
#define E_NOARTCOM -106
#define E_NOLOAN -107

#endif
