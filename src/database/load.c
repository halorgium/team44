#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../shared/defines.h"
#include "../shared/lib.h"
#include "structs.h"
#include "read_line.h"
#include "globals.h"

static int loadNextID(FILE *f);
static int loadAllUsers(FILE *f);
static int loadAllArtists(FILE *f);
static int loadAllAlbums(FILE *f);
static int loadAllLoans(FILE *f);
static int loadAllLoansReturned(FILE *f);
static int loadAlbumComments(FILE *f);
static int loadArtistComments(FILE *f);
static int loadUserComments(FILE *f);

Boolean databaseLoaded = FALSE;

/* ALL LOAD operations are performed only at initilisation*/
int loadDatabase() {
    FILE *InFile = NULL;

    if(databaseLoaded) {
	return DB_ALREADY_LOADED;
    }

    /* Set first variables */
    firstUser = NULL;
    firstAlbum = NULL;
    firstArtist = NULL;
    firstUserComment = NULL;
    firstAlbumComment = NULL;
    firstArtistComment = NULL;
    firstLoan = NULL;

    /* Start Load nextID */
    InFile = fopen(DATA_LOCATION"/"NEXTID_FILE_NAME, "r");
    if(InFile == NULL) return NEXTID_LOAD_FAILURE;
    if(loadNextID(InFile)!=1) return NEXTID_LOAD_FAILURE;
    fclose(InFile);
    
    /* Start Load Users */
    InFile = fopen(DATA_LOCATION"/"USER_FILE_NAME, "r");
    if(InFile == NULL) return USER_LOAD_FAILURE;
    if(loadAllUsers(InFile)!=1) return USER_LOAD_FAILURE;
    fclose(InFile);
    
    /* Start Load Albums */
    InFile = fopen(DATA_LOCATION"/"ALBUM_FILE_NAME, "r");
    if(InFile == NULL) return ALBUM_LOAD_FAILURE;
    if(loadAllAlbums(InFile)!=1) return ALBUM_LOAD_FAILURE;
    fclose(InFile);

    /* Start Load Artists */
    InFile = fopen(DATA_LOCATION"/"ARTIST_FILE_NAME, "r");
    if(InFile == NULL) return ARTIST_LOAD_FAILURE;
    if(loadAllArtists(InFile)!=1) return ARTIST_LOAD_FAILURE;
    fclose(InFile);
    
    /* Start Load User Comments */
    InFile = fopen(DATA_LOCATION"/"USER_COMMENT_FILE_NAME, "r");
    if(InFile == NULL) return USR_COM_LOAD_FAILURE;
    if(loadUserComments(InFile)!=1) return USR_COM_LOAD_FAILURE;
    fclose(InFile);

    /* Start Load Album Comments */
    InFile = fopen(DATA_LOCATION"/"ALBUM_COMMENT_FILE_NAME, "r");
    if(InFile == NULL) return ALB_COM_LOAD_FAILURE;
    if(loadAlbumComments(InFile)!=1) return ALB_COM_LOAD_FAILURE;
    fclose(InFile);

    /* Start Load Artist Comments */
    InFile = fopen(DATA_LOCATION"/"ARTIST_COMMENT_FILE_NAME, "r");
    if(InFile == NULL) return ART_COM_LOAD_FAILURE;
    if(loadArtistComments(InFile)!=1) return ART_COM_LOAD_FAILURE;
    fclose(InFile);
    
    /* Start Load Loans */
    InFile = fopen(DATA_LOCATION"/"LOAN_FILE_NAME, "r");
    if(InFile == NULL) return LOAN_LOAD_FAILURE;
    if(loadAllLoans(InFile)!=1) return LOAN_LOAD_FAILURE;
    fclose(InFile);

    /* Start Load Loans Returned */
    InFile = fopen(DATA_LOCATION"/"LOANRET_FILE_NAME, "r");
    if(InFile == NULL) return LOANRET_LOAD_FAILURE;
    if(loadAllLoansReturned(InFile)!=1) return LOANRET_LOAD_FAILURE;
    fclose(InFile);

    databaseLoaded=TRUE;
    
    return 1;
}

