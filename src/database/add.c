#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/time.h>

#include "../shared/defines.h"
#include "../shared/lib.h"
#include "structs.h"
#include "globals.h"
#include "save.h"

/* returns the current time of day (used for album borrowing and returning */
static long getctime(void)
{
    long toReturn = -1;
    int suc = 0;
    struct timeval *tv = malloc(sizeof(struct timeval));
    if (tv == NULL) {
        return -1;
    }
    suc = gettimeofday(tv, NULL);

    if (suc != 0) {
        /* Some problem with retrieving the time */
        free(tv);
        return -1;
    }

    /* Return the time of day */
    toReturn = tv->tv_sec;
    free(tv);
    return toReturn;
}

/* Checks that string is acceptable for entering into the database */
static Boolean checkString(const char *string)
{
    if (string == NULL) {
        return FALSE;
    }
    if (string[0] == '\0' || string[0] == ' ') {
        return FALSE;
    }
    if (strchr(string, '%') != NULL) {
        return FALSE;
    }

    return TRUE;
}

Boolean checkString2(char *string)
{
    return checkString((const char *) string);
}

/* Function: addUser
 * Params:  const char* name, char* email
 * Returns: int.
 *
 * This function adds a user into the database. It returns
 * E_INVALID_PARAM if either of the parameters are NULL, and 
 * E_MALLOC_FAILED if a memory allocation failed. Otherwise
 * it returns the ID of the new user. userIDs always have values greater
 * than 0.
 * 
 * Blank user names and emails are allowed to be added 
 */
int addUser(const char *userCode, const char *name, const char *email,
            Boolean isLib)
{
    userNode_t *newUserNode = NULL;

    /* check for char * params */
    if (checkString(userCode) != TRUE || checkString(name) != TRUE
        || checkString(email) != TRUE) {
        return E_INVALID_PARAM;
    }

    /* get userCode */
    if (getUserExists(makeUserID(userCode)) == TRUE) {
        return ALREADY_ADDED;
    }

    /*allocates memory for the new user */
    newUserNode = (userNode_t *) malloc(sizeof(userNode_t));
    if (newUserNode == NULL) {  /*malloc failure? */
        return E_MALLOC_FAILED;
    }

    /*allocates memory for the newuser's userCode */
    newUserNode->userCode =
        (char *) malloc(sizeof(char) * (strlen(userCode) + 1));
    if (newUserNode->userCode == NULL) {        /*pointer is null - malloc failure */
        free(newUserNode);      /*free the memory before returning error */
        return E_MALLOC_FAILED;
    }

    /*allocates memory for the newuser's name */
    newUserNode->userName =
        (char *) malloc(sizeof(char) * (strlen(name) + 1));
    if (newUserNode->userName == NULL) {        /*malloc failure test */
        free(newUserNode->userCode);    /*free the memory before returning error */
        free(newUserNode);
        return E_MALLOC_FAILED;
    }

    /*allocates memory for the newuser's emailAddress */
    newUserNode->emailAddress =
        (char *) malloc(sizeof(char) * (strlen(email) + 1));
    if (newUserNode->emailAddress == NULL) {    /*pointer is null - malloc failure */
        free(newUserNode->userName);    /*free the memory before returning error */
        free(newUserNode->userCode);
        free(newUserNode);
        return E_MALLOC_FAILED;
    }

    /*copy param strings into newUsers fields, sets ID */
    newUserNode->ID = makeUserID(userCode);
    strcpy(newUserNode->userCode, userCode);
    strcpy(newUserNode->userName, name);
    strcpy(newUserNode->emailAddress, email);
    if (isLib == TRUE) {
        newUserNode->isLibrarian = TRUE;
    }
    else {
        newUserNode->isLibrarian = FALSE;
    }

    newUserNode->next = firstUser;      /*insert at front of list */
    firstUser = newUserNode;

    /*now its in mem, so save to disk */
    if (saveUser
        (newUserNode->ID, userCode, name, email,
         newUserNode->isLibrarian) < 0)
        return DB_SAVE_FAILURE;

    return newUserNode->ID;
}

/* end addUser()*/

/* Function: addAlbum
 * Params:  char* title, char*artist
 * Returns: int.
 *
 * This function adds an album into the database. It  returns
 * E_INVALID_PARAM if either of the parameters are NULL, and 
 * E_MALLOC_FAILED if a memory allocation failed. Otherwise
 * the ID of the new album is returned. IDs always have values greater
 * than 0.
 * Blank album titles are allowed to be added 
 */
