#include <stdio.h>
#include <string.h>
#include <math.h>

#include "lib.h"
#include "save.h"
#include "../../shared/defines.h"

/*functions dealing with saving database to disk*/
int saveDatabase(void){}

int  saveAllUsers(){};
int saveUser(const char *name, char* userCode, const char *email, const Boolean bool){
    int check;
    char *line = NULL;
    FILE userFile = = fopen(strcat(SOURCE_LOCATION, USERS_FILE_NAME), "rw");

    line = malloc(sizeof(char)*(strlen(name)+strlen(userCode)+strlen(email)+1+3)+1);
    if(line == NULL) return E_MALLOC_FAILED;

    sprintf(line, "%s\%%%s\%%%d\%%%s\n", name, userCode, bool, email);

    printf("%s",line);

    check =  saveLine(userFile, line);
    free(line);
    return check;
}

int  saveAllAlbums(){};
int saveAlbum(const char* title, const int artistID, int ID){
    char *line = NULL;
    int check;
    FILE albumFile = = fopen(strcat(SOURCE_LOCATION, ALBUMS_FILE_NAME), "rw");

    /*get memory for the line that will be saved*/
    line = malloc(sizeof(char)*(strlen(title)+floor(log10(artistID))+floor(log10(ID))+2)+1);
    if(line == NULL) return E_MALLOC_FAILED;

    sprintf(line, "%d\%%%s\%%%d\n", ID, title, artistID);

    printf("%s",line);

    check = saveLine(albumFile, line);
    free(line);
    return check;
}

int  saveAllArtists(){};
int saveArtist(char *name, int ID){

    char *line = NULL;
    int check;
    FILE file = = fopen(strcat(SOURCE_LOCATION, ARTISTS_FILE_NAME), "rw");

    /*get memory for the line that will be saved*/
    line = malloc(sizeof(char)*(strlen(name)+floor(log10(ID))+1)+1);
    if(line == NULL) return E_MALLOC_FAILED;

    sprintf(line, "%d\%%%s\n", ID, name);

    printf("%s",line);

    check = saveLine(file, line);
    free(line);
    return check;
}

int  saveAllLoans(){}
int saveLoan(int ID, int albumID, char *user, int time, Boolean isReturned){

    char *line = NULL;
    int check;
    FILE file = = fopen(strcat(SOURCE_LOCATION, LOANS_FILE_NAME), "rw");

    /*get memory for the line that will be saved*/
    line = malloc(sizeof(char)*(strlen(user)+floor(log10(albumID))+floor(log10(ID))+floor(log10(time))+1+4)+1);
    if(line == NULL) return E_MALLOC_FAILED;

    sprintf(line, "%d\%%%d\%%%s\%%%d\%%%d\n", ID, albumID, user, time, isReturned);

    printf("%s",line);

    check = saveLine(file, line);
    free(line);
    return check;
}

int  saveAllComments(){};
int saveCommentArtist(char *owner, char *body, int ID, int artistID){};
int saveCommentAlbum(char *owner, char *body, int ID, int albumID){};
int saveCommentUser(char *owner, char *body, int ID, char *user){};



int saveCommentUser(char *owner, char *body, int ID, char *userName){};

int saveLine(FILE *file, char *line){

    if(fputs(line, file) < 0) return SAVE_FAILURE;
    if(fputc('\n', file) < 0) return SAVE_FAILURE;

    return 1;

}