static int loadNextID(FILE *file) {
    char *line = NULL;

    while((line = readLine(file)) != NULL ){

	/* Format to parse (per line) */
	/* [nextAlbumID]%[nextArtistID]%[nextUserCommentID]%
	   [nextAlbumCommentID]%[nextArtistCommentID]%[nextLoanID]% */

	char *temp = line;
	char *temp2 = NULL;
	char *char2int = NULL;
	
	temp2 = strchr(temp, '%');
	if(temp2 == NULL) {
	    free(line);
	    return DB_LOAD_FAILURE;
	}
	
	/* get albumID */
	char2int = malloc(sizeof(char)*(strlen(temp)-strlen(temp2))+1);
	if(char2int == NULL) {
	    free(line);
	    return E_MALLOC_FAILED;
	}

	strncpy(char2int, temp, (strlen(temp)-strlen(temp2)));
	/*null terminate new string*/
	char2int[strlen(temp)-strlen(temp2)] = '\0';
	
	_nextAlbumID = atoi(char2int);
	free(char2int);

	temp = temp2 + 1;  /*temp string getting smaller*/
	temp2 = strchr(temp, '%');
	if(temp2 == NULL) {
	    free(line);
	    return DB_LOAD_FAILURE;
	}

	/* get artistID */
	char2int = malloc(sizeof(char)*(strlen(temp)-strlen(temp2))+1);
	if(char2int == NULL) {
	    free(line);
	    return E_MALLOC_FAILED;
	}

	strncpy(char2int, temp, (strlen(temp)-strlen(temp2)));
	/*null terminate new string*/
	char2int[strlen(temp)-strlen(temp2)] = '\0';
	
	_nextArtistID = atoi(char2int);
	free(char2int);

	temp = temp2 + 1;  /*temp string getting smaller*/
	temp2 = strchr(temp, '%');
	if(temp2 == NULL) {
	    free(line);
	    return DB_LOAD_FAILURE;
	}

	/* get userCommentID */
	char2int = malloc(sizeof(char)*(strlen(temp)-strlen(temp2))+1);
	if(char2int == NULL) {
	    free(line);
	    return E_MALLOC_FAILED;
	}

	strncpy(char2int, temp, (strlen(temp)-strlen(temp2)));
	/*null terminate new string*/
	char2int[strlen(temp)-strlen(temp2)] = '\0';
	
	_nextUserCommentID = atoi(char2int);
	free(char2int);

	temp = temp2 + 1;  /*temp string getting smaller*/
	temp2 = strchr(temp, '%');
	if(temp2 == NULL) {
	    free(line);
	    return DB_LOAD_FAILURE;
	}

	/* get albumCommentID */
	char2int = malloc(sizeof(char)*(strlen(temp)-strlen(temp2))+1);
	if(char2int == NULL) {
	    free(line);
	    return E_MALLOC_FAILED;
	}

	strncpy(char2int, temp, (strlen(temp)-strlen(temp2)));
	/*null terminate new string*/
	char2int[strlen(temp)-strlen(temp2)] = '\0';
	
	_nextAlbumCommentID = atoi(char2int);
	free(char2int);

	temp = temp2 + 1;  /*temp string getting smaller*/
	temp2 = strchr(temp, '%');
	if(temp2 == NULL) {
	    free(line);
	    return DB_LOAD_FAILURE;
	}

	/* get artistCommentID */
	char2int = malloc(sizeof(char)*(strlen(temp)-strlen(temp2))+1);
	if(char2int == NULL) {
	    free(line);
	    return E_MALLOC_FAILED;
	}

	strncpy(char2int, temp, (strlen(temp)-strlen(temp2)));
	/*null terminate new string*/
	char2int[strlen(temp)-strlen(temp2)] = '\0';
	
	_nextArtistCommentID = atoi(char2int);
	free(char2int);

	temp = temp2 + 1;  /*temp string getting smaller*/
	temp2 = strchr(temp, '%');
	if(temp2 == NULL) {
	    free(line);
	    return DB_LOAD_FAILURE;
	}

	/* get loanID */
	char2int = malloc(sizeof(char)*(strlen(temp)-strlen(temp2))+1);
	if(char2int == NULL) {
	    free(line);
	    return E_MALLOC_FAILED;
	}

	strncpy(char2int, temp, (strlen(temp)-strlen(temp2)));
	/*null terminate new string*/
	char2int[strlen(temp)-strlen(temp2)] = '\0';
	
	_nextLoanID = atoi(char2int);
	free(char2int);

	/*end of line test*/
	temp = temp2 + 1;  /*temp string getting smaller*/
	temp2 = strchr(temp, '%');
	if(temp2 == NULL) {
	    free(line);
	    return DB_LOAD_FAILURE;
	}

	/*free memory before reiteration*/
	free(line);
    }
    return 1;
}