int addAlbum(const char *title, int artistID)
{

    albumNode_t *newAlbumNode = NULL;

    /* Check nextid is ok */
    if (getAlbumExists(_nextAlbumID) == TRUE) {
        return DB_NEXTID_ERROR;
    }

    /*checks whether argument pointers are NULL */
    if (checkString(title) != TRUE) {
        return E_INVALID_PARAM;
    }

    if (getArtistExists(artistID) == FALSE) {
        return E_NOARTIST;
    }

    /* check whether album with this title has already
       been created by this artist */
    {
        /*pointer to list of albums */
        int *allAlbums = getAlbums();
        int i;                  /*counter */

        /*check for album 'already in' library */

        for (i = 0; allAlbums[i] != LAST_ID_IN_ARRAY; i++) {
            char *albumTitle = getAlbumTitle(allAlbums[i]);
            if (strcmp(albumTitle, title) == 0 &&
                artistID == getAlbumArtist(allAlbums[i])) {
                /*album added is 'the same' as another in database */
                free(albumTitle);
                free(allAlbums);
                return ALREADY_ADDED;
            }
            free(albumTitle);
        }
        free(allAlbums);
    }

    /*allocates memory for new Album */
    newAlbumNode = (albumNode_t *) malloc(sizeof(albumNode_t));
    if (newAlbumNode == NULL) { /*malloc failure test */
        return E_MALLOC_FAILED;
    }

    /*allocates memory for the Album's title */
    newAlbumNode->title = (char *) malloc(sizeof(char) * (strlen(title) + 1));
    if (newAlbumNode->title == NULL) {  /*pointer is null - malloc failure */
        free(newAlbumNode);
        return E_MALLOC_FAILED;
    }

    /*copy the param strings into the newAlbum, set ID */
    strcpy(newAlbumNode->title, title);
    newAlbumNode->artistID = artistID;
    /* album is being created for first time - unique id should be created */
    newAlbumNode->ID = _nextAlbumID;    /*this needs changing */

    newAlbumNode->next = firstAlbum;    /*insert at front of list */
    firstAlbum = newAlbumNode;

    /*created in memory so now needs to be saved */
    /*an errror id should be created */
    if (saveAlbum(newAlbumNode->ID, title, artistID) < 0)
        return DB_SAVE_FAILURE;

    _nextAlbumID++;

    if (saveNextID() < 0)
        return DB_SAVE_FAILURE;

    return _nextAlbumID - 1;    /*return id then increment */
}

/*end addAlbum() */

int addArtist(const char *name)
{

    artistNode_t *newArtistNode = NULL;

    /* Check nextid is ok */
    if (getArtistExists(_nextArtistID) == TRUE) {
        return DB_NEXTID_ERROR;
    }

    /*checks whether argument pointers are NULL */
    if (checkString(name) != TRUE) {
        return E_INVALID_PARAM;
    }

    /* check whether artist already exists */
    {
        /*pointer to list of artists */
        int *allArtists = getArtists();
        int i;                  /*counter */

        for (i = 0; allArtists[i] != LAST_ID_IN_ARRAY; i++) {
            char *artistName = getArtistName(allArtists[i]);
            if (strcmp(artistName, name) == 0) {
                /*artist added is 'the same' as another in database */
                free(artistName);
                free(allArtists);
                return ALREADY_ADDED;
            }
            free(artistName);
        }
        free(allArtists);
    }

    /*allocates memory for new Artist */
    newArtistNode = (artistNode_t *) malloc(sizeof(artistNode_t));
    if (newArtistNode == NULL) {        /*malloc failure test */
        return E_MALLOC_FAILED;
    }

    /*allocates memory for the Artist's title */
    newArtistNode->name = (char *) malloc(sizeof(char) * (strlen(name) + 1));
    if (newArtistNode->name == NULL) {  /*pointer is null - malloc failure */
        free(newArtistNode);
        return E_MALLOC_FAILED;
    }

    /*copy the param string into the newArtist */
    strcpy(newArtistNode->name, name);
    /* artist is being created for first time - unique id should be created */
    newArtistNode->ID = _nextArtistID;  /*this needs changing */

    newArtistNode->next = firstArtist;  /*insert at front of list */
    firstArtist = newArtistNode;

    /*created in memory so now needs to be saved */
    if (saveArtist(newArtistNode->ID, name) < 0)
        return DB_SAVE_FAILURE;

    _nextArtistID++;

    if (saveNextID() < 0)
        return DB_SAVE_FAILURE;

    return _nextArtistID - 1;   /*return id then increment  CHANGE */
}                               /*end addArtist() */

