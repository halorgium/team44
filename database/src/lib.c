/***************************************************************************
 *                                                                         *
 * @lib.c                                                             *
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

#include "../../shared/defines.h"
#include "../../shared/structs.h"
#include "../../shared/lib.h"
#include "globals.h"
#include "save.h"

/*-------------------------- functions ------------------------------------*/



int addArtist(const char *name){}



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
    userNode_t *u = NULL;

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
    
    newUserNode->next = firstUser;   /*insert at front of list*/
    firstUser = newUserNode;

    /*now its in mem, so save to disk*/
    /*may also add history and loans (NULL, NULL)*/
    if(saveUser(name, userCode, email, bool)!=0)return SAVE_FAILURE;
    
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

/*------------------------------------------------______*/

/* Function: getAlbumTitle
 * Params:  int idNumber
 * Returns: char*.
 *
 * This function retrieves the title of an album with the specified id. 
 * It returns NULL if the album does not exist in the database.
 */
/* const char* getAlbumTitle(int idNumber){ */
/*     albumNode_t *a; */
    
    /**find album id in list, assumes unsorted list**/
   /*  for(a = firstAlbum; a != NULL; a=a->next){ */
/* 	if(a->ID == idNumber){ */
/* 	    return a->title; */
/* 	} */
/*     } */
/*     return NULL; */
/* } *//*end getAlbumTitle()*/





/* Function: getUserName
 * Params:  int idNumber
 * Returns: char*.
 *
 * This function retrieves the name of a user with the specified id. 
 * It returns NULL if the user does not exist in the database.
 */
/* const char* getUserName(char *userCode){ */
/*     userNode_t *u; */
    
    /**find user id in user list, assumes unsorted list*/
   /*  for(u = firstUser; u != NULL; u=u->next){ */
/* 	if(strcmp(u->userCode, userCode)==0){ */
/* 	    return u->userName; */
/* 	} */
/*     } */
/*     return NULL; */
/* } *//*end getUserName() */


/* Function: getUserEmail
 * Params:  int idNumber
 * Returns: char*.
 *
 * This function retrieves the email address of a user with the specified id. 
 * It returns NULL if the user does not exist in the database.
 */
/* const char* getUserEmail(char *code){ */
/*     userNode_t *u; */
    
    /**find user id in list,then return the users emailAdress*/
   /*  for(u = firstUser; u != NULL; u=u->next){ */
/* 	if(strcmp(u->userCode, code)==0){ */
/* 	    return u->emailAddress; */
/* 	} */
/*     } */
/*     return NULL; */
/* } *//*end getUserEmail() */





/* Function: getCommentBody
 * Params:  int commentID
 * Returns: char*.
 *
 * This function retrieves the body of a comment with the specified id. 
 * It returns NULL if the comment does not exist in the database.
 */
/*
const char* getCommentBody(int commentID){

    artistCommentNode_t *c;
    userCommentNode_t *u;
    albumCommentNode_t *a;
*/
/**find comment id in artist list, assumes unsorted list**/
/*
    for(c = firstArtistComment; c != NULL; c=c->next){
	if(c->ID == commentID){
	    return c->comment;
	}
    } 
*/
    /**find comment id in user list, assumes unsorted list**/
/*
    for(u = firstUserComment; u != NULL; u=u->next){
	if(u->ID == commentID){
	    return u->comment;
	}
    }
*/
    /**find comment id in album list, assumes unsorted list**/
/*
    for(a = firstAlbumComment; a != NULL; a=a->next){
	if(a->ID == commentID){
	    return a->comment;
	}
    }
    
    return NULL;
    }*/
/** end getCommentBody()*/


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
} /* end getAllAlbums()*/


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
/* char** getAllUsers(){ */
    
   /*  int size = 0; */    /**number of users in list**/
/*     char *userArray[size];*/  /*array of users to be returned*/ 
/*     userNode_t *u; */
    
    /**count number of users in list,*/
    /*I assume this is faster than calling realloc at every node**/
/*     for(u = firstUser; u != NULL; u=u->next){ */
/* 	size++; */
/*     } */

 /*mallocs memory for array and goes back through list, adding id's to array*/
/*     userArray = (char*) malloc(sizeof(char)*(size+1)); */
/*     if(userArray == NULL){ */
/* 	return NULL; */
/*     } */
    
/*     for(u=firstUser, size=0; u != NULL; u=u->next){ */
/* 	strcpy(userArray[size++], u->userCode); */
/*     } */
    
/*     userArray[size] = NULL; */
/*     return userArray; */
/*}*//* end getAllUsers()*/


