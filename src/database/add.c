#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "../shared/defines.h"
#include "../shared/structs.h"
#include "../shared/lib.h"
#include "globals.h"
#include "save.h"

int addLoan(int userID, int albumID){
    
    loanNode_t *newLoanNode = NULL;
    
    /*checks whether argument pointers are NULL */
    if(user == NULL || albumID < 1){
	return E_INVALID_PARAM;
    }
    
    /*allocates memory for new Loan*/
    newLoanNode = (loanNode_t*) malloc(sizeof(loanNode_t));
    if(newLoanNode == NULL){        /*malloc failure test*/
	return E_MALLOC_FAILED;
    }
    
    newLoanNode->userID = userID;
    newLoanNode->albumID = albumID;
    newLoanNode->timeStamp = 99999;  /*change*/
    newLoanNode->isReturned = FALSE;
    
    /* loan is being created for first time - unique id should be created*/
    newLoanNode->ID = nextLoanID;  /*this needs changing*/
    
    newLoanNode->next = firstLoan;   /*insert at front of list*/
    firstLoan = newLoanNode;

    /*created in memory so now needs to be saved*/
    if(saveLoan(newLoanNode->ID, albumID, user, newLoanNode->timeStamp, FALSE) < 0)
	return SAVE_FAILURE;
    
    return nextLoanID++;    /*return id then increment  CHANGE*/    
}


int addArtist(char *name){
    
    artistNode_t *newArtistNode = NULL;
    
    /*checks whether argument pointers are NULL */
    if(name == NULL){
	return E_INVALID_PARAM;
    }
    
    /*allocates memory for new Artist*/
    newArtistNode = (artistNode_t*) malloc(sizeof(artistNode_t));
    if(newArtistNode == NULL){        /*malloc failure test*/
	return E_MALLOC_FAILED;
    }
    
    /*allocates memory for the Artist's title*/
    newArtistNode->name =(char*) malloc(sizeof(char) *(strlen(name)+1));
    if(newArtistNode->name == NULL){    /*pointer is null - malloc failure*/
	free(newArtistNode);
	return E_MALLOC_FAILED;	
    }
    
    
    /*copy the param string into the newArtist*/
    strcpy(newArtistNode->name, name);
    /* artist is being created for first time - unique id should be created*/
    newArtistNode->ID = nextArtistID;  /*this needs changing*/
    
    newArtistNode->next = firstArtist;   /*insert at front of list*/
    firstArtist = newArtistNode;

    /*created in memory so now needs to be saved*/
    if(saveArtist(name, newArtistNode->ID) < 0) return SAVE_FAILURE;
    
    return nextArtistID++;    /*return id then increment  CHANGE*/
}/*end addArtist()*/



/* Function: addAlbum
 * Params:  char* title, char*artist
 * Returns: int.
 *
 * This function adds an album into the database. It  returns
 * E_INVALID_PARAM if either of the parameters are NULL, and 
 * E_MALLOC_FAILED if a memory allocation failed. Otherwise
 * the ID of the new album is returned. IDs always have values greater
 * than 0.
 * Blank album titles and artist are allowed to be added 
 */
int addAlbum(const char *title, const int artistID){
    
    albumNode_t *newAlbumNode = NULL;
    
    /*checks whether argument pointers are NULL */
    if(title == NULL || artistID <0 ){
	return E_INVALID_PARAM;
    }
    
    /*allocates memory for new Album*/
    newAlbumNode = (albumNode_t*) malloc(sizeof(albumNode_t));
    if(newAlbumNode == NULL){        /*malloc failure test*/
	return E_MALLOC_FAILED;
    }
    
    /*allocates memory for the Album's title*/
    newAlbumNode->title =(char*) malloc(sizeof(char) *(strlen(title)+1));
    if(newAlbumNode->title == NULL){    /*pointer is null - malloc failure*/
	free(newAlbumNode);
	return E_MALLOC_FAILED;	
    }
    
    
    /*copy the param strings into the newAlbum, set ID*/
    strcpy(newAlbumNode->title, title);
    newAlbumNode->artistID = artistID;
    /* album is being created for first time - unique id should be created*/
    newAlbumNode->ID = nextAlbumID;  /*this needs changing*/
    
    newAlbumNode->next = firstAlbum;   /*insert at front of list*/
    firstAlbum = newAlbumNode;

    /*created in memory so now needs to be saved*/
    /*an errror id should be created*/
    if(saveAlbum(title, artistID, newAlbumNode->ID)==0) return SAVE_FAILURE;
    
    
    return nextAlbumID++;    /*return id then increment  CHANGE*/
} /*end addAlbum() */


