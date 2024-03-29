/***************************************************************************
 *                                                                         *
 * @get.c                                                                  *
 * @version: 19/05/2003
 * @continued 24/7/03
 * @Eden Sinclair, 2020812                                                 *
 *                                                                         *
 * This program provides all the necessary get methods for lib.c.          *
 *                                                                         *
 ***************************************************************************/

#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "../shared/defines.h"
#include "../shared/lib.h"
#include "structs.h"
#include "globals.h"

static char *htmlEscape(const char *, Boolean);

static char *htmlEscape(const char *input, Boolean replaceSpaces)
{
    char *data = NULL;
    char *data2 = NULL;

    if (input == NULL) {
        return NULL;
    }

    if (replaceSpaces != TRUE) {
        replaceSpaces = FALSE;
    }

    /* Copy data */
    data = malloc(sizeof(char) * (strlen(input) + 1));
    if (data == NULL) {
        return NULL;
    }
    strncpy(data, input, strlen(input) + 1);

    data2 = replaceCharWithString(data, '&', "&amp;");
    if (data2 == NULL) {
        free(data);
        return NULL;
    }
    data = data2;

    data2 = replaceCharWithString(data, '"', "&quot;");
    if (data2 == NULL) {
        return NULL;
    }
    data = data2;

    data2 = replaceCharWithString(data, '<', "&lt;");
    if (data2 == NULL) {
        return NULL;
    }
    data = data2;

    data2 = replaceCharWithString(data, '>', "&gt;");
    if (data2 == NULL) {
        return NULL;
    }
    data = data2;

    if (replaceSpaces == TRUE) {
        data2 = replaceCharWithString(data, ' ', "&nbsp;");
        if (data2 == NULL) {
            return NULL;
        }
        data = data2;
    }

    data2 = replaceCharWithString(data, NEWLINE_SUBST, "<br />\n");
    if (data2 == NULL) {
        return NULL;
    }
    data = data2;

    return data;
}

/*--------------- User Functions ----------------------*/

static userNode_t *getUser(int idNumber)
{
    userNode_t *a;

    /**find user id in list, assumes unsorted list**/
    for (a = firstUser; a != NULL; a = a->next) {
        if (a->ID == idNumber) {
            return a;
        }
    }
    return NULL;
}

Boolean getUserExists(int idNumber)
{
    if (getUser(idNumber) == NULL) {
        return FALSE;
    }
    return TRUE;
}

int makeUserID(const char *userCode)
{
    int id = 0;
    int i;
    int len = strlen(userCode);

    for (i = 0; i < len; i++) {
        id = id + (userCode[i] * (31 ^ (len - 1 - i)));
    }

    return id;
}

char *getUserCode(int idNumber)
{
    userNode_t *a;
    a = getUser(idNumber);

    if (a != NULL) {
        return htmlEscape(a->userCode, TRUE);
    }
    return NULL;
}

char *getUserName(int idNumber)
{
    userNode_t *a;
    a = getUser(idNumber);

    if (a != NULL) {
        return htmlEscape(a->userName, TRUE);
    }
    return NULL;
}

char *getUserEmail(int idNumber)
{
    userNode_t *a;
    a = getUser(idNumber);

    if (a != NULL) {
        return htmlEscape(a->emailAddress, TRUE);
    }
    return NULL;
}

Boolean isUserLibrarian(int idNumber)
{
    userNode_t *a;
    a = getUser(idNumber);

    if (a != NULL) {
        return a->isLibrarian;
    }
    return FALSE;
}

/* Function: getUsers
 * Params:  
 * Returns: int*.
 *
 * This function retrieves an array of user IDs. This array should be
 * freed by the caller function. The last id in the array is
 * LAST_ID_IN_ARRAY. For example, if there were three users in the
 * database, with IDs '4', '7', '19', then this function should return
 * this array:
 *
 * [4,7,19,LAST_ID_IN_ARRAY]
 *
 * a memory allocation failure returns NULL.
 */
