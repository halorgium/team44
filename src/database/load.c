#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../shared/defines.h"
#include "../shared/structs.h"
#include "../shared/read_line.h"
#include "globals.h"

int loadAllUsers(FILE *f);
int loadAllArtists(FILE *f);
int loadAllAlbums(FILE *f);
int loadAllLoans(FILE *f);
int loadAlbumComments(FILE *f);
int loadArtistComments(FILE *f);
int loadUserComments(FILE *f);

Boolean databaseLoaded=FALSE;

/* ALL LOAD operations are performed only at initilisation*/
int loadDatabase(){
    FILE *albumInFile=NULL;
    FILE *artistInFile=NULL;
    FILE *art_comInFile=NULL;
    FILE *alb_comInFile=NULL;
    FILE *usr_comInFile=NULL;
    FILE *loanInFile=NULL;
    FILE *userInFile=NULL;

    if(databaseLoaded) {
	return DB_ALREADY_LOADED;
    }

    fprintf(stderr, "Start Load Users\n");
    userInFile = fopen(SOURCE_LOCATION""USERS_FILE_NAME, "r");
    if(userInFile == NULL) return USER_LOAD_FAILURE;
    if(loadAllUsers(userInFile)!=1) return USER_LOAD_FAILURE;
    
    fprintf(stderr, "Start Load Albums\n");
    albumInFile = fopen(SOURCE_LOCATION""ALBUMS_FILE_NAME, "r");
    if(albumInFile == NULL) return ALBUM_LOAD_FAILURE;
    if(loadAllAlbums(albumInFile)!=1) return ALBUM_LOAD_FAILURE;
    
    fprintf(stderr, "Start Load Artists\n");
    artistInFile = fopen(SOURCE_LOCATION""ARTISTS_FILE_NAME, "r");
    if(artistInFile == NULL) return ARTIST_LOAD_FAILURE;
    if(loadAllArtists(artistInFile)!=1) return ARTIST_LOAD_FAILURE;
    
    fprintf(stderr, "Start Load User Comments\n");
    usr_comInFile = fopen(SOURCE_LOCATION""USER_COMMENTS_FILE_NAME, "r");
    if(usr_comInFile == NULL) return USR_COM_LOAD_FAILURE;
    if(loadUserComments(usr_comInFile)!=1) return USR_COM_LOAD_FAILURE;

    fprintf(stderr, "Start Load Album Comments\n");
    alb_comInFile = fopen(SOURCE_LOCATION""ALBUM_COMMENTS_FILE_NAME, "r");
    if(alb_comInFile == NULL) return ALB_COM_LOAD_FAILURE;
    if(loadAlbumComments(alb_comInFile)!=1) return ALB_COM_LOAD_FAILURE;

    fprintf(stderr, "Start Load Artist Comments\n");
    art_comInFile = fopen(SOURCE_LOCATION""ARTIST_COMMENTS_FILE_NAME, "r");
    if(art_comInFile == NULL) return ART_COM_LOAD_FAILURE;
    if(loadArtistComments(art_comInFile)!=1) return ART_COM_LOAD_FAILURE;
    
    fprintf(stderr, "Start Load Loans\n");
    loanInFile = fopen(SOURCE_LOCATION""LOANS_FILE_NAME, "r");
    if(loanInFile == NULL) return LOAN_LOAD_FAILURE;
    if(loadAllLoans(loanInFile)!=1) return LOAN_LOAD_FAILURE;
    
    return 1;
}

