#ifndef _STRUCTS_H
#define _STRUCTS_H 1

#include "../shared/defines.h"

/* structure which holds a user and links next user in list*/
typedef struct userNode{
    int ID;           /*unique id based on usercode - but don't tell anyone*/
    char *userCode;   /*unique user name*/
    char *userName;
    char *emailAddress;
    Boolean isLibrarian;       /* FALSE-standard, TRUE-librarian*/
    
    struct userNode *next;
} userNode_t;

/* structure which holds an album and links to next album in list */
typedef struct albumNode{
    int ID;
    char *title;
    int artistID;

    struct albumNode* next;
} albumNode_t;

/*struct to hold an artist*/
typedef struct artistNode{
    int ID;
    char *name;
    
    struct artistNode *next;
} artistNode_t;

/* structure which holds a comment about a user and links to next comment in list*/
typedef struct userCommentNode{
    int ID;
    int userID;

    int userOwner;
    char *comment;

    struct userCommentNode *next;
} userCommentNode_t;

/* structure which holds a comment about an album  and links to next comment in list*/
typedef struct albumCommentNode{
    int ID;
    int albumID;
    
    int userOwner;
    char *comment;
    
    struct albumCommentNode *next;
} albumCommentNode_t;

/* structure which holds a comment about an artist and links to next comment in list*/
typedef struct artistCommentNode{
    int ID;
    int artistID;
    
    int userOwner;
    char *comment;
    
    struct artistCommentNode *next;
} artistCommentNode_t;

/*struct to hold loans */
typedef struct loanNode{
    int ID;
    int albumID;
    int userID;
    long timeStampIn;
    long timeStampOut;
    Boolean isReturned;
    
    struct loanNode *next;
} loanNode_t;

#endif
