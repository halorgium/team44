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
#include "../shared/structs.h"
#include "../shared/lib.h"
#include "globals.h"

/*'private' method construct*/
int *getAllLoansByStatus(Boolean isReturned);

/*--------------- User Functions ----------------------*/

userNode_t *getUser(int userID){
    userNode_t *a; 
    
    /**find user id in list, assumes unsorted list**/
    for(a = firstUser; a != NULL; a=a->next){
	if(a->ID == userID){
	    return a;
 	} 
    }
    return NULL;
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
int *getAllUsers(void){
    
    int size = 0;   /**number of users in list**/
    int *userArray=NULL;  /*array of users to be returned*/ 
    userNode_t *u;
    
    /**count number of users in list,*/
    /*I assume this is faster than calling realloc at every node**/
    for(u = firstUser; u != NULL; u=u->next){
	size++;
    }

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

int makeUserID(char *userCode) {
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
	return a->userCode;
    }
    return NULL;
}

const char* getUserName(int idNumber) {
    userNode_t *a;
    a = getUser(idNumber);
    
    if(a != NULL) {
	return a->userName;
    }
    return NULL;
}

const char* getUserEmail(int idNumber) {
    userNode_t *a;
    a = getUser(idNumber);
    
    if(a != NULL) {
	return a->emailAddress;
    }
    return NULL;
}

Boolean isUserInDatabase(int idNumber){
    userNode_t *a;
    a = getUser(idNumber);
    
    if(a != NULL) {
	return TRUE;
    }
    return FALSE;
}

Boolean isUserLibrarian(int idNumber) {
    userNode_t *a;
    a = getUser(idNumber);
    
    if(a != NULL) {
	return a->isLibrarian;
    }
    return FALSE;
}

/*--------------- Album Functions ----------------------*/

albumNode_t *getAlbum(int albumID){
    albumNode_t *a; 
    
    /**find album id in list, assumes unsorted list**/
    for(a = firstAlbum; a != NULL; a=a->next){
	if(a->ID == albumID){
	    return a;
 	} 
    }
    return NULL;
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
int *getAllAlbums(void){
    
    int size = 0;        /**size will be number of albums in list**/
    int *albumArray = 0;    /*ID array to be returned*/
    albumNode_t *a;
    
    /**counts num of albums in list,*/
    /*I assume this is faster than calling realloc at every node**/
    for(a = firstAlbum; a != NULL; a=a->next) size++;

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
	return a->title;
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

/*returns array of ids terminated by LAST_ID...*/
int *getAlbumComments(int idNumber){
    int size = 0;
    int *commentArray = NULL;
    albumCommentNode_t *com;
    
    /**counts num of albums in list,*/
    /*I assume this is faster than calling realloc at every node**/
    for(com = firstAlbumComment; com != NULL; com=com->next) {
	if(com->albumID == idNumber) size++;
    }

    /*mallocs memory for array and goes back through list, adding id's to array*/
    commentArray = (int*) malloc(sizeof(int)*(size+1));
    if(commentArray == NULL) return NULL;
    
    for(com=firstAlbumComment, size=0; com != NULL;com=com->next){
	if(com->albumID == idNumber) commentArray[size++] = com->ID;
    }
    
    commentArray[size] = LAST_ID_IN_ARRAY; /*set last field*/

    return commentArray;
}

/*returns id of loan that shows this album is on loan or error*/
int getAlbumCurrentLoan(int idNumber){
    int i;
    int *albumLoans = NULL;

    albumLoans = getLoansByAlbum(idNumber);
    if(albumLoans != NULL) {
	/* search for a loan that is still not returned*/
	for(i = 0; albumLoans[i] != LAST_ID_IN_ARRAY; i++) {
	    if(getLoan(albumLoans[i])->isReturned == FALSE)
		return albumLoans[i];
	}
    }
    
    return -1;
}

/*--------------- Artist Functions ----------------------*/

artistNode_t *getArtist(int artistID){
    artistNode_t *a; 
    
    /**find artist id in list, assumes unsorted list**/
    for(a = firstArtist; a != NULL; a=a->next){
	if(a->ID == artistID){
	    return a;
 	} 
    }
    return NULL;
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
int *getAllArtists(void){
    int size = 0;        /**size will be number of artists in list**/
    int *artistArray = 0;    /*ID array to be returned*/
    artistNode_t *a;
    
    /**counts num of artists in list,*/
    /*I assume this is faster than calling realloc at every node**/
    for(a = firstArtist; a != NULL; a=a->next) size++;

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
	return a->name;
    }
    return NULL;
}
/*end getArtistName() */

int *getArtistAlbums(int idNumber) {
    int size = 0;        /**size will be number of albums in list**/
    int *albumArray = 0;    /*ID array to be returned*/
    albumNode_t *a;
    
    /**counts num of albums in list,*/
    /*I assume this is faster than calling realloc at every node**/
    for(a = firstAlbum; a != NULL; a=a->next) {
	if(a->artistID == idNumber) size++;
    }
    
    /*mallocs memory for array and goes back through list, adding id's to array*/
    albumArray = (int*) malloc(sizeof(int)*(size+1));
    if(albumArray == NULL) return NULL;
    
    for(a=firstAlbum, size=0; a != NULL; a=a->next){
	if(a->artistID == idNumber) albumArray[size++] = a->ID;
    }
    
    albumArray[size] = LAST_ID_IN_ARRAY; /*set last field*/
    return albumArray;
}

int *getArtistComments(int idNumber) {
    return NULL;
}

/*--------------- User Comment Functions ------------------------*/

userCommentNode_t *getUserComment(int commentID){
    userCommentNode_t *c; 
    
    /**find comment id in list, assumes unsorte
       d list**/
    for(c = firstUserComment; c != NULL; c=c->next){
	if(c->ID == commentID){
	    return c;
 	} 
    }
    return NULL;
}

int *getUserCommentsByUser(int idNumber) {
    int size = 0;        /**size will be number of artists in list**/
    int *commentArray = 0;    /*ID array to be returned*/
    userCommentNode_t *a;
    
    /**counts num of artists in list,*/
    /*I assume this is faster than calling realloc at every node**/
    for(a = firstUserComment; a != NULL; a=a->next) {
	if(a->userOwner == idNumber) size++;
    }

    /*mallocs memory for array and goes back through list, adding id's to array*/
    commentArray = (int*) malloc(sizeof(int)*(size+1));
    if(commentArray == NULL) return NULL;
    
    for(a=firstUserComment, size=0; a != NULL; a=a->next){
	if(a->userOwner == idNumber) commentArray[size++] = a->ID;
    }
    
    commentArray[size] = LAST_ID_IN_ARRAY; /*set last field*/
    return commentArray;
}

int *getUserCommentsForUser(int idNumber) {
    int size = 0;        /**size will be number of artists in list**/
    int *commentArray = 0;    /*ID array to be returned*/
    userCommentNode_t *a;
    
    /**counts num of artists in list,*/
    /*I assume this is faster than calling realloc at every node**/
    for(a = firstUserComment; a != NULL; a=a->next) {
	if(a->userOwner == idNumber) size++;
    }

    /*mallocs memory for array and goes back through list, adding id's to array*/
    commentArray = (int*) malloc(sizeof(int)*(size+1));
    if(commentArray == NULL) return NULL;
    
    for(a=firstUserComment, size=0; a != NULL; a=a->next){
	if(a->userID == idNumber) commentArray[size++] = a->ID;
    }
    
    commentArray[size] = LAST_ID_IN_ARRAY; /*set last field*/
    return commentArray;
}

/*--------------- Album Comment Functions -----------------------*/

albumCommentNode_t *getAlbumComment(int commentID){
    albumCommentNode_t *c; 
    
    /**find comment id in list, assumes unsorted list**/
    for(c = firstAlbumComment; c != NULL; c=c->next){
	if(c->ID == commentID){
	    return c;
 	} 
    }
    return NULL;
}

int *getAlbumCommentsByUser(int idNumber) {
    int size = 0;        /**size will be number of artists in list**/
    int *commentArray = 0;    /*ID array to be returned*/
    albumCommentNode_t *a;
    
    /**counts num of artists in list,*/
    /*I assume this is faster than calling realloc at every node**/
    for(a = firstAlbumComment; a != NULL; a=a->next) {
	if(a->userOwner == idNumber) size++;
    }

    /*mallocs memory for array and goes back through list, adding id's to array*/
    commentArray = (int*) malloc(sizeof(int)*(size+1));
    if(commentArray == NULL) return NULL;
    
    for(a=firstAlbumComment, size=0; a != NULL; a=a->next){
	if(a->userOwner == idNumber) commentArray[size++] = a->ID;
    }
    
    commentArray[size] = LAST_ID_IN_ARRAY; /*set last field*/
    return commentArray;
}

int *getAlbumCommentsForAlbum(int idNumber) {
    int size = 0;        /**size will be number of artists in list**/
    int *commentArray = 0;    /*ID array to be returned*/
    albumCommentNode_t *a;
    
    /**counts num of artists in list,*/
    /*I assume this is faster than calling realloc at every node**/
    for(a = firstAlbumComment; a != NULL; a=a->next) {
	if(a->userOwner == idNumber) size++;
    }

    /*mallocs memory for array and goes back through list, adding id's to array*/
    commentArray = (int*) malloc(sizeof(int)*(size+1));
    if(commentArray == NULL) return NULL;
    
    for(a=firstAlbumComment, size=0; a != NULL; a=a->next){
	if(a->albumID == idNumber) commentArray[size++] = a->ID;
    }
    
    commentArray[size] = LAST_ID_IN_ARRAY; /*set last field*/
    return commentArray;
}

/*--------------- Artist Comment Functions ----------------------*/

/*returns pointer to comment */
artistCommentNode_t *getArtistComment(int commentID){
    artistCommentNode_t *c; 
    
    /**find comment id in list, assumes unsorted list**/
    for(c = firstArtistComment; c != NULL; c=c->next){
	if(c->ID == commentID){
	    return c;
 	} 
    }
    return NULL;
}

int *getArtistCommentsByUser(int idNumber) {
    int size = 0;        /**size will be number of artists in list**/
    int *commentArray = 0;    /*ID array to be returned*/
    artistCommentNode_t *a;
    
    /**counts num of artists in list,*/
    /*I assume this is faster than calling realloc at every node**/
    for(a = firstArtistComment; a != NULL; a=a->next) {
	if(a->userOwner == idNumber) size++;
    }

    /*mallocs memory for array and goes back through list, adding id's to array*/
    commentArray = (int*) malloc(sizeof(int)*(size+1));
    if(commentArray == NULL) return NULL;
    
    for(a=firstArtistComment, size=0; a != NULL; a=a->next){
	if(a->userOwner == idNumber) commentArray[size++] = a->ID;
    }
    
    commentArray[size] = LAST_ID_IN_ARRAY; /*set last field*/
    return commentArray;
}

int *getArtistCommentsForArtist(int idNumber) {
    int size = 0;        /**size will be number of artists in list**/
    int *commentArray = 0;    /*ID array to be returned*/
    artistCommentNode_t *a;
    
    /**counts num of artists in list,*/
    /*I assume this is faster than calling realloc at every node**/
    for(a = firstArtistComment; a != NULL; a=a->next) {
	if(a->userOwner == idNumber) size++;
    }

    /*mallocs memory for array and goes back through list, adding id's to array*/
    commentArray = (int*) malloc(sizeof(int)*(size+1));
    if(commentArray == NULL) return NULL;
    
    for(a=firstArtistComment, size=0; a != NULL; a=a->next){
	if(a->artistID == idNumber) commentArray[size++] = a->ID;
    }
    
    commentArray[size] = LAST_ID_IN_ARRAY; /*set last field*/
    return commentArray;
}

/*--------------- Loan Functions --------------------------------*/

loanNode_t *getLoan(int idNumber){
    loanNode_t *l; 
    
    /**find artist id in list, assumes unsorted list**/
    for(l = firstLoan; l != NULL; l=l->next){
	if(l->ID == idNumber){
	    return l;
 	} 
    }
    return NULL;
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

int getLoanStatus(int idNumber) {
    loanNode_t *a;
    a = getLoan(idNumber);
    
    if(a != NULL) {
	if(a->isReturned) {
	    return LOAN_INACTIVE;
	}
	return LOAN_ACTIVE;
    }
    return E_NOLOAN;
}

int *getLoansByUser(int idNumber){
    int size = 0;        /**size will be number of artists in list**/
    int *loanArray = 0;    /*ID array to be returned*/
    loanNode_t *a;
    
    /**counts num of artists in list,*/
    /*I assume this is faster than calling realloc at every node**/
    for(a = firstLoan; a != NULL; a=a->next) {
	if(a->userID == idNumber) size++;
    }

    /*mallocs memory for array and goes back through list, adding id's to array*/
    loanArray = (int*) malloc(sizeof(int)*(size+1));
    if(loanArray == NULL) return NULL;
    
    for(a=firstLoan, size=0; a != NULL; a=a->next){
	if(a->userID == idNumber) loanArray[size++] = a->ID;
    }
    
    loanArray[size] = LAST_ID_IN_ARRAY; /*set last field*/
    return loanArray;
}

int *getLoansByAlbum(int idNumber){
    int size = 0;        /**size will be number of artists in list**/
    int *loanArray = 0;    /*ID array to be returned*/
    loanNode_t *a;
    
    /**counts num of artists in list,*/
    /*I assume this is faster than calling realloc at every node**/
    for(a = firstLoan; a != NULL; a=a->next) {
	if(a->albumID == idNumber) size++;
    }

    /*mallocs memory for array and goes back through list, adding id's to array*/
    loanArray = (int*) malloc(sizeof(int)*(size+1));
    if(loanArray == NULL) return NULL;
    
    for(a=firstLoan, size=0; a != NULL; a=a->next){
	if(a->albumID == idNumber) loanArray[size++] = a->ID;
    }
    
    loanArray[size] = LAST_ID_IN_ARRAY; /*set last field*/
    return loanArray;
}


/*******************************************************/
/*******************************************************/
/*******************************************************/
/*******************************************************/
/*******************************************************/
/*******************************************************/
/*******************************************************/
/*******************************************************/
/*******************************************************/
/*******************************************************/
/*******************************************************/
/*******************************************************/
/*******************************************************/
/*******************************************************/
/*******************************************************/
/*******************************************************/
/*******************************************************/
/*******************************************************/
/*******************************************************/
/*******************************************************/
/*******************************************************/
/*******************************************************/
/*******************************************************/
/*******************************************************/


int *getAllLoansByStatus(int isReturned){

    int arraySize = 0; 
    int *loansArray = 0; /*new array to be returned*/ 
    loanNode_t *l;

    /*allocate mem for 1st element in array*/ 
    loansArray = (int*) malloc(sizeof(int));
    if(loansArray == NULL) return NULL;
    
    /*loops through array and reallocates memory when it finds loans,*/ 
    /* that are 'isreturned'*/ 
    for(l = firstLoan; l != NULL; l=l->next){
	if(l->isReturned == isReturned){
	    /*reallocates mem for array and checks for mem error*/ 
	    if(realloc(loansArray, sizeof(int)*(arraySize+2)) == NULL){
		free(loansArray);
		return NULL;
	    }
 	    /*add comment to array and increment size*/ 
	    loansArray[arraySize++] = l->ID;
	}
    }

    /*put in end of array identifier*/
    loansArray[arraySize] = LAST_ID_IN_ARRAY;
    return loansArray;
}

int *getAllCurrentLoans(void){
    return getAllLoansByStatus(FALSE);
}

int *getAllReturnedLoans(void){
    return getAllLoansByStatus(TRUE);
}

int *getLoansByUserCodeAndStatus(int idNumber, int isReturned){

    int arraySize = 0;
    int *loansByUser = 0;
    int *loansByStatus = 0;
    int *both = 0;
    int i;
    int k;

    loansByStatus = getAllLoansByStatus(isReturned);
    loansByUser = getLoansByUser(idNumber);

    /*get memory for first elem in array*/
    both = (int*) malloc(sizeof(int));
    if(both == NULL) return NULL;

    /*loop through both arrays find common loan id's (O(n^2))*/
    for(i=0; loansByUser[i] != LAST_ID_IN_ARRAY; i++){
	for(k=0; loansByStatus[k] != LAST_ID_IN_ARRAY; k++){
	    if(loansByUser[i] == loansByStatus[k]){

		/*get more memory for array*/
		if(realloc(both, sizeof(int)*(arraySize+2)) == NULL){
		    free(loansByUser);
		    free(loansByStatus);
		    free(both);
		    return NULL;
		}
		/*put common ID in array*/
		both[arraySize++] = loansByUser[i];
	    }
	}
    }
    free(loansByUser);
    free(loansByStatus);

    both[arraySize] = LAST_ID_IN_ARRAY;
    return both;    
}