/* Function: getAllComments
 * Params:  
 * Returns: int*.
 *
 * This function returns an array of comment IDs. This array should be
 * freed by the caller function. The last id in the array is
 * LAST_ID_IN_ARRAY. For example, if there were three comments in the
 * database, with IDs '4', '7', '19', then this function returns
 * this array:
 *
 *  [4,7,19,LAST_ID_IN_ARRAY]
 *
 * If a memory allocation failure occurs then NULL is returned
 */
/* int* getAllComments(){ */
    
/*     int size = 0; *//**counter used to find number of albums in list**/ 
/*     int *commentArray = 0; */
/*     commentNode_t *c; */
   
     /*counts number of comments in list.*/ 
     /*I assume this is faster than calling realloc at every node**/ 
/*     for(c = firstComment; c != NULL; c=c->next){ */
/* 	size++; */
/*     } */

  /*mallocs memory for array and goes back through list, adding id's to array*/ 
/*     commentArray = (int*) malloc(sizeof(int)*(size+1)); */
/*     if(commentArray == NULL){ */
/* 	return NULL; */
/*     } */
    
/*     for(c=firstComment, size=0; c != NULL; c=c->next){ */
/* 	commentArray[size++] = c->ID; */
/*     } */
    
/*     commentArray[size] = LAST_ID_IN_ARRAY; */
/*     return commentArray; */
/* } *//*end getAllComments()*/ 


/*Function: getCommentsByAlbumID
 * Params:  int albumID
 * Returns: int*.
 *
 * This function returns an array of comment IDs that relate to a
 * particular album. This array should be freed by the caller
 * function. See the description for getAllComments() for the structure
 * of the array.
 *
 * If the album does not exist, then an array containing only 
 * LAST_ID_IN_ARRAY is returned.
 * if malloc fails then NULL is returned.
 *
 */
/* int* getCommentsByAlbumID(int albumID){ */
    
/*     int arraySize = 0; */
/*     int *albumCommentsArray = 0; */
/*     commentNode_t *c; */

     /*mallocs the first element of array to be returned*/ 
/*     albumCommentsArray = (int*) malloc(sizeof(int)); */
/*     if(albumCommentsArray == NULL){ */
/* 	return NULL; */
/*     } */
    
     /*loops through array, reallocating memory when finding album comments,*/ 
      /*adding comments to array**/ 
/*     for(c = firstComment; c != NULL; c=c->next){ */
/* 	if(c->albumID == albumID){ */
	    /*reallocate memory for array and check for error*/ 
/* 	    if(realloc(albumCommentsArray, sizeof(int)*(arraySize+2)) == NULL){ */
/* 		free(albumCommentsArray); */
/* 		return NULL; */
/* 	    } */
/* 	    albumCommentsArray[arraySize++] = c->ID; *//*add comment to array*/ 
/* 	} */
/*     } */
    
/*     albumCommentsArray[arraySize] = LAST_ID_IN_ARRAY; */
/*     return albumCommentsArray; */
/* }*/ /*end getCommentsByAlbumID()*/ 


 /* Function: getCommentsByUserID 
  * Params:  int userID
  * Returns: int*.
  *
  * This function returns an array of comment IDs written by a particular
  * user. This array should be freed by the caller function. See the
  * description for getAllComments() for the structure of the array.
  *
 *  If the user does not exist, then an array containing
 *  only LAST_ID_IN_ARRAY is returned.
 *  
 * if  malloc fails then Null is returned.
 */ 
/* int* getCommentsByUserID(int userID){ */
    
/*     int arraySize = 0; */
/*     int *userCommentsArray = 0; *//*new array to be returned*/ 
/*     commentNode_t *c; */

   /*allocate mem for 1st element in array*/ 
/*     userCommentsArray = (int*) malloc(sizeof(int)); */
/*     if(userCommentsArray == NULL){ */
/* 	return NULL; */
/*     } */
    
     /*loops through array and reallocates memory when it finds comments,*/ 
    /* with that userID and adds them to array*/ 
/*     for(c = firstComment; c != NULL; c=c->next){ */
/* 	if(c->userID == userID){ */
	    /*reallocates mem for array and checks for mem error*/ 
/* 	    if(realloc(userCommentsArray, sizeof(int)*(arraySize+2)) == NULL){ */
/* 		free(userCommentsArray); */
/* 		return NULL; */
/* 	    } */
 	    /*add comment to array and increment size*/ 
/* 	    userCommentsArray[arraySize++] = c->ID; */
/* 	} */
/*     } */
    
/*     userCommentsArray[arraySize] = LAST_ID_IN_ARRAY; */
/*     return userCommentsArray; */
/* } *//*end getCommentByUserID()*/ 


/*Function: getCommentsByAlbumAndUserID
 * Params:  int albumID, int userID 
 * Returns: int*. 
 * 
 * This function returns an array of comment IDs that relate to a 
 * particular album and are written by a particular person. This array 
 * should be freed by the caller function. See the description for
 * getAllComments() for the structure of the array. 
 * 
 * If the user or album do not exist, then an array 
 * containing only LAST_ID_IN_ARRAY is returned. 
 * 
 * if malloc fails then NULL is returned.
 *
 */
 