int *getUsers(void)
{
    int size = 0;   /**number of users in list**/
    int *userArray = NULL;      /*array of users to be returned */
    userNode_t *u;

    size = getUsersCount();

    /*mallocs memory for array and goes back through list, adding id's to array */
    userArray = (int *) malloc(sizeof(int) * (size + 1));
    if (userArray == NULL) {
        return NULL;
    }

    for (u = firstUser, size = 0; u != NULL; u = u->next) {
        userArray[size++] = u->ID;
    }

    userArray[size] = LAST_ID_IN_ARRAY; /*set last field */

    return userArray;
}

/* end getAllUsers()*/

int getUsersCount(void)
{
    int size = 0;   /**number of users in list**/
    userNode_t *u;

    /**count number of users in list,*/
    /*I assume this is faster than calling realloc at every node* */
    for (u = firstUser; u != NULL; u = u->next)
        size++;

    return size;
}

int *getUsersByType(Boolean isLib)
{
    int size = 0;   /**number of users in list**/
    int *userArray = NULL;      /*array of users to be returned */
    userNode_t *u;

    size = getUsersByTypeCount(isLib);

    /*mallocs memory for array and goes back through list, adding id's to array */
    userArray = (int *) malloc(sizeof(int) * (size + 1));
    if (userArray == NULL) {
        return NULL;
    }

    for (u = firstUser, size = 0; u != NULL; u = u->next) {
        if (isUserLibrarian(u->ID) == isLib)
            userArray[size++] = u->ID;
    }

    userArray[size] = LAST_ID_IN_ARRAY; /*set last field */

    return userArray;
}

/* end getAllUsers()*/

int getUsersByTypeCount(Boolean isLib)
{
    int size = 0;   /**number of users in list**/
    userNode_t *u;

    /**count number of users in list,*/
    /*I assume this is faster than calling realloc at every node* */
    for (u = firstUser; u != NULL; u = u->next) {
        if (isUserLibrarian(u->ID) == isLib)
            size++;
    }

    return size;
}

/*--------------- Album Functions ----------------------*/

static albumNode_t *getAlbum(int idNumber)
{
    albumNode_t *a;

    /**find album id in list, assumes unsorted list**/
    for (a = firstAlbum; a != NULL; a = a->next) {
        if (a->ID == idNumber) {
            return a;
        }
    }
    return NULL;
}

Boolean getAlbumExists(int idNumber)
{
    if (getAlbum(idNumber) == NULL) {
        return FALSE;
    }
    return TRUE;
}

/* Function: getAlbumTitle
 * Params:  int idNumber
 * Returns: char*.
 *
 * This function retrieves the title of an album with the specified id. 
 * It returns NULL if the album does not exist in the database.
 */
char *getAlbumTitle(int idNumber)
{
    albumNode_t *a;
    a = getAlbum(idNumber);

    if (a != NULL) {
        return htmlEscape(a->title, TRUE);
    }
    return NULL;
}

/*end getAlbumTitle() */

/*returns id number of albums' artist, or error*/
int getAlbumArtist(int idNumber)
{
    albumNode_t *a;
    a = getAlbum(idNumber);

    if (a != NULL) {
        return a->artistID;
    }
    return E_NOALBUM;
}

/*returns id of loan that shows this album is on loan or error*/
int getAlbumCurrentLoan(int idNumber)
{
    int i;
    int *albumLoans = NULL;
    int toReturn = E_NOLOAN;

    albumLoans = getLoansByAlbum(idNumber, FALSE);
    if (albumLoans != NULL) {
        /* search for a loan that is still not returned */
        for (i = 0; albumLoans[i] != LAST_ID_IN_ARRAY; i++) {
            if (isLoanReturned(albumLoans[i]) == FALSE) {
                toReturn = albumLoans[i];
            }
        }
    }

    free(albumLoans);

    return toReturn;
}

