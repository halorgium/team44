#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../shared/defines.h"
#include "../shared/lib.h"
#include "structs.h"
#include "read_line.h"
#include "globals.h"

int loadNextID(FILE *f);
int loadAllUsers(FILE *f);
int loadAllArtists(FILE *f);
int loadAllAlbums(FILE *f);
int loadAllLoans(FILE *f);
int loadAllLoansReturned(FILE *f);

int loadAlbumComments(FILE *f);
int loadArtistComments(FILE *f);
int loadUserComments(FILE *f);

Boolean databaseLoaded=FALSE;

/* ALL LOAD operations are performed only at initilisation*/
int loadDatabase() {
    FILE *InFile=NULL;

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

    fprintf(stderr, "Start Load nextID\n");
    InFile = fopen(DATA_LOCATION"/"NEXTID_FILE_NAME, "r");
    if(InFile == NULL) return NEXTID_LOAD_FAILURE;
    if(loadNextID(InFile)!=1) return NEXTID_LOAD_FAILURE;
    fclose(InFile);
    
    fprintf(stderr, "Start Load Users\n");
    InFile = fopen(DATA_LOCATION"/"USER_FILE_NAME, "r");
    if(InFile == NULL) return USER_LOAD_FAILURE;
    if(loadAllUsers(InFile)!=1) return USER_LOAD_FAILURE;
    fclose(InFile);
    
    fprintf(stderr, "Start Load Albums\n");
    InFile = fopen(DATA_LOCATION"/"ALBUM_FILE_NAME, "r");
    if(InFile == NULL) return ALBUM_LOAD_FAILURE;
    if(loadAllAlbums(InFile)!=1) return ALBUM_LOAD_FAILURE;
    fclose(InFile);

    fprintf(stderr, "Start Load Artists\n");
    InFile = fopen(DATA_LOCATION"/"ARTIST_FILE_NAME, "r");
    if(InFile == NULL) return ARTIST_LOAD_FAILURE;
    if(loadAllArtists(InFile)!=1) return ARTIST_LOAD_FAILURE;
    fclose(InFile);
    
    fprintf(stderr, "Start Load User Comments\n");
    InFile = fopen(DATA_LOCATION"/"USER_COMMENT_FILE_NAME, "r");
    if(InFile == NULL) return USR_COM_LOAD_FAILURE;
    if(loadUserComments(InFile)!=1) return USR_COM_LOAD_FAILURE;
    fclose(InFile);

    fprintf(stderr, "Start Load Album Comments\n");
    InFile = fopen(DATA_LOCATION"/"ALBUM_COMMENT_FILE_NAME, "r");
    if(InFile == NULL) return ALB_COM_LOAD_FAILURE;
    if(loadAlbumComments(InFile)!=1) return ALB_COM_LOAD_FAILURE;
    fclose(InFile);

    fprintf(stderr, "Start Load Artist Comments\n");
    InFile = fopen(DATA_LOCATION"/"ARTIST_COMMENT_FILE_NAME, "r");
    if(InFile == NULL) return ART_COM_LOAD_FAILURE;
    if(loadArtistComments(InFile)!=1) return ART_COM_LOAD_FAILURE;
    fclose(InFile);
    
    fprintf(stderr, "Start Load Loans\n");
    InFile = fopen(DATA_LOCATION"/"LOAN_FILE_NAME, "r");
    if(InFile == NULL) return LOAN_LOAD_FAILURE;
    if(loadAllLoans(InFile)!=1) return LOAN_LOAD_FAILURE;
    fclose(InFile);

    fprintf(stderr, "Start Load Loans Returned\n");
    InFile = fopen(DATA_LOCATION"/"LOANRET_FILE_NAME, "r");
    if(InFile == NULL) return LOANRET_LOAD_FAILURE;
    if(loadAllLoansReturned(InFile)!=1) return LOANRET_LOAD_FAILURE;
    fclose(InFile);

    databaseLoaded=TRUE;
    
    return 1;
}

