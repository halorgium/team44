/***************************************************************************
 *                                                                         *
 * @lib.c                                                                  *
 * @version: 19/05/2003
 * @continued 24/7/03
 * @Eden Sinclair, 2020812                                                 *
 *                                                                         *
 * This program implements a simple in-memory database for storing         *
 * information for a music discussion system.                              *
 * It proveides functions to add new users into the database, add new      *
 * albums to the database, and users can add comments about albums.        *
 * Various getter methods are available which search through the database  *
 *                                                                         *
 ***************************************************************************/

#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "../shared/defines.h"
#include "../shared/lib.h"
#include "structs.h"
#include "globals.h"

static char *htmlEscape(const char* input) {
  char *data=NULL;
  char *temp=NULL;
  char replacee=0;

  if(input == NULL) {
    return "(null)";
  }

  /* Copy data */
  data=malloc(sizeof(char)*(strlen(input)+1));
  strncpy(data, input, strlen(input)+1);

  /* Checking for '&' */
/*   replacee = '&'; */
/*   temp=strchr(data, replacee); */
/*   while(temp != NULL) { */
/*     int lenToCopy=0; */
/*     char *replacement = "&amp;"; */
/*     char *tempA=NULL; */
/*     char *tempB=NULL; */
/*     char *newdata=calloc(sizeof(char), (strlen(data)+strlen(replacement)-1)); */
/*     if(newdata == NULL) return "(null)"; */

/*     tempA=data; */
/*     tempB=newdata; */

    /* copy pre-string */
/*     lenToCopy=strlen(tempA)-strlen(temp); */
/*     if(lenToCopy > 0) { */
/*       strncpy(tempB, tempA, lenToCopy); */
/*       tempB=tempB+lenToCopy; */
/*       tempA=tempA+lenToCopy; */
/*     } */
/*     tempA++; */

    /* copy replacement */
/*     strncpy(tempB, replacement, strlen(replacement)); */
/*     tempB=tempB+strlen(replacement); */

    /* copy post-string */
/*     lenToCopy=strlen(tempA)+1; */
/*     if(lenToCopy > 0) { */
/*       strncpy(tempB, tempA, lenToCopy); */
/*     } */

/*     temp=newdata+(strlen(data)-strlen(temp)); */

    /* free old data */
/*     free(data); */
    /* set to newdata */
/*     data=newdata; */

/*     temp=strchr(temp+1, replacee); */
/*   } */

  /* Checking for '<' */
  replacee = '<';
  temp=strchr(data, replacee);
  while(temp != NULL) {
    int lenToCopy=0;
    char *replacement = "&lt;";
    char *tempA=NULL;
    char *tempB=NULL;
    char *newdata=calloc(sizeof(char), (strlen(data)+strlen(replacement)));
    if(newdata == NULL) return "(null)";

    tempA=data;
    tempB=newdata;

    /* copy pre-string */
    lenToCopy=strlen(tempA)-strlen(temp);
    if(lenToCopy > 0) {
      strncpy(tempB, tempA, lenToCopy);
      tempB=tempB+lenToCopy;
      tempA=tempA+lenToCopy;
    }
    tempA++;

    /* copy replacement */
    strncpy(tempB, replacement, strlen(replacement));
    tempB=tempB+strlen(replacement);

    /* copy post-string */
    lenToCopy=strlen(tempA)+1;
    if(lenToCopy > 0) {
      strncpy(tempB, tempA, lenToCopy);
    }

    /* free old data */
    free(data);
    /* set to newdata */
    data=newdata;

    temp=strchr(data, replacee);
  }

  /* Checking for '>' */
  replacee = '>';
  temp=strchr(data, replacee);
  while(temp != NULL) {
    int lenToCopy=0;
    char *replacement = "&gt;";
    char *tempA=NULL;
    char *tempB=NULL;
    char *newdata=calloc(sizeof(char), (strlen(data)+strlen(replacement)));
    if(newdata == NULL) return "(null)";

    tempA=data;
    tempB=newdata;

    /* copy pre-string */
    lenToCopy=strlen(tempA)-strlen(temp);
    if(lenToCopy > 0) {
      strncpy(tempB, tempA, lenToCopy);
      tempB=tempB+lenToCopy;
      tempA=tempA+lenToCopy;
    }
    tempA++;

    /* copy replacement */
    strncpy(tempB, replacement, strlen(replacement));
    tempB=tempB+strlen(replacement);

    /* copy post-string */
    lenToCopy=strlen(tempA)+1;
    if(lenToCopy > 0) {
      strncpy(tempB, tempA, lenToCopy);
    }

    /* free old data */
    free(data);
    /* set to newdata */
    data=newdata;

    temp=strchr(data, replacee);
  }

  return data;
}

