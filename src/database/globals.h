#ifndef _DB_GLOBALS
#define _DB_GLOBALS 4

#include <stdio.h>
#include "../shared/structs.h"

#define SOURCE_LOCATION "./var/"
#define ALBUMS_FILE_NAME "albums"
#define ARTISTS_FILE_NAME "artists"
#define USERS_FILE_NAME "users"
#define USER_COMMENTS_FILE_NAME "comu"
#define ALBUM_COMMENTS_FILE_NAME "comal"
#define ARTIST_COMMENTS_FILE_NAME "comar"
#define LOANS_FILE_NAME "loan"

/*all are  pointers to the first node in the 3 lists*/
static userNode_t *firstUser = NULL;
static albumNode_t *firstAlbum = NULL;
static artistNode_t *firstArtist = NULL;
static userCommentNode_t *firstUserComment = NULL;
static artistCommentNode_t *firstArtistComment = NULL;
static albumCommentNode_t *firstAlbumComment = NULL;
static loanNode_t *firstLoan = NULL;

/*simple ID's which are incremented after addition of new users etc*/
/* these should be changed before final implementation*/
static int nextArtistID = 100;
static int nextAlbumID = 200;
static int nextCommentID = 300;
static int nextLoanID = 400;

#endif