/* Function: getAllAlbums
 * Params:  
 * Returns: int*.
 *
 * This function retrieves an array of album IDs. This array should be
 * freed by the caller function. The last id in the array is
 * LAST_ID_IN_ARRAY. For example, if there were three albums in the
 * database, with IDs '4', '7', '19', then this function returns
 * this array:
 *
 *  [4,7,19,LAST_ID_IN_ARRAY]
 *
 * NULL is returned if a memory allocation error occurred.
 */
int *getAlbums(void)
{
    int size = 0;        /**size will be number of albums in list**/
    int *albumArray = 0;        /*ID array to be returned */
    albumNode_t *a;

    size = getAlbumsCount();

    /*mallocs memory for array and goes back through list, adding id's to array */
    albumArray = (int *) malloc(sizeof(int) * (size + 1));
    if (albumArray == NULL)
        return NULL;

    for (a = firstAlbum, size = 0; a != NULL; a = a->next) {
        albumArray[size++] = a->ID;
    }

    albumArray[size] = LAST_ID_IN_ARRAY;        /*set last field */
    return albumArray;
}

/* end getAllAlbums()*/

int getAlbumsCount(void)
{
    int size = 0;        /**size will be number of albums in list**/
    albumNode_t *a;

    /**counts num of albums in list,*/
    /*I assume this is faster than calling realloc at every node* */
    for (a = firstAlbum; a != NULL; a = a->next)
        size++;

    return size;
}

/*--------------- Artist Functions ----------------------*/

static artistNode_t *getArtist(int idNumber)
{
    artistNode_t *a;

    /**find artist id in list, assumes unsorted list**/
    for (a = firstArtist; a != NULL; a = a->next) {
        if (a->ID == idNumber) {
            return a;
        }
    }
    return NULL;
}

Boolean getArtistExists(int idNumber)
{
    if (getArtist(idNumber) == NULL) {
        return FALSE;
    }
    return TRUE;
}

/* Function: getArtistName
 * Params:  int idNumber
 * Returns: char*.
 *
 * This function retrieves the name of an artist with the specified id. 
 * It returns NULL if the artistdoes not exist in the database.
 */
char *getArtistName(int idNumber)
{
    artistNode_t *a;
    a = getArtist(idNumber);

    if (a != NULL) {
        return htmlEscape(a->name, TRUE);
    }
    return NULL;
}

/*end getArtistName() */

int *getArtists(void)
{
    int size = 0;        /**size will be number of artists in list**/
    int *artistArray = 0;       /*ID array to be returned */
    artistNode_t *a;

    size = getArtistsCount();

    /*mallocs memory for array and goes back through list, adding id's to array */
    artistArray = (int *) malloc(sizeof(int) * (size + 1));
    if (artistArray == NULL)
        return NULL;

    for (a = firstArtist, size = 0; a != NULL; a = a->next) {
        artistArray[size++] = a->ID;
    }

    artistArray[size] = LAST_ID_IN_ARRAY;       /*set last field */
    return artistArray;
}

/* end getAllArtists()*/

int getArtistsCount(void)
{
    int size = 0;        /**size will be number of artists in list**/
    artistNode_t *a;

    /**counts num of artists in list,*/
    /*I assume this is faster than calling realloc at every node* */
    for (a = firstArtist; a != NULL; a = a->next)
        size++;

    return size;
}

int *getArtistAlbums(int idNumber)
{
    int size = 0;        /**size will be number of albums in list**/
    int *albumArray = 0;        /*ID array to be returned */
    albumNode_t *a;

    size = getArtistAlbumsCount(idNumber);

    /*mallocs memory for array and goes back through list, adding id's to array */
    albumArray = (int *) malloc(sizeof(int) * (size + 1));
    if (albumArray == NULL)
        return NULL;

    for (a = firstAlbum, size = 0; a != NULL; a = a->next) {
        if (a->artistID == idNumber)
            albumArray[size++] = a->ID;
    }

    albumArray[size] = LAST_ID_IN_ARRAY;        /*set last field */
    return albumArray;
}

