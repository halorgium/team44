#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../../shared/defines.h"
#include "../../shared/structs.h"
#include "lib.h"
#include "../../shared/read_line.h"
#include "load.h"



/* ALL LOAD operations are performed only at initilisation*/
int  loadDatabase(){

    FILE *albumInFile = fopen(strcat(SOURCE_LOCATION, ALBUMS_FILE_NAME), "r");
    FILE *artistInFile = fopen(strcat(SOURCE_LOCATION, ARTISTS_FILE_NAME), "r");
    FILE *art_comInFile = fopen(strcat(SOURCE_LOCATION, ARTIST_COMMENTS_FILE_NAME), "r");
    FILE *alb_comInFile = fopen(strcat(SOURCE_LOCATION, ALBUM_COMMENTS_FILE_NAME), "r");
    FILE *usr_comInFile = fopen(strcat(SOURCE_LOCATION, USER_COMMENTS_FILE_NAME), "r");
    FILE *loanInFile = fopen(strcat(SOURCE_LOCATION, LOANS_FILE_NAME), "r");
    FILE *userInFile = fopen(strcat(SOURCE_LOCATION, USERS_FILE_NAME), "r");

    if(loadAllUsers(userInFile)!=1) return LOAD_FAILURE;
    if(loadAllArtists(artistInFile)!=1) return LOAD_FAILURE;
    if(loadAllAlbums(albumInFile)!=1) return LOAD_FAILURE;
    if(loadAllLoans(loanInFile)!=1) return LOAD_FAILURE;
    if(loadArtistsComments(art_comInFile)!=1) return LOAD_FAILURE;
    if(loadAlbumComments(alb_comInFile)!=1) return LOAD_FAILURE;
    if(loadUserComments(usr_comInFile)!=1) return LOAD_FAILURE;

    return 1;
}