int loadNextID(FILE *file) {
    char *line = NULL;

    while((line = readLine(file)) != NULL ){

      /* Format to parse (per line) */
      /* [nextAlbumID]%[nextArtistID]%[nextUserCommentID]%
	 [nextAlbumCommentID]%[nextArtistCommentID]%[nextLoanID] */

	char *temp = line;
	char *temp2 = NULL;
	char *char2int = NULL;
	
	temp2 = strchr(temp, '%');

	/* get albumID */
	char2int = malloc(sizeof(char)*(strlen(temp)-strlen(temp2))+1);
	if(char2int == NULL) return E_MALLOC_FAILED;
	strncpy(char2int, temp, (strlen(temp)-strlen(temp2)));
	/*null terminate new string*/
	char2int[strlen(temp)-strlen(temp2)] = '\0';
	
	_nextAlbumID = atoi(char2int);
	free(char2int);

	temp = temp2 + 1;  /*temp string getting smaller*/
	temp2 = strchr(temp, '%');

	/* get artistID */
	char2int = malloc(sizeof(char)*(strlen(temp)-strlen(temp2))+1);
	if(char2int == NULL) return E_MALLOC_FAILED;
	strncpy(char2int, temp, (strlen(temp)-strlen(temp2)));
	/*null terminate new string*/
	char2int[strlen(temp)-strlen(temp2)] = '\0';
	
	_nextArtistID = atoi(char2int);
	free(char2int);

	temp = temp2 + 1;  /*temp string getting smaller*/
	temp2 = strchr(temp, '%');

	/* get userCommentID */
	char2int = malloc(sizeof(char)*(strlen(temp)-strlen(temp2))+1);
	if(char2int == NULL) return E_MALLOC_FAILED;
	strncpy(char2int, temp, (strlen(temp)-strlen(temp2)));
	/*null terminate new string*/
	char2int[strlen(temp)-strlen(temp2)] = '\0';
	
	_nextUserCommentID = atoi(char2int);
	free(char2int);

	temp = temp2 + 1;  /*temp string getting smaller*/
	temp2 = strchr(temp, '%');

	/* get albumCommentID */
	char2int = malloc(sizeof(char)*(strlen(temp)-strlen(temp2))+1);
	if(char2int == NULL) return E_MALLOC_FAILED;
	strncpy(char2int, temp, (strlen(temp)-strlen(temp2)));
	/*null terminate new string*/
	char2int[strlen(temp)-strlen(temp2)] = '\0';
	
	_nextAlbumCommentID = atoi(char2int);
	free(char2int);

	temp = temp2 + 1;  /*temp string getting smaller*/
	temp2 = strchr(temp, '%');

	/* get artistCommentID */
	char2int = malloc(sizeof(char)*(strlen(temp)-strlen(temp2))+1);
	if(char2int == NULL) return E_MALLOC_FAILED;
	strncpy(char2int, temp, (strlen(temp)-strlen(temp2)));
	/*null terminate new string*/
	char2int[strlen(temp)-strlen(temp2)] = '\0';
	
	_nextArtistCommentID = atoi(char2int);
	free(char2int);

	temp = temp2 + 1;  /*temp string getting smaller*/
	temp2 = strchr(temp, '%');

	/* get loanID */
	char2int = malloc(sizeof(char)*(strlen(temp)-strlen(temp2))+1);
	if(char2int == NULL) return E_MALLOC_FAILED;
	strncpy(char2int, temp, (strlen(temp)-strlen(temp2)));
	/*null terminate new string*/
	char2int[strlen(temp)-strlen(temp2)] = '\0';
	
	_nextLoanID = atoi(char2int);
	free(char2int);

	/*end of line test*/
	temp = temp2 + 1;  /*temp string getting smaller*/
	if((strchr(temp, '%'))!= NULL) return DB_LOAD_FAILURE;

	/*free memory before reiteration*/
/*	free(line);*/
	line = NULL;
    }
    return 1;
}