int getArtistAlbumsCount(int idNumber)
{
    int size = 0;        /**size will be number of albums in list**/
    albumNode_t *a;

    /**counts num of albums in list,*/
    /*I assume this is faster than calling realloc at every node* */
    for (a = firstAlbum; a != NULL; a = a->next) {
        if (a->artistID == idNumber)
            size++;
    }

    return size;
}

/*--------------- User Comment Functions ------------------------*/

static userCommentNode_t *getUserComment(int idNumber)
{
    userCommentNode_t *c;

    /**find comment id in list, assumes unsorte
       d list**/
    for (c = firstUserComment; c != NULL; c = c->next) {
        if (c->ID == idNumber) {
            return c;
        }
    }
    return NULL;
}

Boolean getUserCommentExists(int idNumber)
{
    if (getUserComment(idNumber) == NULL) {
        return FALSE;
    }
    return TRUE;
}

int getUserCommentUser(int idNumber)
{
    userCommentNode_t *a;
    a = getUserComment(idNumber);

    if (a != NULL) {
        return a->userID;
    }
    return E_NOUSRCOM;
}

int getUserCommentOwner(int idNumber)
{
    userCommentNode_t *a;
    a = getUserComment(idNumber);

    if (a != NULL) {
        return a->userOwner;
    }
    return E_NOUSRCOM;
}

char *getUserCommentBody(int idNumber)
{
    userCommentNode_t *a;
    a = getUserComment(idNumber);

    if (a != NULL) {
        return htmlEscape(a->comment, FALSE);
    }
    return NULL;
}

int *getUserCommentsByUser(int idNumber)
{
    int size = 0;        /**size will be number of artists in list**/
    int *commentArray = 0;      /*ID array to be returned */
    userCommentNode_t *a;

    size = getUserCommentsByUserCount(idNumber);

    /*mallocs memory for array and goes back through list, adding id's to array */
    commentArray = (int *) malloc(sizeof(int) * (size + 1));
    if (commentArray == NULL)
        return NULL;

    for (a = firstUserComment, size = 0; a != NULL; a = a->next) {
        if (a->userOwner == idNumber)
            commentArray[size++] = a->ID;
    }

    commentArray[size] = LAST_ID_IN_ARRAY;      /*set last field */
    return commentArray;
}

int getUserCommentsByUserCount(int idNumber)
{
    int size = 0;        /**size will be number of artists in list**/
    userCommentNode_t *a;

    /**counts num of artists in list,*/
    /*I assume this is faster than calling realloc at every node* */
    for (a = firstUserComment; a != NULL; a = a->next) {
        if (a->userOwner == idNumber)
            size++;
    }

    return size;
}

int *getUserCommentsForUser(int idNumber)
{
    int size = 0;        /**size will be number of artists in list**/
    int *commentArray = 0;      /*ID array to be returned */
    userCommentNode_t *a;

    size = getUserCommentsForUserCount(idNumber);

    /*mallocs memory for array and goes back through list, adding id's to array */
    commentArray = (int *) malloc(sizeof(int) * (size + 1));
    if (commentArray == NULL)
        return NULL;

    for (a = firstUserComment, size = 0; a != NULL; a = a->next) {
        if (a->userID == idNumber)
            commentArray[size++] = a->ID;
    }

    commentArray[size] = LAST_ID_IN_ARRAY;      /*set last field */
    return commentArray;
}

int getUserCommentsForUserCount(int idNumber)
{
    int size = 0;        /**size will be number of artists in list**/
    userCommentNode_t *a;

    /**counts num of artists in list,*/
    /*I assume this is faster than calling realloc at every node* */
    for (a = firstUserComment; a != NULL; a = a->next) {
        if (a->userID == idNumber)
            size++;
    }

    return size;
}

