#ifndef _SAVE_H_
#define _SAVE_H_

#include "../shared/defines.h"

int saveUser(int ID, const char* userCode, const char *userName, const char *emailAddress, Boolean librarian);

int saveAlbum(int ID, const char* title, const int artistID);

int saveArtist(int ID, char *name);

int saveUserComment(int ID, int userID, int owner, char *body);
int saveAlbumComment(int ID, int albumID, int owner, char *body);
int saveArtistComment(int ID, int artistID, int owner, char *body);

int saveLoan(int ID, int albumID, int userID, int timeIn, int timeOut, Boolean isReturned);

int saveLine(FILE *file, char *line);

#endif