/* Function: addComment
 * Params:  int albumID, int userID, char* title, char* body
 * Returns: int.
 *
 * This function adds a comment into the database. It returns
 * E_INVALID_PARAM if either of the title or body parameters are NULL, and 
 * E_MALLOC_FAILED if a memory allocation fails. It returns E_NOUSER if the
 * user does not exist and E_NOALBUM if the album does not exist.
 * Otherwise it returns the ID of the new comment.
 * IDs always have values greater than 0.
 * Blank comment titles and bodies are allowed to be added 
 */

int addUserComment(int userID, int owner, const char *body)
{
    userCommentNode_t *newCommentNode = NULL;

    /* Check nextid is ok */
    if (getUserCommentExists(_nextUserCommentID) == TRUE) {
        return DB_NEXTID_ERROR;
    }

    if (checkString(body) != TRUE) {
        return E_INVALID_PARAM;
    }

    /*test to see if user is in the database */
    if (getUserExists(userID) == FALSE || getUserExists(owner) == FALSE) {
        return E_NOUSER;
    }

    /*allocates memory for newComment */
    newCommentNode = (userCommentNode_t *) malloc(sizeof(userCommentNode_t));
    if (newCommentNode == NULL) {       /*malloc failure? */
        return E_MALLOC_FAILED;
    }

    /*allocates memory for newComment's comment body */
    newCommentNode->comment = malloc(sizeof(char) * (strlen(body) + 1));
    if (newCommentNode->comment == NULL) {      /*malloc failure? */
        free(newCommentNode);
        return E_MALLOC_FAILED;
    }

    /*set comment fields and id fields */
    newCommentNode->userID = userID;
    newCommentNode->userOwner = owner;
    strcpy(newCommentNode->comment, body);

    /*may need changeing */
    newCommentNode->ID = _nextUserCommentID;

    /*insert new node at front of list */
    newCommentNode->next = firstUserComment;
    firstUserComment = newCommentNode;

    /*save to disk return error if not worked */
    if (saveUserComment(newCommentNode->ID, userID, owner, body) < 0)
        return DB_SAVE_FAILURE;

    _nextUserCommentID++;

    if (saveNextID() < 0)
        return DB_SAVE_FAILURE;

    return _nextUserCommentID - 1;      /*return and increment id */
}                               /* end addCommentUser() */

int addAlbumComment(int albumID, int owner, const char *body)
{
    albumCommentNode_t *newCommentNode = NULL;

    /* Check nextid is ok */
    if (getAlbumCommentExists(_nextAlbumCommentID) == TRUE) {
        return DB_NEXTID_ERROR;
    }

    /**null param  check**/
    if (checkString(body) != TRUE) {
        return E_INVALID_PARAM;
    }

    /*test to see if album is in the database */
    if (getAlbumExists(albumID) == FALSE) {
        return E_NOARTIST;
    }

    /*test to see if user is in the database */
    if (getUserExists(owner) == FALSE) {
        return E_NOUSER;
    }

    /*allocates memory for newComment */
    newCommentNode =
        (albumCommentNode_t *) malloc(sizeof(albumCommentNode_t));
    if (newCommentNode == NULL) {       /*malloc failure? */
        return E_MALLOC_FAILED;
    }

    /*allocates memory for newComment's comment body */
    newCommentNode->comment = malloc(sizeof(char) * (strlen(body) + 1));
    if (newCommentNode->comment == NULL) {      /*malloc failure? */
        free(newCommentNode);
        return E_MALLOC_FAILED;
    }

    /*set comment fields and id fields */
    newCommentNode->albumID = albumID;
    newCommentNode->userOwner = owner;
    strcpy(newCommentNode->comment, body);

    /*may need changing */
    newCommentNode->ID = _nextAlbumCommentID;

    /*insert new node at front of list */
    newCommentNode->next = firstAlbumComment;
    firstAlbumComment = newCommentNode;

    /*save to disk */
    if (saveAlbumComment(newCommentNode->ID, albumID, owner, body) < 0)
        return DB_SAVE_FAILURE;

    _nextAlbumCommentID++;

    if (saveNextID() < 0)
        return DB_SAVE_FAILURE;

    return _nextAlbumCommentID - 1;     /*return and increment id */
}                               /* end addCommentAlbum() */