int loadAllUsers(FILE *file){

   
    char *line = NULL;

    while((line = readLine(file)) != NULL ){

	userNode_t *newUser = NULL;
	char *temp = line;
	char *temp2 = NULL;
	char *char2int = NULL;
	
	newUser = malloc(sizeof(userNode_t));
	if(newUser == NULL) return E_MALLOC_FAILED;
	
	temp2 = strchr(temp, '%');

	/*get userCode from file*/
	newUser->userCode = malloc(sizeof(char)*(strlen(temp)-strlen(temp2))+1);
	if(newUser->userCode == NULL) return E_MALLOC_FAILED;

	strncpy(newUser->userCode, temp, (strlen(temp)-strlen(temp2)));
	/*null terminate new string*/
	newUser->userCode[strlen(temp)-strlen(temp2)] = '\0';

	temp = temp2 + 1;  /*temp string getting smaller, also skip the '%'*/
	temp2 = strchr(temp, '%');

	/*get user name from file*/
	newUser->userName = malloc(sizeof(char)*(strlen(temp)-strlen(temp2))+1);
	if(newUser->userName == NULL) return E_MALLOC_FAILED;

	temp = temp2 + 1;  /*temp string getting smaller*/
	temp2 = strchr(temp, '%');

	/*get isLibrarian Boolean out of file, if char ==1 (49) then true*/
	if(temp[0] == '1') newUser->isLibrarian = TRUE;
	else newUser->isLibrarian = FALSE;

	temp = temp2 + 1;  /*temp string getting smaller*/
	temp2 = strchr(temp, '%');

	/*get ID*/
	strncpy(char2int, temp, (strlen(temp)-strlen(temp2)));
	/*null terminate new string*/
	char2int[strlen(temp)-strlen(temp2)] = '\0';
	
	newUser->ID =  atoi(char2int);
	free(char2int);

	temp = temp2 + 1;  /*remove '%' char*/
	/*end of line test*/
	if((strchr(temp, '%'))!= NULL)return DB_LOAD_FAILURE;

	/*now get emailaddress from last element in file*/
	newUser->emailAddress = malloc(sizeof(char)*(strlen(temp)+1));
	if(newUser->emailAddress == NULL) return E_MALLOC_FAILED;

	strcpy(newUser->emailAddress, temp);
						     
	newUser->next = firstUser;   /*insert at front of list*/
	firstUser = newUser;

	/*free memory before reiteration*/
/*	free(line);*/
	line = NULL;
    }
    return 1;
}
/*int loadUser(FILE *file);*/

int loadAllAlbums(FILE *file){

    char *line = NULL;

    while((line = readLine(file)) != NULL ){

	albumNode_t *newAlbum = NULL;
	char *temp = line;
	char *temp2 = NULL;
	char *char2int =NULL;
	
	newAlbum = malloc(sizeof(albumNode_t));
	if(newAlbum == NULL) return E_MALLOC_FAILED;
	
	temp2 = strchr(temp, '%');

	/*get albumCode from file*/
	char2int = malloc(sizeof(char)*(strlen(temp)-strlen(temp2))+1);
	if(char2int == NULL) return E_MALLOC_FAILED;

	strncpy(char2int, temp, (strlen(temp)-strlen(temp2)));
	/*null terminate new string*/
	char2int[strlen(temp)-strlen(temp2)] = '\0';
	
	newAlbum->ID =  atoi(char2int);
	free(char2int);

	temp = temp2 + 1;  /*temp string getting smaller, also skip the '%'*/
	temp2 = strchr(temp, '%');

	/*get album title from file*/
	newAlbum->title = malloc(sizeof(char)*(strlen(temp)-strlen(temp2))+1);
	if(newAlbum->title == NULL) return E_MALLOC_FAILED;

	strncpy(newAlbum->title, temp, (strlen(temp)-strlen(temp2)));
	/*null terminate new string*/
	newAlbum->title[strlen(temp)-strlen(temp2)] = '\0';

	temp = temp2 + 1;  /*remove '%' char*/
	/*temp2 = strchr(temp, '%');*/
	
	/*end of line test*/
	if((strchr(temp, '%'))!= NULL)return DB_LOAD_FAILURE;

	/*get artistId out of file*/
	char2int = malloc(sizeof(char)*(strlen(temp)+1));
	if(char2int == NULL) return E_MALLOC_FAILED;

	strcpy(char2int, temp);
	newAlbum->artistID = atoi(char2int);
	free(char2int);
							     
	newAlbum->next = firstAlbum;   /*insert at front of list*/
	firstAlbum = newAlbum;

	/*free memory before reiteration*/
/*	free(line);*/
    }

    return 1;

}
/*int loadAlbum(const int ID);*/

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

	/*get artistCode from file*/
	char2int = malloc(sizeof(char)*(strlen(temp)-strlen(temp2))+1);
	if(char2int == NULL) return E_MALLOC_FAILED;

	strncpy(char2int, temp, (strlen(temp)-strlen(temp2)));
	/*null terminate new string*/
	char2int[strlen(temp)-strlen(temp2)] = '\0';
	newArtist->ID = atoi(char2int);
	free(char2int);

	temp = temp2 + 1; /*remove '%' char*/
	/*end of line test*/
	if((strchr(temp, '%'))!= NULL)return DB_LOAD_FAILURE;

	/*get artist title from file*/
	newArtist->name = malloc(sizeof(char)*(strlen(temp)+1));
	if(newArtist->name == NULL) return E_MALLOC_FAILED;

	strcpy(newArtist->name, temp);
							     
	newArtist->next = firstArtist;   /*insert at front of list*/
	firstArtist = newArtist;

	/*free memory before reiteration*/