/* int* getCommentsByAlbumAndUserID(int albumID, int userID){ */
    
/*     int arraySize = 0; */
/*     int *UACArray = 0;  */  /**User and Album Comments array**/ 
/*     commentNode_t *c;   */ /*temporary pointer*/

    /*allocates memory for the first element in the array*/ 
/*     UACArray = (int*) malloc(sizeof(int)); */
/*     if(UACArray == NULL){ */
/* 	return NULL;  */ /*malloc check*/ 
/*     } */
    
   /**loops through comment list reallocating memory to UACArray if needed*/ 
/*     for(c = firstComment; c != NULL; c=c->next){ */
	/*are ID's in comment!?*/ 
/* 	if(c->userID == userID && c->albumID == albumID){ */
	    /*realloc always keeps array 1 bigger than no. of elements*/ 
/* 	    if(realloc(UACArray, sizeof(int)*(arraySize+2)) == NULL){  */
/* 		free(UACArray); */
/* 		return NULL; */
/* 	    } */
/* 	    UACArray[arraySize++] = c->ID; */
/* 	}end if*/ 
/*     } */
    
/*     UACArray[arraySize] = LAST_ID_IN_ARRAY; */
/*     return UACArray; */
/* }*/ /*end getcommentsByAlbumAndUserID()*/ 



Boolean isUserInDatabase(char *userCode){
    userNode_t u;
/**/
    u = firstUser;
    for(;u!=NULL; u=u->next){
	if(strcmp(u->userName, userCode)== 0) return TRUE;
    }
    return FALSE;
}

int *getAlbumsByArtistID(int ID){
    
    int arraySize = 0; 
    int *albumsArray = 0; /*new array to be returned*/ 
    albumNode_t *c;

   /*allocate mem for 1st element in array*/ 
    albumsArray = (int*) malloc(sizeof(int));
    if(albumsArray == NULL) return NULL;
    
     /*loops through array and reallocates memory when it finds albums,*/ 
    /* with that artistID and adds them to array*/ 
    for(c = firstAlbum; c != NULL; c=c->next){
	if(c->artistID == ID){
	    /*reallocates mem for array and checks for mem error*/ 
	    if(realloc(albumsArray, sizeof(int)*(arraySize+2)) == NULL){
		free(albumsArray);
		return NULL;
	    }
 	    /*add comment to array and increment size*/ 
	    albumsArray[arraySize++] = c->ID;
	}
    }
    
    albumsArray[arraySize] = LAST_ID_IN_ARRAY;
    return albumsArray;
}

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


int *getAllArtists(void){

    int arraySize = 0; 
    int *artistsArray = 0; /*new array to be returned*/ 
    artistNode_t *c;

   /*allocate mem for 1st element in array*/ 
    artistsArray = (int*) malloc(sizeof(int));
    if(artistsArray == NULL) return NULL;
    
     /*loops through list and reallocates memory for every artist*/ 
    for(c = firstArtist; c != NULL; c=c->next){
	/*reallocates mem for array and checks for mem error*/ 
	if(realloc(artistsArray, sizeof(int)*(arraySize+2)) == NULL){
	    free(albumsArray);
	    return NULL;
	}
	/*add comment to array and increment size*/ 
	albumsArray[arraySize++] = c->ID;
    }
    
    albumsArray[arraySize] = LAST_ID_IN_ARRAY;
    return albumsArray;
}
loanNode_t *getLoan(int loanID){
    loanNode_t *l; 
    
    /**find artist id in list, assumes unsorted list**/
    for(l = firstLoan; l != NULL; l=l->next){
	if(l->ID == loanID){
	    return l;
 	} 
    }
    return NULL;
}
int *getLoansByUserID(int ID){}
int *getLoansByAlbumID(int ID){}
int *getAllCurrentLoans(void){}
int *getAllReturnedLoans(void){}
int *getLoansByUserIDAndStatus(int userID, int isReturned){}
int isAlbumOnLoan(int albumID){}
userCommentNode_t *getUserComment(int commentID){}
int *getUserCommentsByOwnerID(int ID){}
int *getUserCommentsByUserID(int ID){}
artistCommentNode_t *getArtistComment(int commentID){}
int *getArtistCommentsByArtistID(int ID){}
int *getArtistCommentsByOwnerID(int ID){}
albumCommentNode_t *getAlbumComment(int commentID){}
int *getAlbumCommentsByOwnerID(int ID){}
int *getAlbumCommentsByAlbumID(int ID){}





/*-------------------- end musiclib.c ---------------------------------*/
