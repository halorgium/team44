#ifndef _DB_GLOBALS
#define _DB_GLOBALS 4

#include <stdio.h>

#include "structs.h"

#define NEXTID_FILE_NAME "nextid"
#define USER_FILE_NAME "user"
#define ALBUM_FILE_NAME "album"
#define ARTIST_FILE_NAME "artist"
#define USER_COMMENT_FILE_NAME "usrcom"
#define ALBUM_COMMENT_FILE_NAME "albcom"
#define ARTIST_COMMENT_FILE_NAME "artcom"
#define LOAN_FILE_NAME "loan"
#define LOANRET_FILE_NAME "loanret"

/*all are  pointers to the first node in the 3 lists*/
userNode_t *firstUser;
albumNode_t *firstAlbum;
artistNode_t *firstArtist;
userCommentNode_t *firstUserComment;
albumCommentNode_t *firstAlbumComment;
artistCommentNode_t *firstArtistComment;
loanNode_t *firstLoan;

/*simple ID's which are incremented after addition of new users etc*/
int _nextAlbumID;
int _nextArtistID;
int _nextUserCommentID;
int _nextAlbumCommentID;
int _nextArtistCommentID;
int _nextLoanID;

#endif
