#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>

#include "../shared/defines.h"
#include "globals.h"
#include "save.h"

/*functions dealing with saving database to disk*/

int saveNextID(void) {
    FILE *outFile = fopen(SOURCE_LOCATION""NEXTID_FILE_NAME, "w");

    if(outFile == NULL) return DB_SAVE_FAILURE;
    fprintf(outFile, "%d%%%d%%%d%%%d%%%d%%%d%%\n", _nextAlbumID, _nextArtistID, _nextUserCommentID, _nextAlbumCommentID, _nextArtistCommentID, _nextLoanID);

    return 1;
}

int saveUser(int ID, const char *userCode, const char* userName, const char *emailAddress, Boolean isLibrarian){
    FILE *outFile = fopen(SOURCE_LOCATION""USERS_FILE_NAME, "a");

    if(outFile == NULL) return DB_SAVE_FAILURE;
    fprintf(outFile, "%d%%%s%%%s%%%s%%%d%%\n", ID, userCode, userName, emailAddress, isLibrarian);

    return 1;
}

int saveAlbum(int ID, const char* title, const int artistID){
    FILE *outFile = fopen(SOURCE_LOCATION""USERS_FILE_NAME, "a");

    if(outFile == NULL) return DB_SAVE_FAILURE;
    fprintf(outFile, "%d%%%s%%%d%%\n", ID, title, artistID);

    return 1;
}

int saveArtist(int ID, const char *name){

    char *line = NULL;
    int check;
    FILE *file = fopen(SOURCE_LOCATION""ARTISTS_FILE_NAME, "rw");

    /*get memory for the line that will be saved*/
    line = malloc(sizeof(char)*(strlen(name)+floor(log10(ID)+1)+1)+1);
    if(line == NULL) return E_MALLOC_FAILED;

    sprintf(line, "%d%%%s\n", ID, name);

    printf("%s",line);

    check = saveLine(file, line);
    free(line);
    return check;
}

int saveUserComment(int ID, int userID, int owner, const char *body){

    char *line = NULL;
    int check;
    /*length of ints (how many chars)*/
    int userIDLen;
    int IDLen;
    int ownerLen;
    FILE *file = fopen(SOURCE_LOCATION""USER_COMMENTS_FILE_NAME, "rw");

    /*find log base 10, then round down*/
    userIDLen = floor(log10(userID)) + 1;
    IDLen = floor(log10(ID)) + 1;
    ownerLen = floor(log10(owner))+1;

    /*get memory for the line that will be saved*/
    line = malloc(sizeof(char)*(strlen(body)+ownerLen+userIDLen+IDLen+3)+1);
    if(line == NULL) return E_MALLOC_FAILED;

    sprintf(line, "%d%%%d%%%d%%%s\n", ID, userID, owner, body);

    printf("%s",line);

    check = saveLine(file, line);
    free(line);
    return check;
}

int saveAlbumComment(int ID, int albumID, int owner, const char *body){
    char *line = NULL;
    int check;
    /*length of ints (how many chars)*/
    int albIDLen;
    int IDLen;
    int ownerLen;
    FILE *file = fopen(SOURCE_LOCATION""ALBUM_COMMENTS_FILE_NAME, "rw");

    /*find log base 10, then round down*/
    albIDLen = floor(log10(albumID)) + 1;
    IDLen = floor(log10(ID)) + 1;
    ownerLen = floor(log10(owner))+1; 

    /*get memory for the line that will be saved*/
    line = malloc(sizeof(char)*(strlen(body)+ownerLen+albIDLen+IDLen+3)+1);
    if(line == NULL) return E_MALLOC_FAILED;

    sprintf(line, "%d%%%d%%%d%%%s\n", ID, albumID, owner, body);

    printf("%s",line);

    check = saveLine(file, line);
    free(line);
    return check;
}

int saveArtistComment(int ID, int artistID, int owner, const char *body){
    char *line = NULL;
    int check;
    /*length of ints (how many chars)*/
    int artIDLen;
    int IDLen;
    int ownerLen;
    FILE *file = fopen(SOURCE_LOCATION""ARTIST_COMMENTS_FILE_NAME, "rw");

    /*find log base 10, then round down*/
    artIDLen = floor(log10(artistID))+1;
    IDLen = floor(log10(ID))+1;
    ownerLen = floor(log10(owner))+1; 

    /*get memory for the line that will be saved*/
    line = malloc(sizeof(char)*(strlen(body)+ownerLen+artIDLen+IDLen+3)+1);
    if(line == NULL) return E_MALLOC_FAILED;

    sprintf(line, "%d%%%d%%%d%%%s\n", ID, artistID, owner, body);

    printf("%s",line);

    check = saveLine(file, line);
    free(line);
    return check;
}

/* int  saveAllLoans(){} */
int saveLoan(int ID, int albumID, int userID, int timeIn, int timeOut, Boolean isReturned){

    char *line = NULL;
    int check;
    /*length of ints (how many chars)*/
    int userIDLen;
    int timeLen;
    int albIDLen;
    int IDLen;
    int boolLen = 1;
    FILE *file = fopen(SOURCE_LOCATION""LOANS_FILE_NAME, "rw");

    /*find log base 10, then round down*/
    userIDLen = floor(log10(userID)) + 1;
    timeLen = floor(log10(timeIn)) + 1;
    albIDLen = floor(log10(albumID)) + 1;
    IDLen = floor(log10(ID)) + 1; 

    /*get memory for the line that will be saved*/
    line = malloc(sizeof(char)*(userIDLen+timeLen+albIDLen+IDLen+boolLen+4)+1);
    if(line == NULL) return E_MALLOC_FAILED;

    sprintf(line, "%d%%%d%%%d%%%d%%%d\n", ID, albumID, userID, timeIn, isReturned);

    printf("%s",line);

    check = saveLine(file, line);
    free(line);
    return check;
}

int saveLine(FILE *file, char *line){

    if(fputs(line, file) < 0) return DB_SAVE_FAILURE;
    if(fputc('\n', file) < 0) return DB_SAVE_FAILURE;

    return 1;

}
