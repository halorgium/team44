#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>

#include "../../shared/defines.h"
#include "globals.h"
#include "save.h"

/*functions dealing with saving database to disk*/
/* int saveDatabase(void){} */

/* int  saveAllUsers(){}; */
int saveUser(const char *name, const char* userCode, const char *email, Boolean bool){
    int check;
    char *line = NULL;
    FILE *userFile = fopen(SOURCE_LOCATION""USERS_FILE_NAME, "rw");

    line = malloc(sizeof(char)*(strlen(name)+strlen(userCode)+strlen(email)+1+3)+1);
    if(line == NULL) return E_MALLOC_FAILED;

    sprintf(line, "%s\%%%s\%%%d\%%%s\n", name, userCode, bool, email);

    printf("%s",line);

    check =  saveLine(userFile, line);
    free(line);
    return check;
}

/* int  saveAllAlbums(){}; */
int saveAlbum(const char* title, const int artistID, int ID){
    char *line = NULL;
    int check;
    int artIDLen;
    int IDLen;
    FILE *albumFile = fopen(SOURCE_LOCATION""ALBUMS_FILE_NAME, "rw");

   /* find how many characters the ints are*/
    artIDLen = floor(log10(artistID));
    IDLen = floor(log10(ID));

    /*get memory for the line that will be saved*/
    line = malloc(sizeof(char)*(strlen(title)+artIDLen+IDLen+2)+1);
    if(line == NULL) return E_MALLOC_FAILED;

    sprintf(line, "%d\%%%s\%%%d\n", ID, title, artistID);

    printf("%s",line);

    check = saveLine(albumFile, line);
    free(line);
    return check;
}

/* int  saveAllArtists(){}; */
int saveArtist(char *name, int ID){

    char *line = NULL;
    int check;
    FILE *file = fopen(SOURCE_LOCATION""ARTISTS_FILE_NAME, "rw");

    /*get memory for the line that will be saved*/
    line = malloc(sizeof(char)*(strlen(name)+floor(log10(ID)+1)+1)+1);
    if(line == NULL) return E_MALLOC_FAILED;

    sprintf(line, "%d\%%%s\n", ID, name);

    printf("%s",line);

    check = saveLine(file, line);
    free(line);
    return check;
}

/* int  saveAllLoans(){} */
int saveLoan(int ID, int albumID, char *user, int time, Boolean isReturned){

    char *line = NULL;
    int check;
    /*length of ints (how many chars)*/
    int timeLen;
    int albIDLen;
    int IDLen;
    int boolLen = 1;
    FILE *file = fopen(SOURCE_LOCATION""LOANS_FILE_NAME, "rw");

    /*find log base 10, then round down*/
    timeLen = floor(log10(time)) + 1;
    albIDLen = floor(log10(albumID)) + 1;
    IDLen = floor(log10(ID)) + 1; 

    /*get memory for the line that will be saved*/
    line = malloc(sizeof(char)*(strlen(user)+timeLen+albIDLen+IDLen+boolLen+4)+1);
    if(line == NULL) return E_MALLOC_FAILED;

    sprintf(line, "%d\%%%d\%%%s\%%%d\%%%d\n", ID, albumID, user, time, isReturned);

    printf("%s",line);

    check = saveLine(file, line);
    free(line);
    return check;
}

/* int  saveAllComments(){}; */
int saveCommentArtist(char *owner, char *body, int ID, int artistID){

    char *line = NULL;
    int check;
    /*length of ints (how many chars)*/
    int artIDLen;
    int IDLen;
    FILE *file = fopen(SOURCE_LOCATION""ARTIST_COMMENTS_FILE_NAME, "rw");

    /*find log base 10, then round down*/
    artIDLen = floor(log10(artistID))+1;
    IDLen = floor(log10(ID))+1; 

    /*get memory for the line that will be saved*/
    line = malloc(sizeof(char)*(strlen(body)+strlen(owner)+artIDLen+IDLen+3)+1);
    if(line == NULL) return E_MALLOC_FAILED;

    sprintf(line, "%d\%%%d\%%%s\%%%s\n", ID, artistID, owner, body);

    printf("%s",line);

    check = saveLine(file, line);
    free(line);
    return check;
}
int saveCommentAlbum(char *owner, char *body, int ID, int albumID){

    char *line = NULL;
    int check;
    /*length of ints (how many chars)*/
    int albIDLen;
    int IDLen;
    FILE *file = fopen(SOURCE_LOCATION""ALBUM_COMMENTS_FILE_NAME, "rw");

    /*find log base 10, then round down*/
    albIDLen = floor(log10(albumID)) + 1;
    IDLen = floor(log10(ID)) + 1; 

    /*get memory for the line that will be saved*/
    line = malloc(sizeof(char)*(strlen(body)+strlen(owner)+albIDLen+IDLen+3)+1);
    if(line == NULL) return E_MALLOC_FAILED;

    sprintf(line, "%d\%%%d\%%%s\%%%s\n", ID, albumID, owner, body);

    printf("%s",line);

    check = saveLine(file, line);
    free(line);
    return check;
}
int saveCommentUser(char *owner, char *body, int ID, char *userCode){

    char *line = NULL;
    int check;
    /*length of ints (how many chars)*/
    int IDLen;
    FILE *file = fopen(SOURCE_LOCATION""USER_COMMENTS_FILE_NAME, "rw");

    /*find log base 10, then round down*/
    IDLen = floor(log10(ID)) + 1; 

    /*get memory for the line that will be saved*/
    line = malloc(sizeof(char)*(strlen(body)+strlen(owner)+strlen(userCode)+IDLen+3)+1);
    if(line == NULL) return E_MALLOC_FAILED;

    sprintf(line, "%d\%%%s\%%%s\%%%s\n", ID, userCode, owner, body);

    printf("%s",line);

    check = saveLine(file, line);
    free(line);
    return check;
}


int saveLine(FILE *file, char *line){

    if(fputs(line, file) < 0) return SAVE_FAILURE;
    if(fputc('\n', file) < 0) return SAVE_FAILURE;

    return 1;

}