/*--------------- Album Comment Functions -----------------------*/

static albumCommentNode_t *getAlbumComment(int idNumber)
{
    albumCommentNode_t *c;

    /**find comment id in list, assumes unsorted list**/
    for (c = firstAlbumComment; c != NULL; c = c->next) {
        if (c->ID == idNumber) {
            return c;
        }
    }
    return NULL;
}

Boolean getAlbumCommentExists(int idNumber)
{
    if (getAlbumComment(idNumber) == NULL) {
        return FALSE;
    }
    return TRUE;
}

int getAlbumCommentAlbum(int idNumber)
{
    albumCommentNode_t *a;
    a = getAlbumComment(idNumber);

    if (a != NULL) {
        return a->albumID;
    }
    return E_NOALBCOM;
}

int getAlbumCommentOwner(int idNumber)
{
    albumCommentNode_t *a;
    a = getAlbumComment(idNumber);

    if (a != NULL) {
        return a->userOwner;
    }
    return E_NOALBCOM;
}

char *getAlbumCommentBody(int idNumber)
{
    albumCommentNode_t *a;
    a = getAlbumComment(idNumber);

    if (a != NULL) {
        return htmlEscape(a->comment, FALSE);
    }
    return NULL;
}

int *getAlbumCommentsByUser(int idNumber)
{
    int size = 0;        /**size will be number of artists in list**/
    int *commentArray = 0;      /*ID array to be returned */
    albumCommentNode_t *a;

    size = getAlbumCommentsByUserCount(idNumber);

    /*mallocs memory for array and goes back through list, adding id's to array */
    commentArray = (int *) malloc(sizeof(int) * (size + 1));
    if (commentArray == NULL)
        return NULL;

    for (a = firstAlbumComment, size = 0; a != NULL; a = a->next) {
        if (a->userOwner == idNumber)
            commentArray[size++] = a->ID;
    }

    commentArray[size] = LAST_ID_IN_ARRAY;      /*set last field */
    return commentArray;
}

int getAlbumCommentsByUserCount(int idNumber)
{
    int size = 0;        /**size will be number of artists in list**/
    albumCommentNode_t *a;

    /**counts num of artists in list,*/
    /*I assume this is faster than calling realloc at every node* */
    for (a = firstAlbumComment; a != NULL; a = a->next) {
        if (a->userOwner == idNumber)
            size++;
    }

    return size;
}

int *getAlbumCommentsForAlbum(int idNumber)
{
    int size = 0;        /**size will be number of artists in list**/
    int *commentArray = 0;      /*ID array to be returned */
    albumCommentNode_t *a;

    size = getAlbumCommentsForAlbumCount(idNumber);

    /*mallocs memory for array and goes back through list, adding id's to array */
    commentArray = (int *) malloc(sizeof(int) * (size + 1));
    if (commentArray == NULL)
        return NULL;

    for (a = firstAlbumComment, size = 0; a != NULL; a = a->next) {
        if (a->albumID == idNumber)
            commentArray[size++] = a->ID;
    }

    commentArray[size] = LAST_ID_IN_ARRAY;      /*set last field */
    return commentArray;
}

int getAlbumCommentsForAlbumCount(int idNumber)
{
    int size = 0;        /**size will be number of artists in list**/
    albumCommentNode_t *a;

    /**counts num of artists in list,*/
    /*I assume this is faster than calling realloc at every node* */
    for (a = firstAlbumComment; a != NULL; a = a->next) {
        if (a->albumID == idNumber)
            size++;
    }

    return size;
}

/*--------------- Artist Comment Functions ----------------------*/

/*returns pointer to comment */
static artistCommentNode_t *getArtistComment(int idNumber)
{
    artistCommentNode_t *c;

    /**find comment id in list, assumes unsorted list**/
    for (c = firstArtistComment; c != NULL; c = c->next) {
        if (c->ID == idNumber) {
            return c;
        }
    }
    return NULL;
}