static int loadAllUsers(FILE *file){
    char *line = NULL;

    while((line = readLine(file)) != NULL ){

	/* Format to parse */
	/* [ID]%[userCode]%[userName]%[emailAddress]%[isLib]% */

	userNode_t *newUser = NULL;
	char *temp = line;
	char *temp2 = NULL;
	char *char2int = NULL;
	
	newUser = malloc(sizeof(userNode_t));
	if(newUser == NULL) {
	    free(line);
	    return E_MALLOC_FAILED;
	}
	
	temp2 = strchr(temp, '%');
	if(temp2 == NULL) {
	    free(line);
	    free(newUser);
	    return DB_LOAD_FAILURE;
	}
	
	/* get ID */
	char2int = malloc(sizeof(char)*(strlen(temp)-strlen(temp2))+1);
	if(char2int == NULL) {
	    free(line);
	    free(newUser);
	    return E_MALLOC_FAILED;
	}

	strncpy(char2int, temp, (strlen(temp)-strlen(temp2)));
	/*null terminate new string*/
	char2int[strlen(temp)-strlen(temp2)] = '\0';
	
	newUser->ID = atoi(char2int);
	free(char2int);

	temp = temp2 + 1;  /*temp string getting smaller, also skip the '%'*/
	temp2 = strchr(temp, '%');
	if(temp2 == NULL) {
	    free(line);
	    free(newUser);
	    return DB_LOAD_FAILURE;
	}

	/* get userCode */
	newUser->userCode = malloc(sizeof(char)*(strlen(temp)-strlen(temp2))+1);
	if(newUser->userCode == NULL) {
	    free(line);
	    free(newUser);
	    return E_MALLOC_FAILED;
	}

	strncpy(newUser->userCode, temp, (strlen(temp)-strlen(temp2)));
	/*null terminate new string*/
	newUser->userCode[strlen(temp)-strlen(temp2)] = '\0';

	temp = temp2 + 1;  /*temp string getting smaller, also skip the '%'*/
	temp2 = strchr(temp, '%');
	if(temp2 == NULL) {
	    free(line);
	    free(newUser->userCode);
	    free(newUser);
	    return DB_LOAD_FAILURE;
	}

	/*get userName */
	newUser->userName = malloc(sizeof(char)*(strlen(temp)-strlen(temp2))+1);
	if(newUser->userName == NULL) {
	    free(line);
	    free(newUser->userCode);
	    free(newUser);
	    return E_MALLOC_FAILED;
	}

	strncpy(newUser->userName, temp, (strlen(temp)-strlen(temp2)));
	/*null terminate new string*/
	newUser->userName[strlen(temp)-strlen(temp2)] = '\0';

	temp = temp2 + 1;  /*temp string getting smaller*/
	temp2 = strchr(temp, '%');
	if(temp2 == NULL) {
	    free(line);
	    free(newUser->userName);
	    free(newUser->userCode);
	    free(newUser);
	    return DB_LOAD_FAILURE;
	}

	/* get emailAddress */
	newUser->emailAddress = malloc(sizeof(char)*(strlen(temp)-strlen(temp2))+1);
	if(newUser->emailAddress == NULL) {
	    free(line);
	    free(newUser->userName);
	    free(newUser->userCode);
	    free(newUser);
	    return E_MALLOC_FAILED;
	}

	strncpy(newUser->emailAddress, temp, (strlen(temp)-strlen(temp2)));
	/* null terminate new string */
	newUser->emailAddress[strlen(temp)-strlen(temp2)] = '\0';
	
	temp = temp2 + 1;  /*temp string getting smaller*/
	temp2 = strchr(temp, '%');
	if(temp2 == NULL) {
	    free(line);
	    free(newUser->emailAddress);
	    free(newUser->userName);
	    free(newUser->userCode);
	    free(newUser);
	    return DB_LOAD_FAILURE;
	}

	/* get isLibrarian [Boolean] */
	/* if char == '1' then true*/
	if(temp[0] == '1') {
	    newUser->isLibrarian = TRUE;
	}
	else {
	    newUser->isLibrarian = FALSE;
	}

	/*end of line test*/
	temp = temp2 + 1;  /*temp string getting smaller*/
	temp2 = strchr(temp, '%');
	if(temp2 == NULL) {
	    free(line);
	    free(newUser->emailAddress);
	    free(newUser->userName);
	    free(newUser->userCode);
	    free(newUser);
	    return DB_LOAD_FAILURE;
	}

	/* Everything OK */
						     
	newUser->next = firstUser;   /*insert at front of list*/
	firstUser = newUser;

	/*free memory before reiteration*/
	free(line);
    }
    return 1;
}