int loadAllUsers(FILE *file){
    char *line = NULL;

    while((line = readLine(file)) != NULL ){

      /* Format to parse */
      /* [ID]%[userCode]%[userName]%[emailAddress]%[isLib] */

	userNode_t *newUser = NULL;
	char *temp = line;
	char *temp2 = NULL;
	char *char2int = NULL;
	
	newUser = malloc(sizeof(userNode_t));
	if(newUser == NULL) return DB_LOAD_FAILURE;
	
	temp2 = strchr(temp, '%');

	/* get ID */
	char2int = malloc(sizeof(char)*(strlen(temp)-strlen(temp2))+1);
	if(char2int == NULL) return E_MALLOC_FAILED;
	strncpy(char2int, temp, (strlen(temp)-strlen(temp2)));
	/*null terminate new string*/
	char2int[strlen(temp)-strlen(temp2)] = '\0';
	
	newUser->ID = atoi(char2int);
	free(char2int);

	temp = temp2 + 1;  /*temp string getting smaller, also skip the '%'*/
	temp2 = strchr(temp, '%');
	if(temp2 == NULL) return DB_LOAD_FAILURE;

	/* get userCode */
	newUser->userCode = malloc(sizeof(char)*(strlen(temp)-strlen(temp2))+1);
	if(newUser->userCode == NULL) return E_MALLOC_FAILED;
	strncpy(newUser->userCode, temp, (strlen(temp)-strlen(temp2)));
	/*null terminate new string*/
	newUser->userCode[strlen(temp)-strlen(temp2)] = '\0';

	temp = temp2 + 1;  /*temp string getting smaller, also skip the '%'*/
	temp2 = strchr(temp, '%');
	if(temp2 == NULL) return DB_LOAD_FAILURE;

	/*get userName */
	newUser->userName = malloc(sizeof(char)*(strlen(temp)-strlen(temp2))+1);
	if(newUser->userName == NULL) return E_MALLOC_FAILED;
	strncpy(newUser->userName, temp, (strlen(temp)-strlen(temp2)));
	/*null terminate new string*/
	newUser->userName[strlen(temp)-strlen(temp2)] = '\0';

	temp = temp2 + 1;  /*temp string getting smaller*/
	temp2 = strchr(temp, '%');
	if(temp2 == NULL) return DB_LOAD_FAILURE;

	/* get emailAddress */
	newUser->emailAddress = malloc(sizeof(char)*(strlen(temp)-strlen(temp2))+1);
	if(newUser->emailAddress == NULL) return E_MALLOC_FAILED;
	strncpy(newUser->emailAddress, temp, (strlen(temp)-strlen(temp2)));
	/* null terminate new string */
	newUser->userCode[strlen(temp)-strlen(temp2)] = '\0';
	
	temp = temp2 + 1;  /*temp string getting smaller*/
	temp2 = strchr(temp, '%');
	if(temp2 == NULL) return DB_LOAD_FAILURE;

	/* get isLibrarian [Boolean] */
	/* if char == '1' then true*/
	if(temp[0] == '1') newUser->isLibrarian = TRUE;
	else newUser->isLibrarian = FALSE;

	/*end of line test*/
	temp = temp2 + 1;  /*temp string getting smaller*/
	if((strchr(temp, '%'))!= NULL) return DB_LOAD_FAILURE;

	/* Everything OK */
						     
	newUser->next = firstUser;   /*insert at front of list*/
	firstUser = newUser;

	/*free memory before reiteration*/
/*	free(line);*/
	line = NULL;
    }
    return 1;
}