Boolean getArtistCommentExists(int idNumber)
{
    if (getArtistComment(idNumber) == NULL) {
        return FALSE;
    }
    return TRUE;
}

int getArtistCommentArtist(int idNumber)
{
    artistCommentNode_t *a;
    a = getArtistComment(idNumber);

    if (a != NULL) {
        return a->artistID;
    }
    return E_NOARTCOM;
}

int getArtistCommentOwner(int idNumber)
{
    artistCommentNode_t *a;
    a = getArtistComment(idNumber);

    if (a != NULL) {
        return a->userOwner;
    }
    return E_NOARTCOM;
}

char *getArtistCommentBody(int idNumber)
{
    artistCommentNode_t *a;
    a = getArtistComment(idNumber);

    if (a != NULL) {
        return htmlEscape(a->comment, FALSE);
    }
    return NULL;
}

int *getArtistCommentsByUser(int idNumber)
{
    int size = 0;        /**size will be number of artists in list**/
    int *commentArray = 0;      /*ID array to be returned */
    artistCommentNode_t *a;

    size = getArtistCommentsByUserCount(idNumber);

    /*mallocs memory for array and goes back through list, adding id's to array */
    commentArray = (int *) malloc(sizeof(int) * (size + 1));
    if (commentArray == NULL)
        return NULL;

    for (a = firstArtistComment, size = 0; a != NULL; a = a->next) {
        if (a->userOwner == idNumber)
            commentArray[size++] = a->ID;
    }

    commentArray[size] = LAST_ID_IN_ARRAY;      /*set last field */
    return commentArray;
}

int getArtistCommentsByUserCount(int idNumber)
{
    int size = 0;        /**size will be number of artists in list**/
    artistCommentNode_t *a;

    /**counts num of artists in list,*/
    /*I assume this is faster than calling realloc at every node* */
    for (a = firstArtistComment; a != NULL; a = a->next) {
        if (a->userOwner == idNumber)
            size++;
    }

    return size;
}

int *getArtistCommentsForArtist(int idNumber)
{
    int size = 0;        /**size will be number of artists in list**/
    int *commentArray = 0;      /*ID array to be returned */
    artistCommentNode_t *a;

    size = getArtistCommentsForArtistCount(idNumber);

    /*mallocs memory for array and goes back through list, adding id's to array */
    commentArray = (int *) malloc(sizeof(int) * (size + 1));
    if (commentArray == NULL)
        return NULL;

    for (a = firstArtistComment, size = 0; a != NULL; a = a->next) {
        if (a->artistID == idNumber)
            commentArray[size++] = a->ID;
    }

    commentArray[size] = LAST_ID_IN_ARRAY;      /*set last field */
    return commentArray;
}

int getArtistCommentsForArtistCount(int idNumber)
{
    int size = 0;        /**size will be number of artists in list**/
    artistCommentNode_t *a;

    /**counts num of artists in list,*/
    /*I assume this is faster than calling realloc at every node* */
    for (a = firstArtistComment; a != NULL; a = a->next) {
        if (a->artistID == idNumber)
            size++;
    }

    return size;
}

/*--------------- Loan Functions --------------------------------*/

static loanNode_t *getLoan(int idNumber)
{
    loanNode_t *l;

    /**find artist id in list, assumes unsorted list**/
    for (l = firstLoan; l != NULL; l = l->next) {
        if (l->ID == idNumber) {
            return l;
        }
    }
    return NULL;
}

Boolean getLoanExists(int idNumber)
{
    if (getLoan(idNumber) == NULL) {
        return FALSE;
    }
    return TRUE;
}

int getLoanUser(int idNumber)
{
    loanNode_t *a;
    a = getLoan(idNumber);

    if (a != NULL) {
        return a->userID;
    }
    return E_NOLOAN;
}