static int loadAllAlbums(FILE *file){
    char *line = NULL;

    while((line = readLine(file)) != NULL ){
	
	/* Format to parse */
	/* [ID]%[title]%[artistID]% */

	albumNode_t *newAlbum = NULL;
	char *temp = line;
	char *temp2 = NULL;
	char *char2int =NULL;
	
	newAlbum = malloc(sizeof(albumNode_t));
	if(newAlbum == NULL) {
	    free(line);
	    return E_MALLOC_FAILED;
	}
	
	temp2 = strchr(temp, '%');
	if(temp2 == NULL) {
	    free(line);
	    free(newAlbum);
	    return DB_LOAD_FAILURE;
	}

	/*get albumID */
	char2int = malloc(sizeof(char)*(strlen(temp)-strlen(temp2))+1);
	if(char2int == NULL) {
	    free(line);
	    free(newAlbum);
	    return E_MALLOC_FAILED;
	}

	strncpy(char2int, temp, (strlen(temp)-strlen(temp2)));
	/*null terminate new string*/
	char2int[strlen(temp)-strlen(temp2)] = '\0';
	
	newAlbum->ID = atoi(char2int);
	free(char2int);

	temp = temp2 + 1;  /*temp string getting smaller, also skip the '%'*/
	temp2 = strchr(temp, '%');
	if(temp2 == NULL) {
	    free(line);
	    free(newAlbum);
	    return DB_LOAD_FAILURE;
	}

	/*get album title */
	newAlbum->title = malloc(sizeof(char)*(strlen(temp)-strlen(temp2))+1);
	if(newAlbum->title == NULL) {
	    free(line);
	    free(newAlbum);
	    return E_MALLOC_FAILED;
	}
	
	strncpy(newAlbum->title, temp, (strlen(temp)-strlen(temp2)));
	/*null terminate new string*/
	newAlbum->title[strlen(temp)-strlen(temp2)] = '\0';

	temp = temp2 + 1;  /*remove '%' char*/
	temp2 = strchr(temp, '%');
	if(temp2 == NULL) {
	    free(line);
	    free(newAlbum->title);
    	    free(newAlbum);
	    return DB_LOAD_FAILURE;
	}
	
	/*get artistId out of file*/
	char2int = malloc(sizeof(char)*(strlen(temp)+1));
	if(char2int == NULL) {
	    free(line);
	    free(newAlbum->title);
    	    free(newAlbum);
	    return E_MALLOC_FAILED;
	}

	strcpy(char2int, temp);
	newAlbum->artistID = atoi(char2int);
	free(char2int);
	
	/*end of line test*/
	temp = temp2 + 1;  /*remove '%' char*/
	temp2 = strchr(temp, '%');
	if(temp2 == NULL) {
	    free(line);
	    free(newAlbum->title);
    	    free(newAlbum);
	    return DB_LOAD_FAILURE;
	}
						     
	newAlbum->next = firstAlbum;   /*insert at front of list*/
	firstAlbum = newAlbum;

	/*free memory before reiteration*/
	free(line);
    }

    return 1;
}

static int loadAllArtists(FILE *file){
    char *line = NULL;

    while((line = readLine(file)) != NULL ){

	/* Format to parse */
	/* [ID]%[name]% */
	
	artistNode_t *newArtist = NULL;
	char *temp = line;
	char *temp2 = NULL;
	char *char2int =NULL;
	
	newArtist = malloc(sizeof(artistNode_t));
	if(newArtist == NULL) {
	    free(line);
	    return E_MALLOC_FAILED;
	}
	
	temp2 = strchr(temp, '%');
	if(temp2 == NULL) {
	    free(line);
	    free(newArtist);
	    return DB_LOAD_FAILURE;
	}

	/*get artistID */
	char2int = malloc(sizeof(char)*(strlen(temp)-strlen(temp2))+1);
	if(char2int == NULL) {
	    free(line);
	    free(newArtist);
	    return E_MALLOC_FAILED;
	}

	strncpy(char2int, temp, (strlen(temp)-strlen(temp2)));
	/*null terminate new string*/
	char2int[strlen(temp)-strlen(temp2)] = '\0';
	
	newArtist->ID = atoi(char2int);
	free(char2int);

	temp = temp2 + 1;  /*temp string getting smaller, also skip the '%'*/
	temp2 = strchr(temp, '%');
	if(temp2 == NULL) {
	    free(line);
	    free(newArtist);
	    return DB_LOAD_FAILURE;
	}

	/*get artistName */
	newArtist->name = malloc(sizeof(char)*(strlen(temp)-strlen(temp2))+1);
	if(newArtist->name == NULL) {
	    free(line);
	    free(newArtist);
	    return E_MALLOC_FAILED;
	}

	strncpy(newArtist->name, temp, (strlen(temp)-strlen(temp2)));
	/*null terminate new string*/
	newArtist->name[strlen(temp)-strlen(temp2)] = '\0';

	/*end of line test*/
	temp = temp2 + 1;  /*remove '%' char*/
	temp2 = strchr(temp, '%');
	if(temp2 == NULL) {
	    free(line);
	    free(newArtist->name);
	    free(newArtist);
	    return DB_LOAD_FAILURE;
	}

	/* Everything OK */
	
	newArtist->next = firstArtist;   /*insert at front of list*/
	firstArtist = newArtist;

	/*free memory before reiteration*/
 	free(line); 
    }

    return 1;
}

