#ifndef _LIB_H_
#define _LIB_H_ 2

#include "defines.h"

int addAlbum(const char *title, const int artistID);
int addUser(const char *, const char *, const char *, const Boolean);
int addArtist(char *name);
int addCommentArtist(char *, char *, int);
int addCommentAlbum(char *, char *, int);
int addCommentUser(char *owner, char *body, char *user);
/*const char *getAlbumTitle(int idNumber);*/
/*const char *getUserName(char *usercode);*/
/*const char *getUserEmail(char *code);*/
/*onst char *getCommentBody(int commentID);*/
int *getAllAlbums(void);
/*char **getAllUsers(void);*/
/* int *getAllComments(void); */
/* int *getCommentsByAlbumID(int albumID); */
/* int *getCommentsByUserID(int userID); */
/* int *getCommentsByAlbumAndUserID(int albumID, int userID); */

int addLoan(char *user, int albumID);
int isUserInDatabase(const char *userCode);
int *getAlbumsByArtistID(int ID);
albumNode_t *getAlbum(int albumID);
artistNode_t *getArtist(int artistID);
int *getAllArtists(void);
loanNode_t *getLoan(int loanID);
int *getLoansByUserCode(char *userCode);
int *getLoansByAlbumID(int ID);
int *getAllCurrentLoans(void);
int *getAllReturnedLoans(void);
int *getLoansByUserCodeAndStatus(char * userCode, Boolean isReturned);
int isAlbumOnLoan(int albumID);
userCommentNode_t *getUserComment(int commentID);
int *getUserCommentsByOwner(char *owner);
int *getUserCommentsByUserCode(char *userCode);
artistCommentNode_t *getArtistComment(int commentID);
int *getArtistCommentsByArtistID(int ID);
int *getArtistCommentsByOwner(char *owner);
albumCommentNode_t *getAlbumComment(int commentID);
int *getAlbumCommentsByOwner(char *owner);
int *getAlbumCommentsByAlbumID(int ID);


#endif
