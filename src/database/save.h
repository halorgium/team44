#ifndef _SAVE_H_
#define _SAVE_H_

#include "../shared/defines.h"

/*int saveDatabase();*/

/*int  saveAllUsers();*/
int saveUser(const char *name, const char* userCode, const char *email, Boolean librarian, int ID);

/*int  saveAllAlbums();*/
int saveAlbum(const char* title, const int artistID, int ID);

/*int  saveAllArtists();*/
int saveArtist(char *name, int ID);

/*int  saveAllLoans();*/
int saveLoan(int ID, int albumID, int userID, int time, Boolean  isReturned);

/*int  saveAllComments();*/
int saveCommentArtist(int owner, char *body, int ID, int artistID);
int saveCommentAlbum(int owner, char *body, int ID, int albumID);
int saveCommentUser(int owner, char *body, int ID, int userID);
int saveLine(FILE *file, char *line);

#endif
