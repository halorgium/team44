#include <stdlib.h>
#include <stdio.h>

#include "../../shared/defines.h"

#include <string.h>
#include <time.h>

int rander(int min, int max) {
    return min+(int) (((float) (max-min))*rand()/(RAND_MAX+1.0));
}

int* getAllAlbums(void) {
   /* Init vars */
  int count=0;
    int *comments=NULL; /* Stores commentids to be returned */

    /* Allocate storage */
    if ((comments=calloc(sizeof(int), 20))==NULL) {
        return NULL;
    }
  
    for(count=0; count<20; count++) {
      comments[count]=count;
    }

    /* Add the value to signal end of array */
    comments[20]=LAST_ID_IN_ARRAY;

    return comments;
}

char* getAlbumTitle(int dsf) {
  char *me=calloc(sizeof(char), 11);
  strncpy(me, "name", 4);
  sprintf(me+4, "%6d", dsf);
  me[11]='\0';
  return me;
}

int getAlbumArtist(int dfd) {
  return 2;
}


int* getAllArtists(void) {
   /* Init vars */
  int count=0;
    int *comments=NULL; /* Stores commentids to be returned */

    /* Allocate storage */
    if ((comments=calloc(sizeof(int), 20))==NULL) {
        return NULL;
    }
  
    for(count=0; count<20; count++) {
      comments[count]=count;
    }

    /* Add the value to signal end of array */
    comments[20]=LAST_ID_IN_ARRAY;

    return comments;
}

char* getArtistName(int dsf) {
  char *me=calloc(sizeof(char), 11);
  strncpy(me, "name", 4);
  sprintf(me+4, "%6d", dsf);
  me[11]='\0';
  return me;
}



int* getAllLoansByUser(char *userid) {
    /* Init vars */
    int count=0;
    int e=0;
    int *comments=NULL; /* Stores commentids to be returned */
    
    /* Allocate storage */
    if ((comments=calloc(sizeof(int), 20))==NULL) {
        return NULL;
    }
    
    if(strncmp(userid, "admin", MAXSIZE_USERCODE) == 0) {
	e=10;
    }
    
    for(count=0; count<20; count++) {
	comments[count]=count+e;
    }

    /* Add the value to signal end of array */
    comments[20]=LAST_ID_IN_ARRAY;

    return comments;
}

int getLoanAlbum(int loanid) {
    return (2^loanid)%20;
}

int getLoanStatus(int loanid) {
    if(((loanid^4)%3) == 0) {
	return LOAN_ACTIVE;
    }
    return LOAN_INACTIVE;
}

char *getLoanStart(int loanid) {
    struct tm *tmtm=malloc(sizeof(struct tm));
    char *temp=malloc(sizeof(char)*100);
    
    tmtm->tm_sec=rander(0, 59);
    tmtm->tm_min=rander(0, 59);
    tmtm->tm_hour=rander(0, 23);
    tmtm->tm_mday=rander(1, 31);
    tmtm->tm_mon=rander(0, 11);
    tmtm->tm_year=rander(100, 103);
    tmtm->tm_wday=rander(0, 6);
    tmtm->tm_yday=rander(0, 365);
    tmtm->tm_isdst=0;
	

    strftime(temp, 90, "%d %m %Y %H:%M:%S", tmtm);
    free(tmtm);
    
    return temp;
}

char *getLoanEnd(int loanid) {
    if(getLoanStatus(loanid) == LOAN_INACTIVE) {
	return getLoanStart(loanid);
    }
    return "N/A";
}
