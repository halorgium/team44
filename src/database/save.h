#ifndef _SAVE_H_
#define _SAVE_H_

#include "../shared/defines.h"

int saveNextID(void);

int saveUser(int ID, const char* userCode, const char *userName, const char *emailAddress, Boolean librarian);
int saveAlbum(int ID, const char* title, int artistID);
int saveArtist(int ID, const char *name);

int saveUserComment(int ID, int userID, int owner, const char *body);
int saveAlbumComment(int ID, int albumID, int owner, const char *body);
int saveArtistComment(int ID, int artistID, int owner, const char *body);

int saveLoan(int ID, int albumID, int userID, long timeIn);
int saveLoanReturned(int loanID, long timeOut);

int saveLine(FILE *file, char *line);

#endif