/*--------------- User Functions ----------------------*/

static userNode_t *getUser(int idNumber){
    userNode_t *a; 
  
    /**find user id in list, assumes unsorted list**/
    for(a = firstUser; a != NULL; a=a->next){
	if(a->ID == idNumber){
	    return a;
	}
    }
    return NULL;
}

Boolean getUserExists(int idNumber) {
    if(getUser(idNumber) == NULL) {
	return FALSE;
    }
    return TRUE;
}

int makeUserID(const char *userCode) {
    int id = 0;
    int i;
    int len = strlen(userCode);

    for(i = 0; i < len; i++){
	id = id + (userCode[i] * (31^(len-1-i)));
    }

    return id;
}

const char* getUserCode(int idNumber) {
    userNode_t *a;
    a = getUser(idNumber);
    
    if(a != NULL) {
	return htmlEscape(a->userCode);
    }
    return NULL;
}

const char* getUserName(int idNumber) {
    userNode_t *a;
    a = getUser(idNumber);

    if(a != NULL) {
	return htmlEscape(a->userName);
    }
    return NULL;
}

const char* getUserEmail(int idNumber) {
    userNode_t *a;
    a = getUser(idNumber);
    
    if(a != NULL) {
	return htmlEscape(a->emailAddress);
    }
    return NULL;
}

Boolean isUserLibrarian(int idNumber) {
    userNode_t *a;
    a = getUser(idNumber);
    
    if(a != NULL) {
	return a->isLibrarian;
    }
    return FALSE;
}

/* Function: getAllUsers
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
int *getUsers(void){
    int size = 0;   /**number of users in list**/
    int *userArray=NULL;  /*array of users to be returned*/ 
    userNode_t *u;
    
    size=getUsersCount();

    /*mallocs memory for array and goes back through list, adding id's to array*/
    userArray = (int *) malloc(sizeof(int)*(size+1));
    if(userArray == NULL){
	return NULL;
    }
    
    for(u=firstUser, size=0; u != NULL; u=u->next){
	userArray[size++] = u->ID;
    }

    userArray[size] = LAST_ID_IN_ARRAY; /*set last field*/
    
    return userArray;
}
/* end getAllUsers()*/

int getUsersCount(void) {
    int size = 0;   /**number of users in list**/
    userNode_t *u;
    
    /**count number of users in list,*/
    /*I assume this is faster than calling realloc at every node**/
    for(u = firstUser; u != NULL; u=u->next) size++;

    return size;
}

/*--------------- Album Functions ----------------------*/

static albumNode_t *getAlbum(int idNumber){
    albumNode_t *a; 
    
    /**find album id in list, assumes unsorted list**/
    for(a = firstAlbum; a != NULL; a=a->next){
	if(a->ID == idNumber){
	    return a;
	} 
    }
    return NULL;
}

