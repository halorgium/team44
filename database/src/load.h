/* load.c */
int loadDatabase(void);
int loadAllUsers(FILE *f);
/*int loadAlbum(const int ID);*/
int loadAllArtists(FILE *f);
int loadAllAlbums(FILE *f);
int loadAllLoans(FILE *f);
/*int loadLoan(const int ID);*/
int loadAlbumComments(FILE *f);
int loadArtistComments(FILE *f);
int loadUserComments(FILE *f);

