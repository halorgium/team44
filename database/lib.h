
#ifndef __MUSICLIB_H__
#define __MUSICLIB_H__

#define E_NOUSER -1
#define E_NOALBUM -2
#define E_INVALID_PARAM -3
#define E_MALLOC_FAILED -4
#define LAST_ID_IN_ARRAY -5

/*

Unless otherwise specified, if a malloc() call fails, you should
return E_MALLOC_FAILED.

*/


/*
This function adds an album into the database. You should return
E_INVALID_PARAM if either of the parameters are NULL. Otherwise you
should return the ID of the new album. IDs always have values greater
than 0.
*/
int addAlbum(const char* title, const char* artist);

/*
This function adds a user into the database. You should return
E_INVALID_PARAM if either of the parameters are NULL. Otherwise you
should return the ID of the new user. IDs always have values greater
than 0.
*/
int addUser(const char* name, const char* email);

/*
This function adds a user into the database. You should return
E_INVALID_PARAM if either of the title or body parameters are
NULL. You should return E_NOUSER if the user does not exist and
E_NOALBUM if the album does not exist. Otherwise you should return the
ID of the new comment. IDs always have values greater than 0.
*/
int addComment(int albumID, int userID, const char* title, const char* body);

/*
This function retrieves the title of an album. You should return NULL
if the album does not exist in the database.
*/
const char* getAlbumTitle(int idNumber);

/*
This function retrieves the title of an album. You should return NULL
if the album does not exist in the database.
*/
const char* getAlbumArtist(int idNumber);

/*
This function retrieves a user's name. You should return NULL if the
user does not exist in the database.
*/
const char* getUserName(int idNumber);

/*
This function retrieves a user's email address. You should return NULL
if the user does not exist in the database.
*/
const char* getUserEmail(int idNumber);

/*
This function retrieves a comment's title. You should return NULL if
the comment does not exist in the database.
*/
const char* getCommentTitle(int commentID);

/*
This function retrieves a comment's body. You should return NULL if
the commentdoes not exist in the database.
*/
const char* getCommentBody(int commentID);

/*
This function retrieves an array of album IDs. This array should be
freed by the caller function. The last id in the array should be
LAST_ID_IN_ARRAY. For example, if there were three albums in the
database, with IDs '4', '7', '19', then this function should return
this array:

 [4,7,19,LAST_ID_IN_ARRAY]

if malloc fails then return NULL.

*/
int* getAllAlbums();

/*
This function retrieves an array of user IDs. This array should be
freed by the caller function. The last id in the array should be
LAST_ID_IN_ARRAY. For example, if there were three users in the
database, with IDs '4', '7', '19', then this function should return
this array:

 [4,7,19,LAST_ID_IN_ARRAY]

if malloc fails then return NULL.

*/
int* getAllUsers();

/*
This function returns an array of comment IDs. This array should be
freed by the caller function. The last id in the array should be
LAST_ID_IN_ARRAY. For example, if there were three comments in the
database, with IDs '4', '7', '19', then this function should return
this array:

 [4,7,19,LAST_ID_IN_ARRAY]

*/
int* getAllComments();

/*
This function returns an array of comment IDs that relate to a
particular album. This array should be freed by the caller
function. See the description for getAllComments() for the structure
of the array.

If the album does not exist, then simply return an array containing
only LAST_ID_IN_ARRAY.

if malloc fails then return NULL.

*/
int* getCommentsByAlbumID(int albumID); 

/*
This function returns an array of comment IDs written by a particular
user. This array should be freed by the caller function. See the
description for getAllComments() for the structure of the array.

If the user does not exist, then simply return an array containing
only LAST_ID_IN_ARRAY.

if malloc fails then return NULL.

*/
int* getCommentsByUserID(int userID); 

/*
This function returns an array of comment IDs that relate to a
particular album and are written by a particular person. This array
should be freed by the caller function. See the description for
getAllComments() for the structure of the array.

If the user or album do not exist, then simply return an array
containing only LAST_ID_IN_ARRAY.

if malloc fails then return NULL.

*/
int* getCommentsByAlbumAndUserID(int albumID, int userID); 

#endif
