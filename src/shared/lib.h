#ifndef _LIB_H_
#define _LIB_H_ 2

#include "defines.h"

int loadDatabase(void);

int addUser(const char *, const char *, const char *, const Boolean);
int addAlbum(const char *, const int);
int addArtist(char *);
int addCommentAlbum(char *, char *, int);
int addCommentArtist(char *, char *, int);
int addCommentUser(int, char *, int);
int addLoan(int, int);

userNode_t *getUser(int);
int *getAllUsers(void);
int makeUserID(char *);
const char *getUserCode(int);
const char *getUserName(int);
const char *getUserEmail(int);
Boolean isUserInDatabase(int);
Boolean isUserLibrarian(int);

albumNode_t *getAlbum(int);
int *getAllAlbums(void);
const char *getAlbumTitle(int);
int getAlbumArtist(int);
int *getAlbumComments(int);
int getAlbumCurrentLoan(int);

artistNode_t *getArtist(int);
int *getAllArtists(void);
const char *getArtistName(int);
int *getArtistAlbums(int);
int *getArtistComments(int);

userCommentNode_t *getUserComment(int);
int *getAllUserComments(void); /** Maybe not **/
int *getUserCommentsByUser(int);
int *getUserCommentsForUser(int);
int getUserCommentUser(int);
int getUserCommentOwner(int);
const char *getUserCommentBody(int);

albumCommentNode_t *getAlbumComment(int);
int *getAllAlbumComments(void); /** Maybe not **/
int *getAlbumCommentsByUser(int);
int *getAlbumCommentsForAlbum(int);
int getAlbumCommentAlbum(int);
int getAlbumCommentOwner(int);
const char *getAlbumCommentBody(int);

artistCommentNode_t *getArtistComment(int);
int *getAllArtistComments(void); /** Maybe not **/
int *getArtistCommentsByUser(int);
int *getArtistCommentsForArtist(int);
int getArtistCommentArtist(int);
int getArtistCommentOwner(int);
const char *getArtistCommentBody(int);

loanNode_t *getLoan(int);
int getLoanUser(int);
int getLoanAlbum(int);
int getLoanTimeIn(int);
int getLoanTimeOut(int);
int getLoanStatus(int);
int *getLoansByUser(int);
int *getLoansByAlbum(int);

#endif
