#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define SOURCE_LOCATION "../var/"
#define ALBUMS_FILE_NAME "albums"
#define ARTISTS_FILE_NAME "artists"
#define USERS_FILE_NAME "users"
#define USER_COMMENTS_FILE_NAME "comu"
#define ALBUM_COMMENTS_FILE_NAME "comal"
#define ARTIST_COMMENTS_FILE_NAME "comar"
#define LOANS_FILE_NAME "loan"


/* lib.c */
int addAlbum(const char *title, const int artistID);
int addUser(const char *, const char *, const char *, const Boolean);
int addCommentArtist(char *, const char *, int);
int addCommentAlbum(char *, const char *, int);
int addCommentUser(char *owner, const char *body, char *user);
const char *getAlbumTitle(int idNumber);
const char *getUserName(char *usercode);
const char *getUserEmail(char *code);
const char *getCommentBody(int commentID);
int *getAllAlbums(void);
/*char **getAllUsers(void);*/
/* int *getAllComments(void); */
/* int *getCommentsByAlbumID(int albumID); */
/* int *getCommentsByUserID(int userID); */
/* int *getCommentsByAlbumAndUserID(int albumID, int userID); */
