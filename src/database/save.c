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
    FILE *outFile = fopen(SOURCE_LOCATION""USER_FILE_NAME, "a");

    if(outFile == NULL) return DB_SAVE_FAILURE;
    fprintf(outFile, "%d%%%s%%%s%%%s%%%d%%\n", ID, userCode, userName, emailAddress, isLibrarian);

    return 1;
}

int saveAlbum(int ID, const char* title, const int artistID){
    FILE *outFile = fopen(SOURCE_LOCATION""ALBUM_FILE_NAME, "a");

    if(outFile == NULL) return DB_SAVE_FAILURE;
    fprintf(outFile, "%d%%%s%%%d%%\n", ID, title, artistID);

    return 1;
}

int saveArtist(int ID, const char *name){
    FILE *outFile = fopen(SOURCE_LOCATION""ARTIST_FILE_NAME, "a");

    if(outFile == NULL) return DB_SAVE_FAILURE;
    fprintf(outFile, "%d%%%s%%\n", ID, name);

    return 1;
}

int saveUserComment(int ID, int userID, int owner, const char *body){
    FILE *outFile = fopen(SOURCE_LOCATION""USER_COMMENT_FILE_NAME, "a");

    if(outFile == NULL) return DB_SAVE_FAILURE;
    fprintf(outFile, "%d%%%d%%%d%%%s%%\n", ID, userID, owner, body);

    return 1;
}

int saveAlbumComment(int ID, int albumID, int owner, const char *body){
    FILE *outFile = fopen(SOURCE_LOCATION""ALBUM_COMMENT_FILE_NAME, "a");

    if(outFile == NULL) return DB_SAVE_FAILURE;
    fprintf(outFile, "%d%%%d%%%d%%%s%%\n", ID, albumID, owner, body);

    return 1;
}

int saveArtistComment(int ID, int artistID, int owner, const char *body){
    FILE *outFile = fopen(SOURCE_LOCATION""ARTIST_COMMENT_FILE_NAME, "a");

    if(outFile == NULL) return DB_SAVE_FAILURE;
    fprintf(outFile, "%d%%%d%%%d%%%s%%\n", ID, artistID, owner, body);

    return 1;
}

int saveLoan(int ID, int albumID, int userID, long timeIn){
    FILE *outFile = fopen(SOURCE_LOCATION""LOAN_FILE_NAME, "a");

    if(outFile == NULL) return DB_SAVE_FAILURE;
    fprintf(outFile, "%d%%%d%%%d%%%ld%%\n", ID, albumID, userID, timeIn);

    return 1;
}

int saveLoanReturned(int loanID, long timeOut){
    FILE *outFile = fopen(SOURCE_LOCATION""LOANRET_FILE_NAME, "a");

    if(outFile == NULL) return DB_SAVE_FAILURE;
    fprintf(outFile, "%d%%%ld%%\n", loanID, timeOut);

    return 1;
}
