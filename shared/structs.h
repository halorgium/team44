#ifndef _STRUCTS_H
#define _STRUCTS_H 1

#include "defines.h"

/* structure which holds a user and links next user in list*/
typedef struct userNode{ 
    char *userCode;   /*unique user name*/
    char *userName;
    char *emailAddress;
    Boolean isLibrarian;       /* FALSE-standard, TRUE-librarian*/
    
    struct userNode *next;
}userNode_t;

/*struct to hold an artist*/
typedef struct artistNode{
    int ID;
    char *name;
    
    struct artistNode *next;
}artistNode_t;

/* structure which holds an album and links to next album in list */
typedef struct albumNode{
    int ID;
    char *title;
    int artistID;
    struct albumNode* next;
}albumNode_t;

/* structure which holds a comment about an artist and links to next comment in list*/
typedef struct artistCommentNode{
    int ID;
    int artistID;
    
    char *userOwner;
    char *comment;
    
    struct artistCommentNode *next;
}artistCommentNode_t;

/* structure which holds a comment about an album  and links to next comment in list*/
typedef struct albumCommentNode{
    int ID;
    int albumID;
    
    char *userOwner;
    char *comment;
    
    struct albumCommentNode *next;
}albumCommentNode_t;

/* structure which holds a comment about a user and links to next comment in list*/
typedef struct userCommentNode{
    int ID;
    char *user;

    char *userOwner;
    char *comment;

    struct userCommentNode *next;
}userCommentNode_t;


/*struct to hold loans - may not be needed*/
typedef struct loanNode{
    int ID;
    int albumID;
    char *userName;
    int timetake;
    int timereturn;
    Boolean returned;
    
    struct loanNode *next;
}loan_t;

#endif
