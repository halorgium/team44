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
#include "lib.h"

/*------------------------ structures and variables ------------------*/

#define TRUE 0
#define FALSE 1
typedef int Boolean;

/* structure which holds a user and links next user in list*/
typedef struct userNode{ 
    /*int ID;*/
    char *userName;   /*unique user name*/
    char *emailAddress;
    Boolean isLibrarian;       /* FALSE-standard, TRUE-librarian*/
    /*history (loans) */
    /*comments*/
    struct userNode *next;
}userNode_t;

/*struct to hold an artist*/
typedef struct artistNode{
    int ID;
    char *name;
    /*maybe comments about artist??*/
    struct artistNode *next;
}artistNode_t;

/* structure which holds an album and links to next album in list */
typedef struct albumNode{
    int ID;
    char *title;
    Boolean isAvailable;   /*TRUE-available*/

    /*may have comments about album or loan histroy*/
    
    struct artistNode *artist;
    struct albumNode* next;
}albumNode_t;


/* structure which holds a comment and links to next comment in list*/
typedef struct commentNode{
    int ID;
    /*variables showing which the comment is about*/
    /* others are null*/
    int artistID;
    int albumID;
    char *user;
    
    char *userOwner;
    char *comment;
    struct commentNode *next;
}commentNode_t;

/*struct to hold loans - may not be needed*/
typedef struct loan{
    int ID;
    int albumID;
    char *userName;
    struct loan *next;
}



/*all are  pointers to the first node in the 3 lists*/
static userNode_t *firstUser = NULL;  
static albumNode_t *firstAlbum = NULL;
static commentNode_t *firstComment = NULL;

/*simple ID's which are incremented after addition of new users etc*/
/* these should be changed before final implementation*/
static int nextArtistID = 100;
static int nextAlbumID = 200;        
static int nextCommentID = 300;
static int nextLoanID = 400;      