static int loadUserComments(FILE *file){
    char *line = NULL;

    while((line = readLine(file)) != NULL ){

	/* Format to parse */
	/* [ID]%[userID]%[owner]%[body]% */

	userCommentNode_t *newUserComment = NULL;
	char *temp = line;
	char *temp2 = NULL;
	char *char2int =NULL;
	
	newUserComment = malloc(sizeof(userCommentNode_t));
	if(newUserComment == NULL) {
	    free(line);
	    return E_MALLOC_FAILED;
	}
	
	temp2 = strchr(temp, '%');
	if(temp2 == NULL) {
	    free(line);
	    free(newUserComment);
	    return DB_LOAD_FAILURE;
	}

	/*get userCommentID */
	char2int = malloc(sizeof(char)*(strlen(temp)-strlen(temp2))+1);
	if(char2int == NULL) {
	    free(line);
	    free(newUserComment);
	    return E_MALLOC_FAILED;
	}

	strncpy(char2int, temp, (strlen(temp)-strlen(temp2)));
	/*null terminate new string*/
	char2int[strlen(temp)-strlen(temp2)] = '\0';
	
	newUserComment->ID = atoi(char2int);
	free(char2int);

	temp = temp2 + 1;  /*temp string getting smaller, also skip the '%'*/
	temp2 = strchr(temp, '%');
	if(temp2 == NULL) {
	    free(line);
	    free(newUserComment);
	    return DB_LOAD_FAILURE;
	}

	/*get userID */
	char2int = malloc(sizeof(char)*(strlen(temp)-strlen(temp2))+1);
	if(char2int == NULL) {
	    free(line);
	    free(newUserComment);
	    return E_MALLOC_FAILED;
	}

	strncpy(char2int, temp, (strlen(temp)-strlen(temp2)));
	/*null terminate new string*/
	char2int[strlen(temp)-strlen(temp2)] = '\0';
	
	newUserComment->userID = atoi(char2int);
	free(char2int);

	temp = temp2 + 1;  /*temp string getting smaller, also skip the '%'*/
	temp2 = strchr(temp, '%');
	if(temp2 == NULL) {
	    free(line);
	    free(newUserComment);
	    return DB_LOAD_FAILURE;
	}

	/*get owner */
	char2int = malloc(sizeof(char)*(strlen(temp)-strlen(temp2))+1);
	if(char2int == NULL) {
	    free(line);
	    free(newUserComment);
	    return E_MALLOC_FAILED;
	}

	strncpy(char2int, temp, (strlen(temp)-strlen(temp2)));
	/*null terminate new string*/
	char2int[strlen(temp)-strlen(temp2)] = '\0';
	
	newUserComment->userOwner = atoi(char2int);
	free(char2int);

	temp = temp2 + 1;  /*temp string getting smaller, also skip the '%'*/
	temp2 = strchr(temp, '%');
	if(temp2 == NULL) {
	    free(line);
	    free(newUserComment);
	    return DB_LOAD_FAILURE;
	}

	/*get body */
	newUserComment->comment = malloc(sizeof(char)*(strlen(temp)-strlen(temp2))+1);
	if(newUserComment->comment == NULL) {
	    free(line);
	    free(newUserComment);
	    return E_MALLOC_FAILED;
	}

	strncpy(newUserComment->comment, temp, (strlen(temp)-strlen(temp2)));
	/*null terminate new string*/
	newUserComment->comment[strlen(temp)-strlen(temp2)] = '\0';

	/*end of line test*/
	temp = temp2 + 1;  /*remove '%' char*/
	temp2 = strchr(temp, '%');
	if(temp2 == NULL) {
	    free(line);
	    free(newUserComment->comment);
	    free(newUserComment);
	    return DB_LOAD_FAILURE;
	}

	/* Everything OK */
	
	newUserComment->next = firstUserComment;   /*insert at front of list*/
	firstUserComment = newUserComment;

	/*free memory before reiteration*/
 	free(line);
    }
    return 1;
}