int addArtistComment(int artistID, int owner, const char *body)
{
    artistCommentNode_t *newCommentNode = NULL;

    /* Check nextid is ok */
    if (getArtistCommentExists(_nextArtistCommentID) == TRUE) {
        return DB_NEXTID_ERROR;
    }

    /**null param  check**/
    if (checkString(body) != TRUE) {
        return E_INVALID_PARAM;
    }

    /*test to see if artist is in the database */
    if (getArtistExists(artistID) == FALSE) {
        return E_NOARTIST;
    }

    /*test to see if user is in the database */
    if (getUserExists(owner) == FALSE) {
        return E_NOUSER;
    }

    /*allocates memory for newComment */
    newCommentNode =
        (artistCommentNode_t *) malloc(sizeof(artistCommentNode_t));
    if (newCommentNode == NULL) {       /*malloc failure? */
        return E_MALLOC_FAILED;
    }

    /*allocates memory for newComment's comment body */
    newCommentNode->comment = malloc(sizeof(char) * (strlen(body) + 1));
    if (newCommentNode->comment == NULL) {      /*malloc failure? */
        free(newCommentNode);
        return E_MALLOC_FAILED;
    }

    /*set comment fields and id fields */
    newCommentNode->artistID = artistID;
    newCommentNode->userOwner = owner;
    strcpy(newCommentNode->comment, body);
    /*id setting may change */
    newCommentNode->ID = _nextArtistCommentID;

    /*insert new node at front of list */
    newCommentNode->next = firstArtistComment;
    firstArtistComment = newCommentNode;

    if (saveArtistComment(newCommentNode->ID, artistID, owner, body) < 0)
        return DB_SAVE_FAILURE;

    _nextArtistCommentID++;

    if (saveNextID() < 0)
        return DB_SAVE_FAILURE;

    return _nextArtistCommentID - 1;    /*return and increment id */
}                               /* end addCommentArtist() */

int addLoan(int albumID, int userID)
{
    long tempTime = -1;
    loanNode_t *newLoanNode = NULL;

    /* check next id is ok */
    if (getLoanExists(_nextLoanID) == TRUE) {
        return DB_NEXTID_ERROR;
    }

    /*checks whether user existsL */
    if (getUserExists(userID) == FALSE) {
        return E_NOUSER;
    }

    if (getAlbumExists(albumID) == FALSE) {
        return E_NOALBUM;
    }

    if (getAlbumCurrentLoan(albumID) != E_NOLOAN) {
        return ALREADY_ADDED;
    }

    /*allocates memory for new Loan */
    newLoanNode = (loanNode_t *) malloc(sizeof(loanNode_t));
    if (newLoanNode == NULL) {  /*malloc failure test */
        return E_MALLOC_FAILED;
    }

    newLoanNode->albumID = albumID;
    newLoanNode->userID = userID;

    tempTime = getctime();
    if (tempTime == -1) {
        return DB_BAD_TIME;
    }

    newLoanNode->timeStampIn = tempTime;
    newLoanNode->timeStampOut = -1;
    newLoanNode->isReturned = FALSE;

    /* loan is being created for first time - unique id should be created */
    newLoanNode->ID = _nextLoanID;      /*this needs changing */

    newLoanNode->next = firstLoan;      /*insert at front of list */
    firstLoan = newLoanNode;

    /*created in memory so now needs to be saved */
    if (saveLoan(newLoanNode->ID, albumID, userID, newLoanNode->timeStampIn) <
        0)
        return DB_SAVE_FAILURE;

    _nextLoanID++;

    if (saveNextID() < 0)
        return DB_SAVE_FAILURE;

    return _nextLoanID - 1;     /*return id then increment  CHANGE */
}

int addLoanReturned(int loanID)
{
    long tempTime = -1;

    /*checks whether loan exists */
    if (getLoanExists(loanID) == FALSE) {
        return E_NOLOAN;
    }

    if (isLoanReturned(loanID) == TRUE) {
        return ALREADY_ADDED;
    }

    tempTime = getctime();
    if (tempTime == -1) {
        return DB_BAD_TIME;
    }

    setLoanReturned(loanID, tempTime);

    /*created in memory so now needs to be saved */
    if (saveLoanReturned(loanID, tempTime) < 0)
        return DB_SAVE_FAILURE;

    return 1;
}
