#include "lib.h"
#include "save.h"


/*functions dealing with saving database to disk*/
int saveDatabase(){};

int  saveAllUsers(){};
int saveUser(const char *name, char* userCode, const char *email, const Boolean bool){};

int  saveAllAlbums(){};
int saveAlbum(const char* title, const Boolean bool, const int artistID, int ID){};

int  saveAllArtists(){};
int saveArtist(){};

int  saveAllLoans(){};
int saveLoan(){};

int  saveAllComments(){};
int saveCommentArtist(char *owner, char *body, int ID, int artistID){};
int saveCommentAlbum(char *owner, char *body, int ID, int albumID){};



int saveCommentUser(char *owner, char *body, int ID, char *userName){};