/* Function: addUser
 * Params:  char* name, char* email
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
int addUser(const char* name, const char *userCode, const char* email, const Boolean bool){

    userNode_t *newUserNode = NULL;

    /* check for NULL params*/
    if(name == NULL || email == NULL|| userCode == NULL ){ 
	return E_INVALID_PARAM;
    }

    if(isUserInDatabase(userCode)==TRUE) return ALREADY_ADDED;    
    
    /*allocates memory for the new user*/
    newUserNode = (userNode_t*) malloc(sizeof(userNode_t));
    if(newUserNode == NULL){        /*malloc failure?*/
	return E_MALLOC_FAILED;
    }
    
    /*allocates memory for the newuser's name*/
    newUserNode->userName = (char*) malloc(sizeof(char) *(strlen(name)+1));
    if(newUserNode->userName == NULL){ /*malloc failure test*/
	free(newUserNode);
	return E_MALLOC_FAILED;
    }
    
    /*allocates memory for the newuser's emailAddress*/
    newUserNode->emailAddress = (char*) malloc(sizeof(char)*(strlen(email)+1));
    if(newUserNode->emailAddress == NULL){ /*pointer is null - malloc failure*/
	free(newUserNode->userName);  /*free the memory before returning error*/
	free(newUserNode);
	return E_MALLOC_FAILED;
    }

    /*allocates memory for the newuser's userCOde*/
    newUserNode->userCode = (char*) malloc(sizeof(char)*(strlen(userCode)+1));
    if(newUserNode->userCode == NULL){ /*pointer is null - malloc failure*/
	free(newUserNode->userName);  /*free the memory before returning error*/
	free(newUserNode->userCode);
	free(newUserNode);
	return E_MALLOC_FAILED;
    }

    /*copy param strings into newUsers fields, sets ID*/
    strcpy(newUserNode->userName, name); 
    strcpy(newUserNode->emailAddress, email);
    strcpy(newUserNode->userCode, userCode);
    newUserNode->isLibrarian = bool;
    newUserNode->ID = getUserID(userCode);
    
    newUserNode->next = firstUser;   /*insert at front of list*/
    firstUser = newUserNode;

    /*now its in mem, so save to disk*/
    /*may also add history and loans (NULL, NULL)*/
    if(saveUser(userCode, email, bool, getUserID(userCode))!=0)return SAVE_FAILURE;
    
    return 1;    /*return then increment id*/
} /* end addUser()*/


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



int addCommentArtist(char *owner, char *body, int artistID){
    
    artistCommentNode_t *newCommentNode = NULL;
    artistNode_t *a;   /*pointers used to check album and user id's**/
    userNode_t *u;

    /**null param  check**/
    if( body == NULL || owner == NULL || artistID < 1){
	return E_INVALID_PARAM;

    }

    /*test to see if any artists are in database*/
    a=firstArtist;  
    if(a==NULL){
	return E_NOALBUM;
    }
    
    /**find album id in album list, assumes unsorted list**/
    for(; a->ID != artistID; a=a->next){
	if(a->next==NULL){
	    return E_NOALBUM;
	}
    }
    
    /*test to see if user is in the database*/
    u=firstUser;    
    if(u==NULL){
	return E_NOUSER;
    }
    
    /**find user in user list, assumes unsorted**/
    for(; strcmp(u->userName, owner) != 0; u=u->next){
	if(u->next==NULL){
	    return E_NOUSER;
	}
    }
    
    /*allocates memory for newComment*/
    newCommentNode = (artistCommentNode_t*) malloc(sizeof(artistCommentNode_t));
    if(newCommentNode == NULL){        /*malloc failure?*/
	return E_MALLOC_FAILED;
    }
    
    /*allocates memory for newComment's owner*/
    newCommentNode->userOwner = malloc(sizeof(char)*(strlen(owner)+1));
    if(newCommentNode->userOwner == NULL){        /* malloc failure test*/
	free(newCommentNode);
	return E_MALLOC_FAILED;
    }
    
    /*allocates memory for newComment's comment body*/
    newCommentNode->comment = malloc(sizeof(char)*(strlen(body)+1));
    if(newCommentNode->comment == NULL){ /*malloc failure?*/
	free(newCommentNode->userOwner);/*free mem before returning error*/
	free(newCommentNode);
	return E_MALLOC_FAILED;
    }

    /*set comment fields and id fields*/
    strcpy(newCommentNode->userOwner, owner);
    strcpy(newCommentNode->comment, body);
    newCommentNode->artistID = artistID;
    /*id setting may change*/
    newCommentNode->ID = nextCommentID;
    
    /*insert new node at front of list*/
    newCommentNode->next = firstArtistComment;   
    firstArtistComment = newCommentNode;

    if(saveCommentArtist(owner, body, newCommentNode->ID, artistID)!=0) return SAVE_FAILURE;
    
    return nextCommentID++;    /*return and increment id*/
}/* end addCommentArtist() */