static int loadAlbumComments(FILE *file){
    char *line = NULL;

    while((line = readLine(file)) != NULL ){

	/* Format to parse */
	/* [ID]%[albumID]%[owner]%[body]% */

	albumCommentNode_t *newAlbumComment = NULL;
	char *temp = line;
	char *temp2 = NULL;
	char *char2int =NULL;
	
	newAlbumComment = malloc(sizeof(albumCommentNode_t));
	if(newAlbumComment == NULL) {
	    free(line);
	    return E_MALLOC_FAILED;
	}
	
	temp2 = strchr(temp, '%');
	if(temp2 == NULL) {
	    free(line);
	    free(newAlbumComment);
	    return DB_LOAD_FAILURE;
	}

	/*get albumCommentID */
	char2int = malloc(sizeof(char)*(strlen(temp)-strlen(temp2))+1);
	if(char2int == NULL) {
	    free(line);
	    free(newAlbumComment);
	    return E_MALLOC_FAILED;
	}

	strncpy(char2int, temp, (strlen(temp)-strlen(temp2)));
	/*null terminate new string*/
	char2int[strlen(temp)-strlen(temp2)] = '\0';
	
	newAlbumComment->ID = atoi(char2int);
	free(char2int);

	temp = temp2 + 1;  /*temp string getting smaller, also skip the '%'*/
	temp2 = strchr(temp, '%');
	if(temp2 == NULL) {
	    free(line);
	    free(newAlbumComment);
	    return DB_LOAD_FAILURE;
	}

	/*get albumID */
	char2int = malloc(sizeof(char)*(strlen(temp)-strlen(temp2))+1);
	if(char2int == NULL) {
	    free(line);
	    free(newAlbumComment);
	    return E_MALLOC_FAILED;
	}

	strncpy(char2int, temp, (strlen(temp)-strlen(temp2)));
	/*null terminate new string*/
	char2int[strlen(temp)-strlen(temp2)] = '\0';
	
	newAlbumComment->albumID = atoi(char2int);
	free(char2int);

	temp = temp2 + 1;  /*temp string getting smaller, also skip the '%'*/
	temp2 = strchr(temp, '%');
	if(temp2 == NULL) {
	    free(line);
	    free(newAlbumComment);
	    return DB_LOAD_FAILURE;
	}

	/*get owner */
	char2int = malloc(sizeof(char)*(strlen(temp)-strlen(temp2))+1);
	if(char2int == NULL) {
	    free(line);
	    free(newAlbumComment);
	    return E_MALLOC_FAILED;
	}

	strncpy(char2int, temp, (strlen(temp)-strlen(temp2)));
	/*null terminate new string*/
	char2int[strlen(temp)-strlen(temp2)] = '\0';
	
	newAlbumComment->userOwner = atoi(char2int);
	free(char2int);

	temp = temp2 + 1;  /*temp string getting smaller, also skip the '%'*/
	temp2 = strchr(temp, '%');
	if(temp2 == NULL) {
	    free(line);
	    free(newAlbumComment);
	    return DB_LOAD_FAILURE;
	}

	/*get body */
	newAlbumComment->comment = malloc(sizeof(char)*(strlen(temp)-strlen(temp2))+1);
	if(newAlbumComment->comment == NULL) {
	    free(line);
	    free(newAlbumComment);
	    return E_MALLOC_FAILED;
	}

	strncpy(newAlbumComment->comment, temp, (strlen(temp)-strlen(temp2)));
	/*null terminate new string*/
	newAlbumComment->comment[strlen(temp)-strlen(temp2)] = '\0';

	/*end of line test*/
	temp = temp2 + 1;  /*remove '%' char*/
	temp2 = strchr(temp, '%');
	if(temp2 == NULL) {
	    free(line);
	    free(newAlbumComment);
	    free(newAlbumComment->comment);
	    return DB_LOAD_FAILURE;
	}

	/* Everything OK */
	
	newAlbumComment->next = firstAlbumComment;   /*insert at front of list*/
	firstAlbumComment = newAlbumComment;

	/*free memory before reiteration*/
 	free(line); 
    }
    return 1;
}