int loadAllAlbums(FILE *file){
    char *line = NULL;

    while((line = readLine(file)) != NULL ){

      /* Format to parse */
      /* [ID]%[title]%[artistID] */

	albumNode_t *newAlbum = NULL;
	char *temp = line;
	char *temp2 = NULL;
	char *char2int =NULL;
	
	newAlbum = malloc(sizeof(albumNode_t));
	if(newAlbum == NULL) return E_MALLOC_FAILED;
	
	temp2 = strchr(temp, '%');
	if(temp2 == NULL) return DB_LOAD_FAILURE;

	/*get albumID */
	char2int = malloc(sizeof(char)*(strlen(temp)-strlen(temp2))+1);
	if(char2int == NULL) return E_MALLOC_FAILED;

	strncpy(char2int, temp, (strlen(temp)-strlen(temp2)));
	/*null terminate new string*/
	char2int[strlen(temp)-strlen(temp2)] = '\0';
	
	newAlbum->ID = atoi(char2int);
	free(char2int);

	temp = temp2 + 1;  /*temp string getting smaller, also skip the '%'*/
	temp2 = strchr(temp, '%');
	if(temp2 == NULL) return DB_LOAD_FAILURE;

	/*get album title */
	newAlbum->title = malloc(sizeof(char)*(strlen(temp)-strlen(temp2))+1);
	if(newAlbum->title == NULL) return E_MALLOC_FAILED;

	strncpy(newAlbum->title, temp, (strlen(temp)-strlen(temp2)));
	/*null terminate new string*/
	newAlbum->title[strlen(temp)-strlen(temp2)] = '\0';

	temp = temp2 + 1;  /*remove '%' char*/
	temp2 = strchr(temp, '%');
	if(temp2 == NULL) return DB_LOAD_FAILURE;
	
	/*get artistId out of file*/
	char2int = malloc(sizeof(char)*(strlen(temp)+1));
	if(char2int == NULL) return E_MALLOC_FAILED;

	strcpy(char2int, temp);
	newAlbum->artistID = atoi(char2int);
	free(char2int);
	
	/*end of line test*/
	temp = temp2 + 1;  /*remove '%' char*/
	if((strchr(temp, '%'))!= NULL) return DB_LOAD_FAILURE;
						     
	newAlbum->next = firstAlbum;   /*insert at front of list*/
	firstAlbum = newAlbum;

	/*free memory before reiteration*/
/*	free(line);*/
    }

    return 1;

}

int loadAllArtists(FILE *file){

    char *line = NULL;

    while((line = readLine(file)) != NULL ){

	artistNode_t *newArtist = NULL;
	char *temp = line;
	char *temp2 = NULL;
	char *char2int =NULL;
	
	newArtist = malloc(sizeof(artistNode_t));
	if(newArtist == NULL) return E_MALLOC_FAILED;
	
	temp2 = strchr(temp, '%');
	if(temp2 == NULL) return DB_LOAD_FAILURE;

	/*get artistID */
	char2int = malloc(sizeof(char)*(strlen(temp)-strlen(temp2))+1);
	if(char2int == NULL) return E_MALLOC_FAILED;

	strncpy(char2int, temp, (strlen(temp)-strlen(temp2)));
	/*null terminate new string*/
	char2int[strlen(temp)-strlen(temp2)] = '\0';
	
	newArtist->ID = atoi(char2int);
	free(char2int);

	temp = temp2 + 1;  /*temp string getting smaller, also skip the '%'*/
	temp2 = strchr(temp, '%');
	if(temp2 == NULL) return DB_LOAD_FAILURE;

	/*get artistName */
	newArtist->name = malloc(sizeof(char)*(strlen(temp)-strlen(temp2))+1);
	if(newArtist->name == NULL) return E_MALLOC_FAILED;

	strncpy(newArtist->name, temp, (strlen(temp)-strlen(temp2)));
	/*null terminate new string*/
	newArtist->name[strlen(temp)-strlen(temp2)] = '\0';

	/*end of line test*/
	temp = temp2 + 1;  /*remove '%' char*/
	if((strchr(temp, '%'))!= NULL) return DB_LOAD_FAILURE;

	/* Everything OK */
	
	newArtist->next = firstArtist;   /*insert at front of list*/
	firstArtist = newArtist;

	/*free memory before reiteration*/
/* 	free(line); */
    }

    return 1;


}