int addCommentAlbum(char *owner, char* body, int albumID){
    
    albumCommentNode_t *newCommentNode = NULL;
    albumNode_t *a;   /*pointers used to check album and user id's**/
    userNode_t *u;
    
    if(owner == NULL || body == NULL || albumID < 1){/**null param  check**/
	return E_INVALID_PARAM;

    }
    a=firstAlbum;  /*test to see if any albums are in database*/
    if(a==NULL){
	return E_NOALBUM;
    }
    /**find album id in album list, assumes unsorted list**/
    for(; a->ID != albumID; a=a->next){
	if(a->next==NULL){
	    return E_NOALBUM;
	}
    }

    /*test to see if user is in the database*/
    u=firstUser;    
    if(u==NULL){
	return E_NOUSER;
    }
    /**find user in user list, assumes unsorted**/
    for(; strcmp(u->userCode, owner)!=0 ; u=u->next){
	if(u->next==NULL){
	    return E_NOUSER;
	}
    }
    
    /*allocates memory for newComment*/
    newCommentNode = (albumCommentNode_t*) malloc(sizeof(albumCommentNode_t));
    if(newCommentNode == NULL){        /*malloc failure?*/
	return E_MALLOC_FAILED;
    }
    
    /*allocates memory for newComment's title*/
    newCommentNode->userOwner = malloc(sizeof(char)*(strlen(owner)+1));
    if(newCommentNode->userOwner == NULL){        /* malloc failure test*/
	free(newCommentNode);
	return E_MALLOC_FAILED;
    }
    
    /*allocates memory for newComment's comment body*/
    newCommentNode->comment = malloc(sizeof(char)*(strlen(body)+1));
    if(newCommentNode->comment == NULL){ /*malloc failure?*/
	free(newCommentNode->userOwner);/*free mem before returning error*/
	free(newCommentNode);
	return E_MALLOC_FAILED;
    }

    /*set comment fields and id fields*/
    strcpy(newCommentNode->userOwner, owner);
    strcpy(newCommentNode->comment, body);
    newCommentNode->albumID = albumID; 

    /*may need changing*/
    newCommentNode->ID = nextCommentID;
    
    /*insert new node at front of list*/
    newCommentNode->next = firstAlbumComment;   
    firstAlbumComment = newCommentNode;

    /*save to disk*/
    if(saveCommentAlbum(owner, body, newCommentNode->ID, albumID)!=0) return SAVE_FAILURE;
    
    return nextCommentID++;    /*return and increment id*/
}/* end addCommentAlbum() */

/*------------------------------------------------*/

int addCommentUser(char *owner, char* body, char *user){
    
    userCommentNode_t *newCommentNode = NULL;
               /*pointers used to check album and user id's**/
    userNode_t *u;
    
    if(owner == NULL || body == NULL || user==NULL){/**null param  check**/
	return E_INVALID_PARAM;

    }
    u=firstUser;  /*test to see if any albums are in database*/
    if(u==NULL){
	return E_NOALBUM;
    }
    /**find owner in  list, assumes unsorted list**/
    for(; strcmp(u->userCode, owner)!=0; u=u->next){
	if(u->next==NULL){
	    return E_NOUSER;
	}
    }
    
    u=firstUser;    /*test to see if user is in the database*/
    if(u==NULL){
	return E_NOUSER;
    }
    /**find user in user list, assumes unsorted**/
    for(; strcmp(u->userCode, user)!=0; u=u->next){
	if(u->next==NULL){
	    return E_NOUSER;
	}
    }
    
    /*allocates memory for newComment*/
    newCommentNode = (userCommentNode_t*) malloc(sizeof(userCommentNode_t));
    if(newCommentNode == NULL){        /*malloc failure?*/
	return E_MALLOC_FAILED;
    }
    
    /*allocates memory for newComment's title*/
    newCommentNode->userOwner = malloc(sizeof(char)*(strlen(owner)+1));
    if(newCommentNode->userOwner == NULL){        /* malloc failure test*/
	free(newCommentNode);
	return E_MALLOC_FAILED;
    }
    
    /*allocates memory for newComment's comment body*/
    newCommentNode->comment = malloc(sizeof(char)*(strlen(body)+1));
    if(newCommentNode->comment == NULL){ /*malloc failure?*/
	free(newCommentNode->userOwner);/*free mem before returning error*/
	free(newCommentNode);
	return E_MALLOC_FAILED;
    }

    /*set comment fields and id fields*/
    strcpy(newCommentNode->userOwner, owner);
    strcpy(newCommentNode->comment, body);

    /*may need changeing*/
    newCommentNode->ID = nextCommentID;
    
    /*insert new node at front of list*/
    newCommentNode->next = firstUserComment;   
    firstUserComment = newCommentNode;

    /*save to disk return error if not worked*/
    if(saveCommentUser(owner, body, newCommentNode->ID, user)!=0) return SAVE_FAILURE;
    
    return nextCommentID++;    /*return and increment id*/
}/* end addCommentUser() */