static int loadArtistComments(FILE *file){

    char *line = NULL;

    while((line = readLine(file)) != NULL ){

	/* Format to parse */
	/* [ID]%[artistID]%[owner]%[body]% */

	artistCommentNode_t *newArtistComment = NULL;
	char *temp = line;
	char *temp2 = NULL;
	char *char2int =NULL;
	
	newArtistComment = malloc(sizeof(artistCommentNode_t));
	if(newArtistComment == NULL) {
	    free(line);
	    free(newArtistComment);
	    return E_MALLOC_FAILED;
	}
	
	temp2 = strchr(temp, '%');
	if(temp2 == NULL) {
	    free(line);
	    free(newArtistComment);
	    return DB_LOAD_FAILURE;
	}

	/*get artistCommentID */
	char2int = malloc(sizeof(char)*(strlen(temp)-strlen(temp2))+1);
	if(char2int == NULL) {
	    free(line);
	    free(newArtistComment);
	    return E_MALLOC_FAILED;
	}

	strncpy(char2int, temp, (strlen(temp)-strlen(temp2)));
	/*null terminate new string*/
	char2int[strlen(temp)-strlen(temp2)] = '\0';
	
	newArtistComment->ID = atoi(char2int);
	free(char2int);

	temp = temp2 + 1;  /*temp string getting smaller, also skip the '%'*/
	temp2 = strchr(temp, '%');
	if(temp2 == NULL) {
	    free(line);
	    free(newArtistComment);
	    return DB_LOAD_FAILURE;
	}

	/*get artistID */
	char2int = malloc(sizeof(char)*(strlen(temp)-strlen(temp2))+1);
	if(char2int == NULL) {
	    free(line);
	    free(newArtistComment);
	    return E_MALLOC_FAILED;
	}

	strncpy(char2int, temp, (strlen(temp)-strlen(temp2)));
	/*null terminate new string*/
	char2int[strlen(temp)-strlen(temp2)] = '\0';
	
	newArtistComment->artistID = atoi(char2int);
	free(char2int);

	temp = temp2 + 1;  /*temp string getting smaller, also skip the '%'*/
	temp2 = strchr(temp, '%');
	if(temp2 == NULL) {
	    free(line);
	    free(newArtistComment);
	    return DB_LOAD_FAILURE;
	}

	/*get owner */
	char2int = malloc(sizeof(char)*(strlen(temp)-strlen(temp2))+1);
	if(char2int == NULL) {
	    free(line);
	    free(newArtistComment);
	    return E_MALLOC_FAILED;
	}

	strncpy(char2int, temp, (strlen(temp)-strlen(temp2)));
	/*null terminate new string*/
	char2int[strlen(temp)-strlen(temp2)] = '\0';
	
	newArtistComment->userOwner = atoi(char2int);
	free(char2int);

	temp = temp2 + 1;  /*temp string getting smaller, also skip the '%'*/
	temp2 = strchr(temp, '%');
	if(temp2 == NULL) {
	    free(line);
	    free(newArtistComment);
	    return DB_LOAD_FAILURE;
	}

	/*get body */
	newArtistComment->comment = malloc(sizeof(char)*(strlen(temp)-strlen(temp2))+1);
	if(newArtistComment->comment == NULL) {
	    free(line);
	    free(newArtistComment);
	    return E_MALLOC_FAILED;
	}

	strncpy(newArtistComment->comment, temp, (strlen(temp)-strlen(temp2)));
	/*null terminate new string*/
	newArtistComment->comment[strlen(temp)-strlen(temp2)] = '\0';

	/*end of line test*/
	temp = temp2 + 1;  /*remove '%' char*/
	temp2 = strchr(temp, '%');
	if(temp2 == NULL) {
	    free(line);
	    free(newArtistComment);
	    free(newArtistComment->comment);
	    return DB_LOAD_FAILURE;
	}

	/* Everything OK */
						     
	newArtistComment->next = firstArtistComment;   /*insert at front of list*/
	firstArtistComment = newArtistComment;

	/*free memory before reiteration*/
 	free(line); 
    }
    return 1;
}

