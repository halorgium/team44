#ifndef _LIB_H_
#define _LIB_H_ 2

#include "defines.h"

int loadDatabase(void);

/* Add methods */
int addUser(const char *, const char *, const char *, const Boolean);
int addAlbum(const char *, const int);
int addArtist(char *);
int addCommentUser(int, int, char *);
int addCommentAlbum(int, int, char *);
int addCommentArtist(int, int, char *);
int addLoan(int, int);

/* Get methods */
/* User */
userNode_t *getUser(int);
int makeUserID(const char *);
const char *getUserCode(int);
const char *getUserName(int);
const char *getUserEmail(int);
Boolean isUserLibrarian(int);

int *getAllUsers(void);
int getUsersCount(void);

/* Album */
albumNode_t *getAlbum(int);
const char *getAlbumTitle(int);
int getAlbumArtist(int);
int getAlbumCurrentLoan(int);

int *getAllAlbums(void);
int getAlbumsCount(void);

/* Artist */
artistNode_t *getArtist(int);
const char *getArtistName(int);

int *getAllArtists(void);
int getArtistsCount(void);
int *getArtistAlbums(int);
int getArtistAlbumsCount(int);

/* User Comment */
userCommentNode_t *getUserComment(int);
/* int *getAllUserComments(void);  Maybe not */
int getUserCommentUser(int);
int getUserCommentOwner(int);
const char *getUserCommentBody(int);

int *getUserCommentsByUser(int);
int getUserCommentsByUserCount(int);
int *getUserCommentsForUser(int);
int getUserCommentsForUserCount(int);

/* Album Comment */
albumCommentNode_t *getAlbumComment(int);
int getAlbumCommentAlbum(int);
int getAlbumCommentOwner(int);
const char *getAlbumCommentBody(int);

/* int *getAllAlbumComments(void);  Maybe not  */
int *getAlbumCommentsByUser(int);
int getAlbumCommentsByUserCount(int);
int *getAlbumCommentsForAlbum(int);
int getAlbumCommentsForAlbumCount(int);

/* Artist Comment */
artistCommentNode_t *getArtistComment(int);
int getArtistCommentArtist(int);
int getArtistCommentOwner(int);
const char *getArtistCommentBody(int);

/* int *getAllArtistComments(void);  Maybe not */
int *getArtistCommentsByUser(int);
int getArtistCommentsByUserCount(int);
int *getArtistCommentsForArtist(int);
int getArtistCommentsForArtistCount(int);

/* Loan */
loanNode_t *getLoan(int);
int getLoanUser(int);
int getLoanAlbum(int);
int getLoanTimeIn(int);
int getLoanTimeOut(int);
int getLoanStatus(int);

int *getLoansByUser(int);
int getLoansByUserCount(int);
int *getLoansByAlbum(int);
int getLoansByAlbumCount(int);

#endif