int loadAllLoans(FILE *file){
    char *line = NULL;

    while((line = readLine(file)) != NULL ){
	loanNode_t *newLoan = NULL;
	char *temp = line;
	char *temp2 = NULL;
	char *char2int =NULL;
	
	newLoan = malloc(sizeof(loanNode_t));
	if(newLoan == NULL) return E_MALLOC_FAILED;

	temp2 = strchr(temp, '%');
	if(temp2 == NULL) return DB_LOAD_FAILURE;

	/*get loanID */
	char2int = malloc(sizeof(char)*(strlen(temp)-strlen(temp2))+1);
	if(char2int == NULL) return E_MALLOC_FAILED;

	strncpy(char2int, temp, (strlen(temp)-strlen(temp2)));
	/*null terminate new string*/
	char2int[strlen(temp)-strlen(temp2)] = '\0';
	
	newLoan->ID = atoi(char2int);
	free(char2int);

	temp = temp2 + 1;  /*temp string getting smaller, also skip the '%'*/
	temp2 = strchr(temp, '%');
	if(temp2 == NULL) return DB_LOAD_FAILURE;

	/*get albumID */
	char2int = malloc(sizeof(char)*(strlen(temp)-strlen(temp2))+1);
	if(char2int == NULL) return E_MALLOC_FAILED;

	strncpy(char2int, temp, (strlen(temp)-strlen(temp2)));
	/*null terminate new string*/
	char2int[strlen(temp)-strlen(temp2)] = '\0';
	
	newLoan->albumID = atoi(char2int);
	free(char2int);

	temp = temp2 + 1;  /*temp string getting smaller, also skip the '%'*/
	temp2 = strchr(temp, '%');
	if(temp2 == NULL) return DB_LOAD_FAILURE;

	/*get userID */
	char2int = malloc(sizeof(char)*(strlen(temp)-strlen(temp2))+1);
	if(char2int == NULL) return E_MALLOC_FAILED;

	strncpy(char2int, temp, (strlen(temp)-strlen(temp2)));
	/*null terminate new string*/
	char2int[strlen(temp)-strlen(temp2)] = '\0';

	newLoan->userID = atoi(char2int);
	free(char2int);

	temp = temp2 + 1;  /*temp string getting smaller, also skip the '%'*/
	temp2 = strchr(temp, '%');
	if(temp2 == NULL) return DB_LOAD_FAILURE;

	/*get timeIn */
	char2int = malloc(sizeof(char)*(strlen(temp)-strlen(temp2))+1);
	if(char2int == NULL) return E_MALLOC_FAILED;

	strncpy(char2int, temp, (strlen(temp)-strlen(temp2)));
	/*null terminate new string*/
	char2int[strlen(temp)-strlen(temp2)] = '\0';
	
	newLoan->timeStampIn = atoi(char2int);
	free(char2int);

	/*end of line test*/
	temp = temp2 + 1;  /*remove '%' char*/
	if((strchr(temp, '%'))!= NULL) return DB_LOAD_FAILURE;

	/* Everything OK */

	newLoan->timeStampOut = -1;
	newLoan->isReturned = FALSE;
	
	newLoan->next = firstLoan;   /*insert at front of list*/
	firstLoan = newLoan;
	
    	/*free memory before reiteration*/
/* 	free(line); */
    }

    return 1;
}

int loadAllLoansReturned(FILE *file){
    char *line = NULL;

    while((line = readLine(file)) != NULL ){
	int tempLoanID=-1;
	int tempTimeOut=-1;
	char *temp = line;
	char *temp2 = NULL;
	char *char2int =NULL;
	
	temp2 = strchr(temp, '%');
	if(temp2 == NULL) return DB_LOAD_FAILURE;

	/*get loanID */
	char2int = malloc(sizeof(char)*(strlen(temp)-strlen(temp2))+1);
	if(char2int == NULL) return E_MALLOC_FAILED;

	strncpy(char2int, temp, (strlen(temp)-strlen(temp2)));
	/*null terminate new string*/
	char2int[strlen(temp)-strlen(temp2)] = '\0';
	
	tempLoanID = atoi(char2int);

	temp = temp2 + 1;  /*temp string getting smaller, also skip the '%'*/
	temp2 = strchr(temp, '%');
	if(temp2 == NULL) return DB_LOAD_FAILURE;

	/*get timeOut */
	char2int = malloc(sizeof(char)*(strlen(temp)-strlen(temp2))+1);
	if(char2int == NULL) return E_MALLOC_FAILED;

	strncpy(char2int, temp, (strlen(temp)-strlen(temp2)));
	/*null terminate new string*/
	char2int[strlen(temp)-strlen(temp2)] = '\0';
	
	tempTimeOut = atoi(char2int);

	/*end of line test*/
	temp = temp2 + 1;  /*remove '%' char*/
	if((strchr(temp, '%'))!= NULL) return DB_LOAD_FAILURE;

	/* Everything OK */
	
	setLoanReturned(tempLoanID, tempTimeOut);
	
    	/*free memory before reiteration*/
/* 	free(line); */
    }

    return 1;
}