static int loadAllLoans(FILE *file){
    char *line = NULL;

    while((line = readLine(file)) != NULL) {

	/* Format to parse */
	/* [ID]%[albumID]%[userID]%[timestamp]% */

	loanNode_t *newLoan = NULL;
	char *temp = line;
	char *temp2 = NULL;
	char *char2int =NULL;
	
	newLoan = malloc(sizeof(loanNode_t));
	if(newLoan == NULL) {
	    free(line);
	    return E_MALLOC_FAILED;
	}

	temp2 = strchr(temp, '%');
	if(temp2 == NULL) {
	    free(line);
	    free(newLoan);
	    return DB_LOAD_FAILURE;
	}

	/*get loanID */
	char2int = malloc(sizeof(char)*(strlen(temp)-strlen(temp2))+1);
	if(char2int == NULL) {
	    free(line);
	    free(newLoan);
	    return E_MALLOC_FAILED;
	}

	strncpy(char2int, temp, (strlen(temp)-strlen(temp2)));
	/*null terminate new string*/
	char2int[strlen(temp)-strlen(temp2)] = '\0';
	
	newLoan->ID = atoi(char2int);
	free(char2int);

	temp = temp2 + 1;  /*temp string getting smaller, also skip the '%'*/
	temp2 = strchr(temp, '%');
	if(temp2 == NULL) {
	    free(line);
	    free(newLoan);
	    return DB_LOAD_FAILURE;
	}

	/*get albumID */
	char2int = malloc(sizeof(char)*(strlen(temp)-strlen(temp2))+1);
	if(char2int == NULL) {
	    free(line);
	    free(newLoan);
	    return E_MALLOC_FAILED;
	}

	strncpy(char2int, temp, (strlen(temp)-strlen(temp2)));
	/*null terminate new string*/
	char2int[strlen(temp)-strlen(temp2)] = '\0';
	
	newLoan->albumID = atoi(char2int);
	free(char2int);

	temp = temp2 + 1;  /*temp string getting smaller, also skip the '%'*/
	temp2 = strchr(temp, '%');
	if(temp2 == NULL) {
 	    free(line);
	    free(newLoan);
	    return DB_LOAD_FAILURE;
	}

	/*get userID */
	char2int = malloc(sizeof(char)*(strlen(temp)-strlen(temp2))+1);
	if(char2int == NULL) {
	    free(line);
	    free(newLoan);
	    return E_MALLOC_FAILED;
	}

	strncpy(char2int, temp, (strlen(temp)-strlen(temp2)));
	/*null terminate new string*/
	char2int[strlen(temp)-strlen(temp2)] = '\0';

	newLoan->userID = atoi(char2int);
	free(char2int);

	temp = temp2 + 1;  /*temp string getting smaller, also skip the '%'*/
	temp2 = strchr(temp, '%');
	if(temp2 == NULL) {
	    free(line);
	    free(newLoan);
	    return DB_LOAD_FAILURE;
	}

	/*get timeIn */
	char2int = malloc(sizeof(char)*(strlen(temp)-strlen(temp2))+1);
	if(char2int == NULL) {
	    free(line);
	    free(newLoan);
	    return E_MALLOC_FAILED;
	}

	strncpy(char2int, temp, (strlen(temp)-strlen(temp2)));
	/*null terminate new string*/
	char2int[strlen(temp)-strlen(temp2)] = '\0';
	
	newLoan->timeStampIn = atoi(char2int);
	free(char2int);

	/*end of line test*/
	temp = temp2 + 1;  /*remove '%' char*/
	temp2 = strchr(temp, '%');
	if(temp2 == NULL) {
	    free(line);
	    free(newLoan);
	    return DB_LOAD_FAILURE;
	}

	/* Everything OK */

	newLoan->timeStampOut = -1;
	newLoan->isReturned = FALSE;
	
	newLoan->next = firstLoan;   /*insert at front of list*/
	firstLoan = newLoan;
	
    	/*free memory before reiteration*/
 	free(line); 
    }

    return 1;
}

static int loadAllLoansReturned(FILE *file){
    char *line = NULL;

    while((line = readLine(file)) != NULL ){

	/* Format to parse */
	/* [ID]%[timestamp]% */

	int tempLoanID=-1;
	int tempTimeOut=-1;
	char *temp = line;
	char *temp2 = NULL;
	char *char2int =NULL;
	
	temp2 = strchr(temp, '%');
	if(temp2 == NULL) {
	    free(line);
	    return DB_LOAD_FAILURE;
	}

	/*get loanID */
	char2int = malloc(sizeof(char)*(strlen(temp)-strlen(temp2))+1);
	if(char2int == NULL) {
	    free(line);
	    return E_MALLOC_FAILED;
	}

	strncpy(char2int, temp, (strlen(temp)-strlen(temp2)));
	/*null terminate new string*/
	char2int[strlen(temp)-strlen(temp2)] = '\0';
	
	tempLoanID = atoi(char2int);
        free(char2int);

	temp = temp2 + 1;  /*temp string getting smaller, also skip the '%'*/
	temp2 = strchr(temp, '%');
	if(temp2 == NULL) {
	    free(line);
	    return DB_LOAD_FAILURE;
	}

	/*get timeOut */
	char2int = malloc(sizeof(char)*(strlen(temp)-strlen(temp2))+1);
	if(char2int == NULL) {
	    free(line);
	    return E_MALLOC_FAILED;
	}

	strncpy(char2int, temp, (strlen(temp)-strlen(temp2)));
	/*null terminate new string*/
	char2int[strlen(temp)-strlen(temp2)] = '\0';
	
	tempTimeOut = atoi(char2int);
        free(char2int);

	/*end of line test*/
	temp = temp2 + 1;  /*remove '%' char*/
	temp2 = strchr(temp, '%');
	if(temp2 == NULL) {
	    free(line);
	    return DB_LOAD_FAILURE;
	}

	/* Everything OK */
	
	setLoanReturned(tempLoanID, tempTimeOut);
	
    	/*free memory before reiteration*/
 	free(line); 
    }

    return 1;
}