/*-------------------------- functions ------------------------------------*/

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
int addAlbum(const char* title, const char* artist){
    albumNode_t *newAlbumNode = NULL;
    
    /*checks whether argument pointers are NULL */
    if(title == NULL || artist == NULL ){
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
    
    /*allocates memory for the Album's artist*/
    newAlbumNode->artist = (char*) malloc(sizeof(char) *(strlen(artist)+1));
    if(newAlbumNode->artist == NULL){     /* malloc failure test*/
	free(newAlbumNode->title);       /*free previously allocated memory*/
	free(newAlbumNode);
	return E_MALLOC_FAILED;
    }
    
    /*copy the param strings into the newAlbum, set ID*/
    strcpy(newAlbumNode->title, title); 
    strcpy(newAlbumNode->artist, artist);
    newAlbumNode->ID = nextAlbumID;
    
    newAlbumNode->next = firstAlbum;   /*insert at front of list*/
    firstAlbum = newAlbumNode;
    
    return nextAlbumID++;    /*return id then increment*/
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
int addUser(const char* name, const char* email){
    userNode_t *newUserNode = NULL;
    
    if(name == NULL || email == NULL ){ /* check for NULL params*/
	return E_INVALID_PARAM;
    }
    
    /*allocates memory for the new user*/
    newUserNode = (userNode_t*) malloc(sizeof(userNode_t));
    if(newUserNode == NULL){        /*malloc failure?*/
	return E_MALLOC_FAILED;
    }
    
    /*allocates memory for the newuser's name*/
    newUserNode->name = (char*) malloc(sizeof(char) *(strlen(name)+1));
    if(newUserNode->name == NULL){ /*malloc failure test*/
	free(newUserNode);
	return E_MALLOC_FAILED;
    }
    
    /*allocates memory for the newuser's emailAddress*/
    newUserNode->emailAddress = (char*) malloc(sizeof(char)*(strlen(email)+1));
    if(newUserNode->emailAddress == NULL){ /*pointer is null - malloc failure*/
	free(newUserNode->name);  /*free the memory before returning error*/
	free(newUserNode);
	return E_MALLOC_FAILED;
    }

    /*copy param strings into newUsers fields, sets ID*/
    strcpy(newUserNode->name, name); 
    strcpy(newUserNode->emailAddress, email);
    newUserNode->ID = nextUserID;
    
    newUserNode->next = firstUser;   /*insert at front of list*/
    firstUser = newUserNode;
    
    return nextUserID++;    /*return then increment id*/
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
int addComment(int albumID, int userID, const char* title, const char* body){
    
    commentNode_t *newCommentNode = NULL;
    albumNode_t *a;   /*pointers used to check album and user id's**/
    userNode_t *u;
    
    if(title == NULL || body == NULL ){/**null param  check**/
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
    
    u=firstUser;    /*test to see if user is in the database*/
    if(u==NULL){
	return E_NOUSER;
    }
    /**find user in user list, assumes unsorted**/
    for(; u->ID != userID; u=u->next){
	if(u->next==NULL){
	    return E_NOUSER;
	}
    }
    
    /*allocates memory for newComment*/
    newCommentNode = (commentNode_t*) malloc(sizeof(commentNode_t));
    if(newCommentNode == NULL){        /*malloc failure?*/
	return E_MALLOC_FAILED;
    }
    
    /*allocates memory for newComment's title*/
    newCommentNode->commentTitle = malloc(sizeof(char)*(strlen(title)+1));
    if(newCommentNode->commentTitle == NULL){        /* malloc failure test*/
	free(newCommentNode);
	return E_MALLOC_FAILED;
    }
    
    /*allocates memory for newComment's comment body*/
    newCommentNode->commentBody = malloc(sizeof(char)*(strlen(body)+1));
    if(newCommentNode->commentBody == NULL){ /*malloc failure?*/
	free(newCommentNode->commentTitle);/*free mem before returning error*/
	free(newCommentNode);
	return E_MALLOC_FAILED;
    }

    /*set comment fields and id fields*/
    strcpy(newCommentNode->commentTitle, title);
    strcpy(newCommentNode->commentBody, body);
    newCommentNode->albumID = albumID; 
    newCommentNode->userID = userID;
    newCommentNode->ID = nextCommentID;
    
    /*insert new node at front of list*/
    newCommentNode->next = firstComment;   
    firstComment = newCommentNode;
    
    return nextCommentID++;    /*return and increment id*/
}/* end addComment() */


/* Function: getAlbumTitle
 * Params:  int idNumber
 * Returns: char*.
 *
 * This function retrieves the title of an album with the specified id. 
 * It returns NULL if the album does not exist in the database.
 */
const char* getAlbumTitle(int idNumber){
    albumNode_t *a;
    
    /**find album id in list, assumes unsorted list**/
    for(a = firstAlbum; a != NULL; a=a->next){
	if(a->ID == idNumber){
	    return a->title;
	}
    }
    return NULL;
}/*end getAlbumTitle()*/


/* Function: getAlbumArtist
 * Params:  int idNumber
 * Returns: char*.
 *
 * This function retrieves the artist of an album with the specified id. 
 * It returns NULL if the album does not exist in the database.
 */
const char* getAlbumArtist(int idNumber){
    albumNode_t *a;
    
    /**find album id in list, assumes unsorted list**/
    for(a = firstAlbum; a != NULL; a=a->next){
	if(a->ID == idNumber){
	    return a->artist;
	}
    }
    return NULL;
}/*end getAlbumArtist()*/


/* Function: getUserName
 * Params:  int idNumber
 * Returns: char*.
 *
 * This function retrieves the name of a user with the specified id. 
 * It returns NULL if the user does not exist in the database.
 */
const char* getUserName(int idNumber){
    userNode_t *u;
    
    /**find user id in user list, assumes unsorted list*/
    for(u = firstUser; u != NULL; u=u->next){
	if(u->ID == idNumber){
	    return u->name;
	}
    }
    return NULL;
}/*end getUserName() */


/* Function: getUserEmail
 * Params:  int idNumber
 * Returns: char*.
 *
 * This function retrieves the email address of a user with the specified id. 
 * It returns NULL if the user does not exist in the database.
 */
const char* getUserEmail(int idNumber){
    userNode_t *u;
    
    /**find user id in list,then return the users emailAdress*/
    for(u = firstUser; u != NULL; u=u->next){
	if(u->ID == idNumber){
	    return u->emailAddress;
	}
    }
    return NULL;
}/*end getUserEmail() */


/* Function: getCommentTitle
 * Params:  int commentID
 * Returns: char*.
 *
 * This function retrieves the title of a comment with the specified id. 
 * It returns NULL if the comment does not exist in the database.
 */
const char* getCommentTitle(int commentID){
    commentNode_t *c;
    
    /**find comment id in list, assumes unsorted list**/
    for(c = firstComment; c != NULL; c=c->next){
	if(c->ID == commentID){
	    return c->commentTitle;
	}
    }
    return NULL;
}/*end getCommentTitle()*/


/* Function: getCommentBody
 * Params:  int commentID
 * Returns: char*.
 *
 * This function retrieves the body of a comment with the specified id. 
 * It returns NULL if the comment does not exist in the database.
 */
const char* getCommentBody(int commentID){
    commentNode_t *c;
    
    /**find comment id in list, then return body**/
    for(c = firstComment; c != NULL; c=c->next){
	if(c->ID == commentID){
	    return c->commentBody;
	}
    }
    return NULL;
}/** end getCommentBody()*/


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
int* getAllAlbums(){
    
    int size = 0;        /**size will be number of albums in list**/
    int *albumArray = 0;    /*ID array to be returned*/
    albumNode_t *a;
    
    /**counts num of albums in list,*/
     /*I assume this is faster than calling realloc at every node**/
    for(a = firstAlbum; a != NULL; a=a->next){
	size++;
    }

/*mallocs memory for array and goes back through list, adding id's to array*/
    albumArray = (int*) malloc(sizeof(int)*(size+1));
    if(albumArray == NULL){
	return NULL;   /*malloc failure test*/
    }
    
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
int* getAllUsers(){
    
    int size = 0;    /**number of users in list**/
    int *userArray = 0;  /*array of users to be returned*/
    userNode_t *u;
    
    /**count number of users in list,*/
    /*I assume this is faster than calling realloc at every node**/
    for(u = firstUser; u != NULL; u=u->next){
	size++;
    }

 /*mallocs memory for array and goes back through list, adding id's to array*/
    userArray = (int*) malloc(sizeof(int)*(size+1));
    if(userArray == NULL){
	return NULL;
    }
    
    for(u=firstUser, size=0; u != NULL; u=u->next){
	userArray[size++] = u->ID;
    }
    
    userArray[size] = LAST_ID_IN_ARRAY;
    return userArray;
}/* end getAllUsers()*/


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
int* getAllComments(){
    
    int size = 0; /**counter used to find number of albums in list**/
    int *commentArray = 0;
    commentNode_t *c;
    
    /*counts number of comments in list.*/
    /*I assume this is faster than calling realloc at every node**/
    for(c = firstComment; c != NULL; c=c->next){
	size++;
    }

 /*mallocs memory for array and goes back through list, adding id's to array*/
    commentArray = (int*) malloc(sizeof(int)*(size+1));
    if(commentArray == NULL){
	return NULL;
    }
    
    for(c=firstComment, size=0; c != NULL; c=c->next){
	commentArray[size++] = c->ID;
    }
    
    commentArray[size] = LAST_ID_IN_ARRAY;
    return commentArray;
} /*end getAllComments()*/


/* Function: getCommentsByAlbumID
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
int* getCommentsByAlbumID(int albumID){
    
    int arraySize = 0;
    int *albumCommentsArray = 0;
    commentNode_t *c;

    /*mallocs the first element of array to be returned*/
    albumCommentsArray = (int*) malloc(sizeof(int));
    if(albumCommentsArray == NULL){
	return NULL;
    }
    
    /*loops through array, reallocating memory when finding album comments,*/
      /*adding comments to array**/
    for(c = firstComment; c != NULL; c=c->next){
	if(c->albumID == albumID){
	    /*reallocate memory for array and check for error*/
	    if(realloc(albumCommentsArray, sizeof(int)*(arraySize+2)) == NULL){
		free(albumCommentsArray);
		return NULL;
	    }
	    albumCommentsArray[arraySize++] = c->ID; /*add comment to array*/
	}
    }
    
    albumCommentsArray[arraySize] = LAST_ID_IN_ARRAY;
    return albumCommentsArray;
} /*end getCommentsByAlbumID()*/


/* Function: getCommentsByUserID
 * Params:  int userID
 * Returns: int*.
 *
 * This function returns an array of comment IDs written by a particular
 * user. This array should be freed by the caller function. See the
 * description for getAllComments() for the structure of the array.
 *
 * If the user does not exist, then an array containing
 * only LAST_ID_IN_ARRAY is returned.
 * 
 * if malloc fails then Null is returned.
 */
int* getCommentsByUserID(int userID){
    
    int arraySize = 0;
    int *userCommentsArray = 0; /*new array to be returned*/
    commentNode_t *c;

    /*allocate mem for 1st element in array*/
    userCommentsArray = (int*) malloc(sizeof(int));
    if(userCommentsArray == NULL){
	return NULL;
    }
    
    /*loops through array and reallocates memory when it finds comments,*/
    /* with that userID and adds them to array*/
    for(c = firstComment; c != NULL; c=c->next){
	if(c->userID == userID){
	    /*reallocates mem for array and checks for mem error*/
	    if(realloc(userCommentsArray, sizeof(int)*(arraySize+2)) == NULL){
		free(userCommentsArray);
		return NULL;
	    }
	    /*add comment to array and increment size*/
	    userCommentsArray[arraySize++] = c->ID;
	}
    }
    
    userCommentsArray[arraySize] = LAST_ID_IN_ARRAY;
    return userCommentsArray;
} /*end getCommentByUserID()*/


/* Function: getCommentsByAlbumAndUserID
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
 */
int* getCommentsByAlbumAndUserID(int albumID, int userID){
    
    int arraySize = 0;
    int *UACArray = 0;    /**User and Album Comments array**/
    commentNode_t *c;    /*temporary pointer*/

    /*allocates memory for the first element in the array*/
    UACArray = (int*) malloc(sizeof(int));
    if(UACArray == NULL){
	return NULL;   /*malloc check*/
    }
    
    /**loops through comment list reallocating memory to UACArray if needed*/
    for(c = firstComment; c != NULL; c=c->next){
	/*are ID's in comment!?*/
	if(c->userID == userID && c->albumID == albumID){
	    /*realloc always keeps array 1 bigger than no. of elements*/
	    if(realloc(UACArray, sizeof(int)*(arraySize+2)) == NULL){ 
		free(UACArray);
		return NULL;
	    }
	    UACArray[arraySize++] = c->ID;
	}/*end if*/
    }
    
    UACArray[arraySize] = LAST_ID_IN_ARRAY;
    return UACArray;
} /*end getcommentsByAlbumAndUserID()*/

int main(int arg, char **args){
    return 1;
}



/*-------------------- end musiclib.c ---------------------------------*/