int loadAlbumComments(FILE *file){
    char *line = NULL;

    while((line = readLine(file)) != NULL ){

	albumCommentNode_t *newAlbumComment = NULL;
	char *temp = line;
	char *temp2 = NULL;
	char *char2int =NULL;
	
	newAlbumComment = malloc(sizeof(albumCommentNode_t));
	if(newAlbumComment == NULL) return E_MALLOC_FAILED;
	
	temp2 = strchr(temp, '%');

	/*get id out of file as char* */
	char2int = malloc(sizeof(char)*(strlen(temp)-strlen(temp2))+1);
	if(char2int == NULL) return E_MALLOC_FAILED;

	/*make albumComment ID */
	strncpy(char2int, temp, (strlen(temp)-strlen(temp2)));
	/*null terminate new string*/
	char2int[strlen(temp)-strlen(temp2)] = '\0';
	newAlbumComment->ID = atoi(char2int);
	free(char2int);

	temp = temp2 + 1;  /*temp string getting smaller, also skip the '%'*/
	temp2 = strchr(temp, '%');

	/*get id of album out of file as char* */
	char2int = malloc(sizeof(char)*(strlen(temp)-strlen(temp2))+1);
	if(char2int == NULL) return E_MALLOC_FAILED;

	/*make album ID */
	strncpy(char2int, temp, (strlen(temp)-strlen(temp2)));
	/*null terminate new string*/
	char2int[strlen(temp)-strlen(temp2)] = '\0';
	newAlbumComment->albumID = atoi(char2int);
	free(char2int);
	
	temp = temp2 + 1;  /*temp string getting smaller, also skip the '%'*/
	temp2 = strchr(temp, '%');

	/*get owner of comment out of file as char* */
	char2int = malloc(sizeof(char)*(strlen(temp)-strlen(temp2))+1);
	if(char2int == NULL) return E_MALLOC_FAILED;

	/*make user ID */
	strncpy(char2int, temp, (strlen(temp)-strlen(temp2)));
	/*null terminate new string*/
	char2int[strlen(temp)-strlen(temp2)] = '\0';
	newAlbumComment->userOwner = atoi(char2int);
	free(char2int);

	temp = temp2 + 1;  /*remove '%' char*/
	/*end of line test*/
	if((strchr(temp, '%'))!= NULL)return DB_LOAD_FAILURE;

	/*now get emailaddress from last element in file*/
	newAlbumComment->comment = malloc(sizeof(char)*(strlen(temp)+1));
	if(newAlbumComment->comment == NULL) return E_MALLOC_FAILED;

	strcpy(newAlbumComment->comment, temp);
						     
	newAlbumComment->next = firstAlbumComment;   /*insert at front of list*/
	firstAlbumComment = newAlbumComment;

	/*free memory before reiteration*/
/* 	free(line); */
    }
    return 1;

}

