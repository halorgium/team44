#ifndef _LIB_H_
#define _LIB_H_ 2

#include "defines.h"

int loadDatabase(void);

/* This method checks whether the string is acceptable */
Boolean checkString2(char *);

/* Add methods */
int addUser(const char *, const char *, const char *, Boolean);
int addAlbum(const char *, int);
int addArtist(const char *);
int addUserComment(int, int, const char *);
int addAlbumComment(int, int, const char *);
int addArtistComment(int, int, const char *);
int addLoan(int, int);
int addLoanReturned(int);

/* Get methods */
/* User */
Boolean getUserExists(int);
int makeUserID(const char *);
char *getUserCode(int);
char *getUserName(int);
char *getUserEmail(int);
Boolean isUserLibrarian(int);

int *getUsers(void);
int getUsersCount(void);

int *getUsersByType(Boolean);
int getUsersByTypeCount(Boolean);

/* Album */
Boolean getAlbumExists(int);
char *getAlbumTitle(int);
int getAlbumArtist(int);
int getAlbumCurrentLoan(int);

int *getAlbums(void);
int getAlbumsCount(void);

/* Artist */
Boolean getArtistExists(int);
char *getArtistName(int);

int *getArtists(void);
int getArtistsCount(void);
int *getArtistAlbums(int);
int getArtistAlbumsCount(int);

/* User Comment */
Boolean getUserCommentExists(int);
int getUserCommentUser(int);
int getUserCommentOwner(int);
char *getUserCommentBody(int);

int *getUserCommentsByUser(int);
int getUserCommentsByUserCount(int);
int *getUserCommentsForUser(int);
int getUserCommentsForUserCount(int);

/* Album Comment */
Boolean getAlbumCommentExists(int);
int getAlbumCommentAlbum(int);
int getAlbumCommentOwner(int);
char *getAlbumCommentBody(int);

int *getAlbumCommentsByUser(int);
int getAlbumCommentsByUserCount(int);
int *getAlbumCommentsForAlbum(int);
int getAlbumCommentsForAlbumCount(int);

/* Artist Comment */
Boolean getArtistCommentExists(int);
int getArtistCommentArtist(int);
int getArtistCommentOwner(int);
char *getArtistCommentBody(int);

int *getArtistCommentsByUser(int);
int getArtistCommentsByUserCount(int);
int *getArtistCommentsForArtist(int);
int getArtistCommentsForArtistCount(int);

/* Loan */
Boolean getLoanExists(int);
int getLoanUser(int);
int getLoanAlbum(int);
int getLoanTimeIn(int);
int getLoanTimeOut(int);
Boolean isLoanReturned(int);

int *getLoansByUser(int, Boolean);
int getLoansByUserCount(int, Boolean);
int *getLoansByAlbum(int, Boolean);
int getLoansByAlbumCount(int, Boolean);

int setLoanReturned(int, int);

#endif
