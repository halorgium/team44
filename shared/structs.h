/* structure which holds a user and links next user in list*/
typedef struct userNode{ 
    /*int ID;*/
    char *userName;   /*unique user name*/
    char *emailAddress;
    Boolean isLibrarian;       /* FALSE-standard, TRUE-librarian*/
    /*history (loans) */
    /*comments*/
    struct userNode *next;
}userNode_t;

/*struct to hold an artist*/
typedef struct artistNode{
    int ID;
    char *name;
    /*maybe comments about artist??*/
    struct artistNode *next;
}artistNode_t;

/* structure which holds an album and links to next album in list */
typedef struct albumNode{
    int ID;
    char *title;
    Boolean isAvailable;   /*TRUE-available*/
    int artistID;

    /*may have comments about album or loan histroy*/
    
    struct albumNode* next;
}albumNode_t;


/* structure which holds a comment about an artist and links to next comment in list*/
typedef struct artistCommentNode{
    int ID;
    int artistID;
    
    char *userOwner;
    char *comment;
    struct commentNode *next;
}artistCommentNode_t;

/* structure which holds a comment about an album  and links to next comment in list*/
typedef struct albumCommentNode{
    int ID;
    int albumID;
    
    char *userOwner;
    char *comment;
    struct commentNode *next;
}albumCommentNode_t;

/* structure which holds a comment about a user and links to next comment in list*/
typedef struct userCommentNode{
    int ID;
    char *user;
    
    char *userOwner;
    char *comment;
    struct commentNode *next;
}userCommentNode_t;


/*struct to hold loans - may not be needed*/
typedef struct loan{
    int ID;
    int albumID;
    char *userName;
    /*no need for status boolean - recorded in album*/
    /*time in and tyime out*/
    
    struct loan *next;
}loan_t;