int getLoanAlbum(int idNumber)
{
    loanNode_t *a;
    a = getLoan(idNumber);

    if (a != NULL) {
        return a->albumID;
    }
    return E_NOLOAN;
}

int getLoanTimeIn(int idNumber)
{
    loanNode_t *a;
    a = getLoan(idNumber);

    if (a != NULL) {
        return a->timeStampIn;
    }
    return E_NOLOAN;
}

int getLoanTimeOut(int idNumber)
{
    loanNode_t *a;
    a = getLoan(idNumber);

    if (a != NULL) {
        return a->timeStampOut;
    }
    return E_NOLOAN;
}

Boolean isLoanReturned(int idNumber)
{
    loanNode_t *a;
    a = getLoan(idNumber);

    if (a != NULL) {
        return a->isReturned;
    }
    return E_NOLOAN;
}

int *getLoansByUser(int idNumber, Boolean isReturned)
{
    int size = 0;        /**size will be number of loans in list**/
    int *loanArray = NULL;      /*ID array to be returned */
    loanNode_t *a;

    if (isReturned != TRUE) {
        isReturned = FALSE;
    }
    else {
        isReturned = TRUE;
    }

    size = getLoansByUserCount(idNumber, isReturned);

    /*mallocs memory for array and goes back through list, adding id's to array */
    loanArray = (int *) malloc(sizeof(int) * (size + 1));
    if (loanArray == NULL)
        return NULL;

    for (a = firstLoan, size = 0; a != NULL; a = a->next) {
        if (a->userID == idNumber && a->isReturned == isReturned)
            loanArray[size++] = a->ID;
    }

    loanArray[size] = LAST_ID_IN_ARRAY; /*set last field */
    return loanArray;
}

int getLoansByUserCount(int idNumber, Boolean isReturned)
{
    int size = 0;        /**size will be number of loans in list**/
    loanNode_t *a;

    if (isReturned != TRUE) {
        isReturned = FALSE;
    }
    else {
        isReturned = TRUE;
    }

    /**counts num of artists in list,*/
    /*I assume this is faster than calling realloc at every node* */
    for (a = firstLoan; a != NULL; a = a->next) {
        if (a->userID == idNumber && a->isReturned == isReturned)
            size++;
    }

    return size;
}

int *getLoansByAlbum(int idNumber, Boolean isReturned)
{
    int size = 0;        /**size will be number of loans in list**/
    int *loanArray = NULL;      /*ID array to be returned */
    loanNode_t *a;

    if (isReturned != TRUE) {
        isReturned = FALSE;
    }
    else {
        isReturned = TRUE;
    }

    size = getLoansByAlbumCount(idNumber, isReturned);

    /*mallocs memory for array and goes back through list, adding id's to array */
    loanArray = (int *) malloc(sizeof(int) * (size + 1));
    if (loanArray == NULL)
        return NULL;

    for (a = firstLoan, size = 0; a != NULL; a = a->next) {
        if (a->albumID == idNumber && a->isReturned == isReturned)
            loanArray[size++] = a->ID;
    }

    loanArray[size] = LAST_ID_IN_ARRAY; /*set last field */
    return loanArray;
}

int getLoansByAlbumCount(int idNumber, Boolean isReturned)
{
    int size = 0;        /**size will be number of loans in list**/
    loanNode_t *a;

    if (isReturned != TRUE) {
        isReturned = FALSE;
    }
    else {
        isReturned = TRUE;
    }

    /**counts num of artists in list,*/
    /*I assume this is faster than calling realloc at every node* */
    for (a = firstLoan; a != NULL; a = a->next) {
        if (a->albumID == idNumber && a->isReturned == isReturned)
            size++;
    }

    return size;
}

int setLoanReturned(int idNumber, int timeOut)
{
    loanNode_t *a;

    a = getLoan(idNumber);

    if (a == NULL) {
        return E_NOLOAN;
    }

    a->isReturned = TRUE;
    a->timeStampOut = timeOut;

    return 1;
}
