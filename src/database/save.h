#ifndef _SAVE_H_
#define _SAVE_H_

#include "../shared/defines.h"

/*int saveDatabase();*/

/*int  saveAllUsers();*/
int saveUser(const char *name, const char* userCode, const char *email, Boolean bool);

/*int  saveAllAlbums();*/
int saveAlbum(const char* title, const int artistID, int ID);

/*int  saveAllArtists();*/
int saveArtist(char *name, int ID);

/*int  saveAllLoans();*/
int saveLoan(int ID, int albumID, char *user, int time, int isReturned);

/*int  saveAllComments();*/
int saveCommentArtist(char *owner, char *body, int ID, int artistID);
int saveCommentAlbum(char *owner, char *body, int ID, int albumID);
int saveCommentUser(char *owner, char *body, int ID, char *userCode);
int saveLine(FILE *file, char *line);

#endif
