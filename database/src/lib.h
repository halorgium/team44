#ifndef _LIB_H_
#define _LIB_H_ 2



/* lib.c */
int addAlbum(const char *title, const int artistID);
int addUser(const char *, const char *, const char *, const Boolean);
int addCommentArtist(char *, char *, int);
int addCommentAlbum(char *, char *, int);
int addCommentUser(char *owner, char *body, char *user);
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

#endif