Boolean getAlbumExists(int idNumber) {
    if(getAlbum(idNumber) == NULL) {
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
const char* getAlbumTitle(int idNumber){
    albumNode_t *a;
    a = getAlbum(idNumber);
    
    if(a != NULL) {
	return htmlEscape(a->title);
    }
    return NULL;
}
/*end getAlbumTitle() */

int getAlbumArtist(int idNumber) {
    albumNode_t *a;
    a = getAlbum(idNumber);
    
    if(a != NULL) {
	return a->artistID;
    }
    return E_NOALBUM;
}

/*returns id of loan that shows this album is on loan or error*/
int getAlbumCurrentLoan(int idNumber){
    int i;
    int *albumLoans = NULL;

    albumLoans = getLoansByAlbum(idNumber, FALSE);
    if(albumLoans != NULL) {
	/* search for a loan that is still not returned*/
	for(i = 0; albumLoans[i] != LAST_ID_IN_ARRAY; i++) {
	    if(isLoanReturned(albumLoans[i]) == FALSE) return albumLoans[i];
	}
    }
    
    return E_NOLOAN;
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
int *getAlbums(void){
    int size = 0;        /**size will be number of albums in list**/
    int *albumArray = 0;    /*ID array to be returned*/
    albumNode_t *a;

    size=getAlbumsCount();

    /*mallocs memory for array and goes back through list, adding id's to array*/
    albumArray = (int*) malloc(sizeof(int)*(size+1));
    if(albumArray == NULL) return NULL;
    
    for(a=firstAlbum, size=0; a != NULL; a=a->next){
	albumArray[size++] = a->ID;
    }
    
    albumArray[size] = LAST_ID_IN_ARRAY; /*set last field*/
    return albumArray;
}
/* end getAllAlbums()*/

int getAlbumsCount(void) {
    int size = 0;        /**size will be number of albums in list**/
    albumNode_t *a;
    
    /**counts num of albums in list,*/
    /*I assume this is faster than calling realloc at every node**/
    for(a = firstAlbum; a != NULL; a=a->next) size++;

    return size;
}

/*--------------- Artist Functions ----------------------*/

static artistNode_t *getArtist(int idNumber) {
    artistNode_t *a; 
    
    /**find artist id in list, assumes unsorted list**/
    for(a = firstArtist; a != NULL; a=a->next){
	if(a->ID == idNumber){
	    return a;
	} 
    }
    return NULL;
}

Boolean getArtistExists(int idNumber) {
    if(getArtist(idNumber) == NULL) {
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
const char* getArtistName(int idNumber){
    artistNode_t *a;
    a = getArtist(idNumber);
    
    if(a != NULL) {
	return htmlEscape(a->name);
    }
    return NULL;
}
/*end getArtistName() */

int *getArtists(void){
    int size = 0;        /**size will be number of artists in list**/
    int *artistArray = 0;    /*ID array to be returned*/
    artistNode_t *a;
    
    size=getArtistsCount();

    /*mallocs memory for array and goes back through list, adding id's to array*/
    artistArray = (int*) malloc(sizeof(int)*(size+1));
    if(artistArray == NULL) return NULL;
    
    for(a=firstArtist, size=0; a != NULL; a=a->next){
	artistArray[size++] = a->ID;
    }
    
    artistArray[size] = LAST_ID_IN_ARRAY; /*set last field*/
    return artistArray;
}
/* end getAllArtists()*/

int getArtistsCount(void) {
    int size = 0;        /**size will be number of artists in list**/
    artistNode_t *a;
    
    /**counts num of artists in list,*/
    /*I assume this is faster than calling realloc at every node**/
    for(a = firstArtist; a != NULL; a=a->next) size++;

    return size;
}

int *getArtistAlbums(int idNumber) {
    int size = 0;        /**size will be number of albums in list**/
    int *albumArray = 0;    /*ID array to be returned*/
    albumNode_t *a;
    
    size=getArtistAlbumsCount(idNumber);
    
    /*mallocs memory for array and goes back through list, adding id's to array*/
    albumArray = (int*) malloc(sizeof(int)*(size+1));
    if(albumArray == NULL) return NULL;
    
    for(a=firstAlbum, size=0; a != NULL; a=a->next){
	if(a->artistID == idNumber) albumArray[size++] = a->ID;
    }
    
    albumArray[size] = LAST_ID_IN_ARRAY; /*set last field*/
    return albumArray;
}

int getArtistAlbumsCount(int idNumber) {
    int size = 0;        /**size will be number of albums in list**/
    albumNode_t *a;
    
    /**counts num of albums in list,*/
    /*I assume this is faster than calling realloc at every node**/
    for(a = firstAlbum; a != NULL; a=a->next) {
	if(a->artistID == idNumber) size++;
    }

    return size;
}

/*--------------- User Comment Functions ------------------------*/

static userCommentNode_t *getUserComment(int idNumber){
    userCommentNode_t *c; 
    
    /**find comment id in list, assumes unsorte
       d list**/
    for(c = firstUserComment; c != NULL; c=c->next){
	if(c->ID == idNumber){
	    return c;
	} 
    }
    return NULL;
}

int getUserCommentUser(int idNumber) {
    userCommentNode_t *a;
    a = getUserComment(idNumber);
    
    if(a != NULL) {
	return a->userID;
    }
    return E_NOUSRCOM;
}

int getUserCommentOwner(int idNumber) {
    userCommentNode_t *a;
    a = getUserComment(idNumber);
    
    if(a != NULL) {
	return a->userOwner;
    }
    return E_NOUSRCOM;
}

const char *getUserCommentBody(int idNumber) {
    userCommentNode_t *a;
    a = getUserComment(idNumber);
    
    if(a != NULL) {
	return htmlEscape(a->comment);
    }
    return NULL;
}

int *getUserCommentsByUser(int idNumber) {
    int size = 0;        /**size will be number of artists in list**/
    int *commentArray = 0;    /*ID array to be returned*/
    userCommentNode_t *a;
    
    size=getUserCommentsByUserCount(idNumber);

    /*mallocs memory for array and goes back through list, adding id's to array*/
    commentArray = (int*) malloc(sizeof(int)*(size+1));
    if(commentArray == NULL) return NULL;
    
    for(a=firstUserComment, size=0; a != NULL; a=a->next){
	if(a->userOwner == idNumber) commentArray[size++] = a->ID;
    }
    
    commentArray[size] = LAST_ID_IN_ARRAY; /*set last field*/
    return commentArray;
}

int getUserCommentsByUserCount(int idNumber) {
    int size = 0;        /**size will be number of artists in list**/
    userCommentNode_t *a;
    
    /**counts num of artists in list,*/
    /*I assume this is faster than calling realloc at every node**/
    for(a = firstUserComment; a != NULL; a=a->next) {
	if(a->userOwner == idNumber) size++;
    }

    return size;
}

int *getUserCommentsForUser(int idNumber) {
    int size = 0;        /**size will be number of artists in list**/
    int *commentArray = 0;    /*ID array to be returned*/
    userCommentNode_t *a;
    
    size=getUserCommentsForUserCount(idNumber);

    /*mallocs memory for array and goes back through list, adding id's to array*/
    commentArray = (int*) malloc(sizeof(int)*(size+1));
    if(commentArray == NULL) return NULL;
    
    for(a=firstUserComment, size=0; a != NULL; a=a->next){
	if(a->userID == idNumber) commentArray[size++] = a->ID;
    }
    
    commentArray[size] = LAST_ID_IN_ARRAY; /*set last field*/
    return commentArray;
}

int getUserCommentsForUserCount(int idNumber) {
    int size = 0;        /**size will be number of artists in list**/
    userCommentNode_t *a;
    
    /**counts num of artists in list,*/
    /*I assume this is faster than calling realloc at every node**/
    for(a = firstUserComment; a != NULL; a=a->next) {
	if(a->userOwner == idNumber) size++;
    }

    return size;
}

/*--------------- Album Comment Functions -----------------------*/

static albumCommentNode_t *getAlbumComment(int idNumber){
    albumCommentNode_t *c; 
    
    /**find comment id in list, assumes unsorted list**/
    for(c = firstAlbumComment; c != NULL; c=c->next){
	if(c->ID == idNumber){
	    return c;
	} 
    }
    return NULL;
}

int getAlbumCommentAlbum(int idNumber) {
    albumCommentNode_t *a;
    a = getAlbumComment(idNumber);
    
    if(a != NULL) {
	return a->albumID;
    }
    return E_NOALBCOM;
}

int getAlbumCommentOwner(int idNumber) {
    albumCommentNode_t *a;
    a = getAlbumComment(idNumber);
    
    if(a != NULL) {
	return a->userOwner;
    }
    return E_NOALBCOM;
}

const char *getAlbumCommentBody(int idNumber) {
    albumCommentNode_t *a;
    a = getAlbumComment(idNumber);
    
    if(a != NULL) {
	return htmlEscape(a->comment);
    }
    return NULL;
}

int *getAlbumCommentsByUser(int idNumber) {
    int size = 0;        /**size will be number of artists in list**/
    int *commentArray = 0;    /*ID array to be returned*/
    albumCommentNode_t *a;
    
    size=getAlbumCommentsByUserCount(idNumber);

    /*mallocs memory for array and goes back through list, adding id's to array*/
    commentArray = (int*) malloc(sizeof(int)*(size+1));
    if(commentArray == NULL) return NULL;
    
    for(a=firstAlbumComment, size=0; a != NULL; a=a->next){
	if(a->userOwner == idNumber) commentArray[size++] = a->ID;
    }
    
    commentArray[size] = LAST_ID_IN_ARRAY; /*set last field*/
    return commentArray;
}

int getAlbumCommentsByUserCount(int idNumber) {
    int size = 0;        /**size will be number of artists in list**/
    albumCommentNode_t *a;
    
    /**counts num of artists in list,*/
    /*I assume this is faster than calling realloc at every node**/
    for(a = firstAlbumComment; a != NULL; a=a->next) {
	if(a->userOwner == idNumber) size++;
    }

    return size;
}

int *getAlbumCommentsForAlbum(int idNumber) {
    int size = 0;        /**size will be number of artists in list**/
    int *commentArray = 0;    /*ID array to be returned*/
    albumCommentNode_t *a;
    
    size=getAlbumCommentsForAlbumCount(idNumber);

    /*mallocs memory for array and goes back through list, adding id's to array*/
    commentArray = (int*) malloc(sizeof(int)*(size+1));
    if(commentArray == NULL) return NULL;
    
    for(a=firstAlbumComment, size=0; a != NULL; a=a->next){
	if(a->albumID == idNumber) commentArray[size++] = a->ID;
    }
    
    commentArray[size] = LAST_ID_IN_ARRAY; /*set last field*/
    return commentArray;
}

int getAlbumCommentsForAlbumCount(int idNumber) {
    int size = 0;        /**size will be number of artists in list**/
    albumCommentNode_t *a;
    
    /**counts num of artists in list,*/
    /*I assume this is faster than calling realloc at every node**/
    for(a = firstAlbumComment; a != NULL; a=a->next) {
	if(a->userOwner == idNumber) size++;
    }

    return size;
}

/*--------------- Artist Comment Functions ----------------------*/

/*returns pointer to comment */
static artistCommentNode_t *getArtistComment(int idNumber){
    artistCommentNode_t *c; 
    
    /**find comment id in list, assumes unsorted list**/
    for(c = firstArtistComment; c != NULL; c=c->next){
	if(c->ID == idNumber) {
	    return c;
	} 
    }
    return NULL;
}

Boolean getArtistCommentExists(int idNumber) {
    if(getArtistComment(idNumber) == NULL) {
	return FALSE;
    }
    return TRUE;
}


int getArtistCommentArtist(int idNumber) {
    artistCommentNode_t *a;
    a = getArtistComment(idNumber);
    
    if(a != NULL) {
	return a->artistID;
    }
    return E_NOARTCOM;
}

int getArtistCommentOwner(int idNumber) {
    artistCommentNode_t *a;
    a = getArtistComment(idNumber);
    
    if(a != NULL) {
	return a->userOwner;
    }
    return E_NOARTCOM;
}

const char *getArtistCommentBody(int idNumber) {
    artistCommentNode_t *a;
    a = getArtistComment(idNumber);
    
    if(a != NULL) {
	return htmlEscape(a->comment);
    }
    return NULL;
}

int *getArtistCommentsByUser(int idNumber) {
    int size = 0;        /**size will be number of artists in list**/
    int *commentArray = 0;    /*ID array to be returned*/
    artistCommentNode_t *a;
    
    size=getArtistCommentsByUserCount(idNumber);

    /*mallocs memory for array and goes back through list, adding id's to array*/
    commentArray = (int*) malloc(sizeof(int)*(size+1));
    if(commentArray == NULL) return NULL;
    
    for(a=firstArtistComment, size=0; a != NULL; a=a->next){
	if(a->userOwner == idNumber) commentArray[size++] = a->ID;
    }
    
    commentArray[size] = LAST_ID_IN_ARRAY; /*set last field*/
    return commentArray;
}

int getArtistCommentsByUserCount(int idNumber) {
    int size = 0;        /**size will be number of artists in list**/
    artistCommentNode_t *a;
    
    /**counts num of artists in list,*/
    /*I assume this is faster than calling realloc at every node**/
    for(a = firstArtistComment; a != NULL; a=a->next) {
	if(a->userOwner == idNumber) size++;
    }

    return size;
}

int *getArtistCommentsForArtist(int idNumber) {
    int size = 0;        /**size will be number of artists in list**/
    int *commentArray = 0;    /*ID array to be returned*/
    artistCommentNode_t *a;
    
    size=getArtistCommentsForArtistCount(idNumber);

    /*mallocs memory for array and goes back through list, adding id's to array*/
    commentArray = (int*) malloc(sizeof(int)*(size+1));
    if(commentArray == NULL) return NULL;
    
    for(a=firstArtistComment, size=0; a != NULL; a=a->next){
	if(a->artistID == idNumber) commentArray[size++] = a->ID;
    }
    
    commentArray[size] = LAST_ID_IN_ARRAY; /*set last field*/
    return commentArray;
}

int getArtistCommentsForArtistCount(int idNumber) {
    int size = 0;        /**size will be number of artists in list**/
    artistCommentNode_t *a;
    
    /**counts num of artists in list,*/
    /*I assume this is faster than calling realloc at every node**/
    for(a = firstArtistComment; a != NULL; a=a->next) {
	if(a->userOwner == idNumber) size++;
    }

    return size;
}

/*--------------- Loan Functions --------------------------------*/

static loanNode_t *getLoan(int idNumber){
    loanNode_t *l; 
    
    /**find artist id in list, assumes unsorted list**/
    for(l = firstLoan; l != NULL; l=l->next){
	if(l->ID == idNumber){
	    return l;
	}
    }
    return NULL;
}

Boolean getLoanExists(int idNumber) {
    if(getLoan(idNumber) == NULL) {
	return FALSE;
    }
    return TRUE;
}

int getLoanUser(int idNumber) {
    loanNode_t *a;
    a = getLoan(idNumber);
    
    if(a != NULL) {
	return a->userID;
    }
    return E_NOLOAN;
}

int getLoanAlbum(int idNumber) {
    loanNode_t *a;
    a = getLoan(idNumber);
    
    if(a != NULL) {
	return a->albumID;
    }
    return E_NOLOAN;
}

int getLoanTimeIn(int idNumber) {
    loanNode_t *a;
    a = getLoan(idNumber);
    
    if(a != NULL) {
	return a->timeStampIn;
    }
    return E_NOLOAN;
}

int getLoanTimeOut(int idNumber) {
    loanNode_t *a;
    a = getLoan(idNumber);
    
    if(a != NULL) {
	return a->timeStampOut;
    }
    return E_NOLOAN;
}

Boolean isLoanReturned(int idNumber) {
    loanNode_t *a;
    a = getLoan(idNumber);
    
    if(a != NULL) {
	return a->isReturned;
    }
    return E_NOLOAN;
}

int *getLoansByUser(int idNumber, Boolean isReturned){
    int size = 0;        /**size will be number of loans in list**/
    int *loanArray = NULL;    /*ID array to be returned*/
    loanNode_t *a;

    if(isReturned != TRUE) {
	isReturned = FALSE;
    }
    else {
	isReturned = TRUE;
    }

    size=getLoansByUserCount(idNumber, isReturned);

    /*mallocs memory for array and goes back through list, adding id's to array*/
    loanArray = (int*) malloc(sizeof(int)*(size+1));
    if(loanArray == NULL) return NULL;
    
    for(a=firstLoan, size=0; a != NULL; a=a->next){
	if(a->userID == idNumber && a->isReturned == isReturned) loanArray[size++] = a->ID;
    }
    
    loanArray[size] = LAST_ID_IN_ARRAY; /*set last field*/
    return loanArray;
}

int getLoansByUserCount(int idNumber, Boolean isReturned) {
    int size = 0;        /**size will be number of loans in list**/
    loanNode_t *a;

    if(isReturned != TRUE) {
	isReturned = FALSE;
    }
    else {
	isReturned = TRUE;
    }
  
    /**counts num of artists in list,*/
    /*I assume this is faster than calling realloc at every node**/
    for(a = firstLoan; a != NULL; a=a->next) {
	if(a->userID == idNumber && a->isReturned == isReturned) size++;
    }

    return size;
}

int *getLoansByAlbum(int idNumber, Boolean isReturned) {
    int size = 0;        /**size will be number of loans in list**/
    int *loanArray = NULL;    /*ID array to be returned*/
    loanNode_t *a;

    if(isReturned != TRUE) {
	isReturned = FALSE;
    }
    else {
	isReturned = TRUE;
    }
  
    size=getLoansByAlbumCount(idNumber, isReturned);

    /*mallocs memory for array and goes back through list, adding id's to array*/
    loanArray = (int*) malloc(sizeof(int)*(size+1));
    if(loanArray == NULL) return NULL;
    
    for(a=firstLoan, size=0; a != NULL; a=a->next){
	if(a->albumID == idNumber && a->isReturned == isReturned) loanArray[size++] = a->ID;
    }

    loanArray[size] = LAST_ID_IN_ARRAY; /*set last field*/
    return loanArray;
}

int getLoansByAlbumCount(int idNumber, Boolean isReturned) {
    int size = 0;        /**size will be number of loans in list**/
    loanNode_t *a;

    if(isReturned != TRUE) {
	isReturned = FALSE;
    }
    else {
	isReturned = TRUE;
    }
  
    /**counts num of artists in list,*/
    /*I assume this is faster than calling realloc at every node**/
    for(a = firstLoan; a != NULL; a=a->next) {
	if(a->albumID == idNumber && a->isReturned == isReturned) size++;
    }

    return size;
}

int setLoanReturned(int idNumber, int timeOut) {
    loanNode_t *a;

    a=getLoan(idNumber);

    if(a == NULL) {
	return E_NOLOAN;
    }

    a->isReturned = TRUE;
    a->timeStampOut = timeOut;

    return 1;
}