int loadAllUsers(FILE *file){

   
    char *line = NULL;

    while((line = readLine(file)) != NULL ){

	userNode_t *newUser = NULL;
	char *temp = line;
	char *temp2 = NULL;
	
	newUser = malloc(sizeof(userNode_t));
	if(newUser == NULL) return E_MALLOC_FAILED;
	
	temp2 = strchr(temp, '%');

	/*get userCode from file*/
	newUser->userCode = malloc(sizeof(char)*(strlen(temp)-strlen(temp2))+1);
	if(newUser->userCode == NULL) return E_MALLOC_FAILED;

	strncpy(newUser->userCode, temp, (strlen(temp)-strlen(temp2)));

	temp = temp2 + 1;  /*temp string getting smaller, also skip the '%'*/
	temp2 = strchr(temp, '%');

	/*get user name from file*/
	newUser->userName = malloc(sizeof(char)*(strlen(temp)-strlen(temp2))+1);
	if(newUser->userName == NULL) return E_MALLOC_FAILED;

	strncpy(newUser->userName, temp, (strlen(temp)-strlen(temp2)));

	temp = temp2 + 1;  /*temp string getting smaller*/
	temp2 = strchr(temp, '%');

	/*get isLibrarian Boolean out of file, if char ==1 (49) then true*/
	if(((int) temp[0]) == 49) newUser->isLibrarian = TRUE;
	else newUser->isLibrarian = FALSE;
	
	/*quick check to test file integrity*/
	if((strlen(temp)-strlen(temp2)) != 1 ) return LOAD_FAILURE;

	temp = temp2 + 1;  /*remove '%' char*/
	/*end of line test*/
	if((strchr(temp, '%'))!= NULL)return LOAD_FAILURE;

	/*now get emailaddress from last element in file*/
	newUser->emailAddress = malloc(sizeof(char)*(strlen(temp)+1));
	if(newUser->emailAddress == NULL) return E_MALLOC_FAILED;

	strcpy(newUser->emailAddress, temp);
						     
	newUser->next = firstUser;   /*insert at front of list*/
	firstUser = newUser;

	/*free memory before reiteration*/
	free(line);
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
	newAlbum->ID =  atoi(char2int);
	free(char2int);

	temp = temp2 + 1;  /*temp string getting smaller, also skip the '%'*/
	temp2 = strchr(temp, '%');

	/*get album title from file*/
	newAlbum->title = malloc(sizeof(char)*(strlen(temp)-strlen(temp2))+1);
	if(newAlbum->title == NULL) return E_MALLOC_FAILED;

	strncpy(newAlbum->title, temp, (strlen(temp)-strlen(temp2)));

	temp = temp2 + 1;  /*remove '%' char*/
	/*temp2 = strchr(temp, '%');*/
	
	/*end of line test*/
	if((strchr(temp, '%'))!= NULL)return LOAD_FAILURE;

	/*get artistId out of file*/
	char2int = malloc(sizeof(char)*(strlen(temp)+1));
	if(char2int == NULL) return E_MALLOC_FAILED;

	strcpy(char2int, temp);
	newAlbum->artistID = atoi(char2int);
	free(char2int);
							     
	newAlbum->next = firstAlbum;   /*insert at front of list*/
	firstAlbum = newAlbum;

	/*free memory before reiteration*/
	free(line);
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
	newArtist->ID = atoi(char2int);
	free(char2int);

	temp = temp2 + 1; /*remove '%' char*/
	/*end of line test*/
	if((strchr(temp, '%'))!= NULL)return LOAD_FAILURE;

	/*get artist title from file*/
	newArtist->name = malloc(sizeof(char)*(strlen(temp)+1));
	if(newArtist->name == NULL) return E_MALLOC_FAILED;

	strcpy(newArtist->name, temp);
							     
	newArtist->next = firstArtist;   /*insert at front of list*/
	firstArtist = newArtist;

	/*free memory before reiteration*/
	free(line);
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
	newLoan->ID = atoi(char2int);
	free(char2int);

	temp = temp2 + 1;  /*temp string getting smaller, also skip the '%'*/
	temp2 = strchr(temp, '%');

	/*get id of album out of file as char* */
	char2int = malloc(sizeof(char)*(strlen(temp)-strlen(temp2))+1);
	if(char2int == NULL) return E_MALLOC_FAILED;

	/*make album ID */
	strncpy(char2int, temp, (strlen(temp)-strlen(temp2)));
	newLoan->albumID = atoi(char2int);
	free(char2int);
	
	temp = temp2 + 1;  /*temp string getting smaller, also skip the '%'*/
	temp2 = strchr(temp, '%');

	/*get album title from file*/
	newLoan->userCode = malloc(sizeof(char)*(strlen(temp)-strlen(temp2))+1);
	if(newLoan->userCode == NULL) return E_MALLOC_FAILED;

	strncpy(newLoan->userCode, temp, (strlen(temp)-strlen(temp2)));

	temp = temp2 + 1;  /*temp string getting smaller, also skip the '%'*/
	temp2 = strchr(temp, '%');

	/*get time stamp out of file*/
	char2int = malloc(sizeof(char)*(strlen(temp)-strlen(temp2))+1);
	if(char2int == NULL) return E_MALLOC_FAILED;

	strncpy(char2int, temp, (strlen(temp)-strlen(temp2)));
	newLoan->timeStamp = atoi(char2int);
	free(char2int);
	
	temp = temp2 + 1; /*remove '%' char*/
	/*end of line test*/
	if((strchr(temp, '%'))!= NULL)return LOAD_FAILURE;

	/*get is REturned boolean out  of file*/
	char2int = malloc(sizeof(char)*(strlen(temp)+1));
	if(char2int == NULL) return E_MALLOC_FAILED;

	strcpy(char2int, temp);
	newLoan->isReturned = atoi(char2int);
	free(char2int);
							     
	newLoan->next = firstLoan;   /*insert at front of list*/
	firstLoan = newLoan;
	
    	/*free memory before reiteration*/
	free(line);
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
	newAlbumComment->ID = atoi(char2int);
	free(char2int);

	temp = temp2 + 1;  /*temp string getting smaller, also skip the '%'*/
	temp2 = strchr(temp, '%');

	/*get id of album out of file as char* */
	char2int = malloc(sizeof(char)*(strlen(temp)-strlen(temp2))+1);
	if(char2int == NULL) return E_MALLOC_FAILED;

	/*make album ID */
	strncpy(char2int, temp, (strlen(temp)-strlen(temp2)));
	newAlbumComment->albumID = atoi(char2int);
	free(char2int);
	
	temp = temp2 + 1;  /*temp string getting smaller, also skip the '%'*/
	temp2 = strchr(temp, '%');

	/*get album title from file*/
	newAlbumComment->userOwner = malloc(sizeof(char)*(strlen(temp)-strlen(temp2))+1);
	if(newAlbumComment->userOwner == NULL) return E_MALLOC_FAILED;

	strncpy(newAlbumComment->userOwner, temp, (strlen(temp)-strlen(temp2)));

	temp = temp2 + 1;  /*remove '%' char*/
	/*end of line test*/
	if((strchr(temp, '%'))!= NULL)return LOAD_FAILURE;

	/*now get emailaddress from last element in file*/
	newAlbumComment->comment = malloc(sizeof(char)*(strlen(temp)+1));
	if(newAlbumComment->comment == NULL) return E_MALLOC_FAILED;

	strcpy(newAlbumComment->comment, temp);
						     
	newAlbumComment->next = firstAlbumComment;   /*insert at front of list*/
	firstAlbumComment = newAlbumComment;

	/*free memory before reiteration*/
	free(line);
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
	newArtistComment->ID = atoi(char2int);
	free(char2int);

	temp = temp2 + 1;  /*temp string getting smaller, also skip the '%'*/
	temp2 = strchr(temp, '%');

	/*get id of artist out of file as char* */
	char2int = malloc(sizeof(char)*(strlen(temp)-strlen(temp2))+1);
	if(char2int == NULL) return E_MALLOC_FAILED;

	/*make artist ID */
	strncpy(char2int, temp, (strlen(temp)-strlen(temp2)));
	newArtistComment->artistID = atoi(char2int);
	free(char2int);
	
	temp = temp2 + 1;  /*temp string getting smaller, also skip the '%'*/
	temp2 = strchr(temp, '%');

	/*get comment owner from file*/
	newArtistComment->userOwner = malloc(sizeof(char)*(strlen(temp)-strlen(temp2))+1);
	if(newArtistComment->userOwner == NULL) return E_MALLOC_FAILED;

	strncpy(newArtistComment->userOwner, temp, (strlen(temp)-strlen(temp2)));

	temp = temp2 + 1;  /*remove '%' char*/
	/*end of line test*/
	if((strchr(temp, '%'))!= NULL)return LOAD_FAILURE;

	/*now get comment from last element in file*/
	newArtistComment->comment = malloc(sizeof(char)*(strlen(temp)+1));
	if(newArtistComment->comment == NULL) return E_MALLOC_FAILED;

	strcpy(newArtistComment->comment, temp);
						     
	newArtistComment->next = firstArtistComment;   /*insert at front of list*/
	firstArtistComment = newArtistComment;

	/*free memory before reiteration*/
	free(line);
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
	newUserComment->ID = atoi(char2int);
	free(char2int);

	temp = temp2 + 1;  /*temp string getting smaller, also skip the '%'*/
	temp2 = strchr(temp, '%');

	/*get comment user(not author) from file*/
	newUserComment->user = malloc(sizeof(char)*(strlen(temp)-strlen(temp2))+1);
	if(newUserComment->user == NULL) return E_MALLOC_FAILED;

	strncpy(newUserComment->user, temp, (strlen(temp)-strlen(temp2)));
	
	temp = temp2 + 1;  /*temp string getting smaller, also skip the '%'*/
	temp2 = strchr(temp, '%');

	/*get comment owner from file*/
	newUserComment->userOwner = malloc(sizeof(char)*(strlen(temp)-strlen(temp2))+1);
	if(newUserComment->userOwner == NULL) return E_MALLOC_FAILED;

	strncpy(newUserComment->userOwner, temp, (strlen(temp)-strlen(temp2)));

	temp = temp2 + 1;  /*remove '%' char*/
	/*end of line test*/
	if((strchr(temp, '%'))!= NULL)return LOAD_FAILURE;

	/*now get comment from last element in file*/
	newUserComment->comment = malloc(sizeof(char)*(strlen(temp)+1));
	if(newUserComment->comment == NULL) return E_MALLOC_FAILED;

	strcpy(newUserComment->comment, temp);
						     
	newUserComment->next = firstUserComment;   /*insert at front of list*/
	firstUserComment = newUserComment;

	/*free memory before reiteration*/
	free(line);
    }
    return 1;
}
/*int loadComment(const int ID);*/

/*char **stringLoader(char *string){}*/