/* 	free(line); */
    }

    return 1;


}
/*int loadArtist(const int ID);*/

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

	/*get id out of file as char* */
	char2int = malloc(sizeof(char)*(strlen(temp)-strlen(temp2))+1);
	if(char2int == NULL) return E_MALLOC_FAILED;

	/*make loan ID */
	strncpy(char2int, temp, (strlen(temp)-strlen(temp2)));
	/*null terminate new string*/
	char2int[strlen(temp)-strlen(temp2)] = '\0';
	newLoan->ID = atoi(char2int);
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
	newLoan->albumID = atoi(char2int);
	free(char2int);
	
	temp = temp2 + 1;  /*temp string getting smaller, also skip the '%'*/
	temp2 = strchr(temp, '%');

	/*get user ID from file*/
	char2int = malloc(sizeof(char)*(strlen(temp)-strlen(temp2))+1);
	if(char2int == NULL) return E_MALLOC_FAILED;

	/*make album ID */
	strncpy(char2int, temp, (strlen(temp)-strlen(temp2)));
	/*null terminate new string*/
	char2int[strlen(temp)-strlen(temp2)] = '\0';
	newLoan->userID = atoi(char2int);
	free(char2int);

	temp = temp2 + 1;  /*temp string getting smaller, also skip the '%'*/
	temp2 = strchr(temp, '%');

	/*get time stamp out of file*/
	char2int = malloc(sizeof(char)*(strlen(temp)-strlen(temp2))+1);
	if(char2int == NULL) return E_MALLOC_FAILED;

	strncpy(char2int, temp, (strlen(temp)-strlen(temp2)));
	/*null terminate new string*/
	char2int[strlen(temp)-strlen(temp2)] = '\0';
	newLoan->timeStampIn = atoi(char2int);
	free(char2int);
	
	temp = temp2 + 1; /*remove '%' char*/
	/*end of line test*/
	if((strchr(temp, '%'))!= NULL)return DB_LOAD_FAILURE;

	/*get is REturned boolean out  of file*/
	char2int = malloc(sizeof(char)*(strlen(temp)+1));
	if(char2int == NULL) return E_MALLOC_FAILED;

	strcpy(char2int, temp);
	newLoan->isReturned = atoi(char2int);
	free(char2int);
							     
	newLoan->next = firstLoan;   /*insert at front of list*/
	firstLoan = newLoan;
	
    	/*free memory before reiteration*/
/* 	free(line); */
    }

    return 1;
}
/*int loadLoan(const int ID);*/


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

/*char **stringLoader(char *string){}*/
