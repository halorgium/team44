#ifndef _DB_GLOBALS
#define _DB_GLOBALS 4

#include <stdio.h>

#include "structs.h"

#define SOURCE_LOCATION "./var/"
#define NEXTID_FILE_NAME "nextid"
#define USERS_FILE_NAME "users"
#define ALBUMS_FILE_NAME "albums"
#define ARTISTS_FILE_NAME "artists"
#define USER_COMMENTS_FILE_NAME "usrcom"
#define ALBUM_COMMENTS_FILE_NAME "albcom"
#define ARTIST_COMMENTS_FILE_NAME "artcom"
#define LOANS_FILE_NAME "loan"

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