int loadArtistComments(FILE *file){

    char *line = NULL;

    while((line = readLine(file)) != NULL ){

	artistCommentNode_t *newArtistComment = NULL;
	char *temp = line;
	char *temp2 = NULL;
	char *char2int =NULL;
	
	newArtistComment = malloc(sizeof(artistCommentNode_t));
	if(newArtistComment == NULL) return E_MALLOC_FAILED;
	
	temp2 = strchr(temp, '%');

	/*get id out of file as char* */
	char2int = malloc(sizeof(char)*(strlen(temp)-strlen(temp2))+1);
	if(char2int == NULL) return E_MALLOC_FAILED;

	/*make artistComment ID */
	strncpy(char2int, temp, (strlen(temp)-strlen(temp2)));
	/*null terminate new string*/
	char2int[strlen(temp)-strlen(temp2)] = '\0';
	newArtistComment->ID = atoi(char2int);
	free(char2int);

	temp = temp2 + 1;  /*temp string getting smaller, also skip the '%'*/
	temp2 = strchr(temp, '%');

	/*get id of artist out of file as char* */
	char2int = malloc(sizeof(char)*(strlen(temp)-strlen(temp2))+1);
	if(char2int == NULL) return E_MALLOC_FAILED;

	/*make artist ID */
	strncpy(char2int, temp, (strlen(temp)-strlen(temp2)));
	/*null terminate new string*/
	char2int[strlen(temp)-strlen(temp2)] = '\0';
	newArtistComment->artistID = atoi(char2int);
	free(char2int);
	
	temp = temp2 + 1;  /*temp string getting smaller, also skip the '%'*/
	temp2 = strchr(temp, '%');

	/*get owner of comment out of file as char* */
	char2int = malloc(sizeof(char)*(strlen(temp)-strlen(temp2))+1);
	if(char2int == NULL) return E_MALLOC_FAILED;

	/*make user ID */
	strncpy(char2int, temp, (strlen(temp)-strlen(temp2)));
	/*null terminate new string*/
	char2int[strlen(temp)-strlen(temp2)] = '\0';
	newArtistComment->userOwner = atoi(char2int);
	free(char2int);

	temp = temp2 + 1;  /*remove '%' char*/
	/*end of line test*/
	if((strchr(temp, '%'))!= NULL)return DB_LOAD_FAILURE;

	/*now get comment from last element in file*/
	newArtistComment->comment = malloc(sizeof(char)*(strlen(temp)+1));
	if(newArtistComment->comment == NULL) return E_MALLOC_FAILED;

	strcpy(newArtistComment->comment, temp);
						     
	newArtistComment->next = firstArtistComment;   /*insert at front of list*/
	firstArtistComment = newArtistComment;

	/*free memory before reiteration*/
/* 	free(line); */
    }
    return 1;
}

int loadUserComments(FILE *file){

    char *line = NULL;

    while((line = readLine(file)) != NULL ){

	userCommentNode_t *newUserComment = NULL;
	char *temp = line;
	char *temp2 = NULL;
	char *char2int =NULL;
	
	newUserComment = malloc(sizeof(userCommentNode_t));
	if(newUserComment == NULL) return E_MALLOC_FAILED;
	
	temp2 = strchr(temp, '%');

	/*get id out of file as char* */
	char2int = malloc(sizeof(char)*(strlen(temp)-strlen(temp2))+1);
	if(char2int == NULL) return E_MALLOC_FAILED;

	/*make userComment ID */
	strncpy(char2int, temp, (strlen(temp)-strlen(temp2)));
	/*null terminate new string*/
	char2int[strlen(temp)-strlen(temp2)] = '\0';
	newUserComment->ID = atoi(char2int);
	free(char2int);

	temp = temp2 + 1;  /*temp string getting smaller, also skip the '%'*/
	temp2 = strchr(temp, '%');

	/*get comment user(not author) from file*/
	char2int = malloc(sizeof(char)*(strlen(temp)-strlen(temp2))+1);
	if(char2int == NULL) return E_MALLOC_FAILED;

	/*make user ID */
	strncpy(char2int, temp, (strlen(temp)-strlen(temp2)));
	/*null terminate new string*/
	char2int[strlen(temp)-strlen(temp2)] = '\0';
	newUserComment->userID = atoi(char2int);
	free(char2int);
	
	temp = temp2 + 1;  /*temp string getting smaller, also skip the '%'*/
	temp2 = strchr(temp, '%');

	/*get owner of comment out of file as char* */
	char2int = malloc(sizeof(char)*(strlen(temp)-strlen(temp2))+1);
	if(char2int == NULL) return E_MALLOC_FAILED;

	/*make user ID */
	strncpy(char2int, temp, (strlen(temp)-strlen(temp2)));
	/*null terminate new string*/
	char2int[strlen(temp)-strlen(temp2)] = '\0';
	newUserComment->userOwner = atoi(char2int);
	free(char2int);
	
	temp = temp2 + 1;  /*remove '%' char*/
	/*end of line test*/
	if((strchr(temp, '%'))!= NULL)return DB_LOAD_FAILURE;

	/*now get comment from last element in file*/
	newUserComment->comment = malloc(sizeof(char)*(strlen(temp)+1));
	if(newUserComment->comment == NULL) return E_MALLOC_FAILED;

	strcpy(newUserComment->comment, temp);
						     
	newUserComment->next = firstUserComment;   /*insert at front of list*/
	firstUserComment = newUserComment;

	/*free memory before reiteration*/
/* 	free(line); */
    }
    return 1;
}
