#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "cgic.h"
#include "globals.h"
#include "../shared/defines.h"

static void doAddArtistComment(void);
static void doViewArtistComment(void);

static int processAddForm(void);
static void printAddForm(void);

static void printAllArtistCommentsByUser(int);
static void printAllArtistCommentsForArtist(int);

void printArtistComment(void) {
    int result=0;
    char func[MAXSIZE_PAGENAME]={'\0'};

    /* if func field is set */
    result = cgiFormStringNoNewlines("func", func, MAXSIZE_PAGENAME);
    if(result != cgiFormSuccess || func == NULL) {
	/* Some sort of failure */
	strncpy(func, "view", MAXSIZE_PAGENAME);
    }

    if(strncmp(func, "add", MAXSIZE_PAGENAME) == 0) {
	/* Do add artistComment etc */
	doAddArtistComment();
    }
    else {
	/* Default */
	/* Do view artistComment etc */
	doViewArtistComment();
    }
}

static void doAddArtistComment(void) {
    int result=0;
    Boolean isAdding=-1;

    fprintf(cgiOut, "<div class=\"head1\">Adding New Artist Comment</div>\n");

    /* if adding field is set */
    result=cgiFormIntegerBounded("adding", &isAdding, FALSE, TRUE, FALSE);
    if(result != cgiFormSuccess) {
	/* Some sort of failure */
	isAdding=FALSE;
    }

    if(isAdding) {
	/* The curr data is ready for processing */
	int newartistCommentid=processAddForm();
	if(newartistCommentid > 0) {
	    /* Artist Comment added ok */
	    const char* artistname=getArtistName(getArtistCommentArtist(newartistCommentid));
	    
	    fprintf(cgiOut, "Adding successful<br />\n");
	    fprintf(cgiOut, "<a href=\"./?page=artist&amp;artistid=%d&amp;hash=%d\">[View Info about %s]</a><br />\n", getArtistCommentArtist(newartistCommentid), _currUserLogon, artistname);
	    fprintf(cgiOut, "<a href=\"./?page=artistcomment&amp;func=view&amp;artistid=%d&amp;hash=%d\">[View All Comments about %s]</a><br />\n", getArtistCommentArtist(newartistCommentid), _currUserLogon, artistname);
	    fprintf(cgiOut, "<a href=\"./?page=artistcomment&amp;func=add&amp;artistid=%d&amp;hash=%d\">[Write another Comment about %s]</a>\n", getArtistCommentArtist(newartistCommentid), _currUserLogon, artistname);
	}
	else {
	    /* Some sort of failure */
	    fprintf(cgiOut, "<a href=\"./?page=artistcomment&amp;func=add&amp;hash=%d\">[Write another Comment about an Artist]</a>\n", _currUserLogon);
	}
    }
    else {
	/* Need to print form */
	printAddForm();
    }
}

static int processAddForm(void) {
    int result=0;
    int newArtistCommentid=-1;
    int artistid=-1;
    char *combody=malloc(sizeof(char)*MAXSIZE_ARTISTCOMMENT);

    result = cgiFormInteger("artid", &artistid, -1);
    if(result != cgiFormSuccess || artistid == -1) {
	newArtistCommentid = E_FORM;
    }
    else {
	result = cgiFormStringNoNewlines("combody", combody, MAXSIZE_ARTISTCOMMENT);
	if(result != cgiFormSuccess || combody == NULL) {
	    newArtistCommentid = E_INVALID_PARAM;
	}
	else {
	    newArtistCommentid=addArtistComment(artistid, _currUserLogon, combody);
	}
    }

    if(newArtistCommentid < 0) {
	switch(newArtistCommentid) {
	case DB_NEXTID_ERROR:
	    fprintf(cgiOut, "Database failure: ID allocation failed<br />\n");
	    break;
	case DB_SAVE_FAILURE:
	    fprintf(cgiOut, "Database failure: Artist Comment save incomplete<br />\n");
	    break;
	case E_NOARTIST:
	    fprintf(cgiOut, "Artist [%d] does not exist<br />\n", artistid);
	    break;
	case E_NOUSER:
	    fprintf(cgiOut, "User [%d] attempting to add comment does not exist<br />\n", _currUserLogon);
	    break;
	case E_FORM:
	    fprintf(cgiOut, "Artist not selected<br />\n");
	    break;
	case E_INVALID_PARAM:
	    fprintf(cgiOut, "Comment body is invalid<br />\n");
	    break;
	case E_MALLOC_FAILED:
	    fprintf(cgiOut, "Memory Allocation Error<br />\n");
	    break;
	default:
	    fprintf(cgiOut, "Unknown error: Adding failed<br />\n");
	    return E_UNKNOWN;
	}
    }
    return newArtistCommentid;
}

static void printAddForm(void) {
    int result=-1;
    
    /* Check for artistid */
    int artistid=-1;
    result = cgiFormInteger("artistid", &artistid, -1);
    if(result != cgiFormSuccess || artistid == -1) {
	/* No artistid */
	artistid=-1;
    }
    
    if(artistid != -1 && getArtistExists(artistid) == FALSE) {
	/* No artistid */
	fprintf(cgiOut, "Artist [%d] does not exist in the database\n", artistid);
	artistid=-1;
    }
    
    if(getArtistsCount() == 0) {
	fprintf(cgiOut, "No artists in database<br />\n");
	fprintf(cgiOut, "<a href=\"./?page=artist&amp;func=add&hash=%d\">[Add Artist]</a><br />\n", _currUserLogon);
	return;
    }

    fprintf(cgiOut, "<form method=\"get\" action=\"./\">\n");
    fprintf(cgiOut, "<table>\n");
    fprintf(cgiOut, "<tbody>\n");
    fprintf(cgiOut, "  <tr><td>\n");
    fprintf(cgiOut, "    <input type=\"hidden\" name=\"page\" value=\"artistcomment\" />\n");
    fprintf(cgiOut, "    <input type=\"hidden\" name=\"func\" value=\"add\" />\n");
    if(artistid != -1) {
	fprintf(cgiOut, "    <input type=\"hidden\" name=\"artid\" value=\"%d\" />\n", artistid);
    }
    fprintf(cgiOut, "    <input type=\"hidden\" name=\"adding\" value=\"%d\" />\n", TRUE);
    fprintf(cgiOut, "    <input type=\"hidden\" name=\"hash\" value=\"%d\" />\n", _currUserLogon);
    fprintf(cgiOut, "  </td></tr>\n");
    fprintf(cgiOut, "  <tr>\n");
    fprintf(cgiOut, "    <td class=\"describe\"><label title=\"Artist Title\">Artist Name: </label></td>\n");
    fprintf(cgiOut, "  </tr>\n");
    fprintf(cgiOut, "  <tr>\n");
    if(artistid == -1) {
	int *allArtists=getArtists();
	int count=0;
	int curr_id=0;
	
	fprintf(cgiOut, "    <td class=\"field\">\n");
	/* Print out the <select> for all artists */
	fprintf(cgiOut, "<select id=\"artid\" name=\"artid\" size=\"5\">\n");
	
	curr_id=allArtists[count];
	while (curr_id != LAST_ID_IN_ARRAY) {
	    fprintf(cgiOut, "  <option value=\"%d\">%s</option>\n", curr_id, getArtistName(curr_id));
	    count++;
	    curr_id=allArtists[count];
	}
	
	fprintf(cgiOut, "</select>\n");
	fprintf(cgiOut, "</td>\n");
    }
    else {
	fprintf(cgiOut, "    <td class=\"field\">%s</td>\n", getArtistName(artistid));
    }
    fprintf(cgiOut, "  </tr>\n");
    fprintf(cgiOut, "  <tr>\n");
    fprintf(cgiOut, "    <td class=\"describe\"><label for=\"combody\" title=\"The Comment\">The Comment: </label></td>\n");
    fprintf(cgiOut, "  </tr>\n");
    fprintf(cgiOut, "  <tr>\n");
    fprintf(cgiOut, "    <td class=\"field\"><input type=\"text\" id=\"combody\" name=\"combody\" size=\"%d\" /></td>\n", MAXSIZE_ARTISTCOMMENT);
    fprintf(cgiOut, "  </tr>\n");
    fprintf(cgiOut, "\n");
    fprintf(cgiOut, "  <tr>\n");
    fprintf(cgiOut, "    <td><input type=\"submit\" value=\"Add Comment\" /></td>\n");
    fprintf(cgiOut, "  </tr>\n");
    fprintf(cgiOut, "</tbody>\n");
    fprintf(cgiOut, "</table>\n");
    fprintf(cgiOut, "</form>\n");

    if(artistid != -1) {
	fprintf(cgiOut, "<hr /><a href=\"./?page=artist&amp;artistid=%d&amp;hash=%d\">Back to Artist page</a>\n", artistid, _currUserLogon);
    }
}

static void doViewArtistComment(void) {
    int result=0;
    int artistid=0;
    int owner=0;

    /* check for artistid field */
    result = cgiFormInteger("artistid", &artistid, -1);
    if(result != cgiFormSuccess || artistid == -1) {
	/* Some sort of failure */
	/* Check for owner field */
	result = cgiFormInteger("owner", &owner, -1);
	if(result != cgiFormSuccess || owner == -1) {
	    /* Nothing specified */
	    fprintf(cgiOut, "No artist specified\n");
	    fprintf(cgiOut, "<a href=\"./?page=artist&amp;hash=%d\">[View All Artists]</a>\n", _currUserLogon);
	    return;
	}
	else {
	    /* Check User exists */
	    if(getUserExists(owner) == FALSE) {
		fprintf(cgiOut, "User [%d] does not exist<br />\n", owner);
		fprintf(cgiOut, "<a href=\"./?page=user&amp;hash=%d\">[View All Users]</a>\n", _currUserLogon);
		return;
	    }
  
	    /* Check privileges */
	    if(owner == _currUserLogon || isUserLibrarian(_currUserLogon) == TRUE) {
		/* Will use owner */
		printAllArtistCommentsByUser(owner);
	    }
	    else {
		fprintf(cgiOut, "You can not view the Artist Comments written by User [%d]\n", owner);
	    }
	}
    }
    else {
	/* Check Artist exists */
	if(getArtistExists(artistid) == FALSE) {
	    fprintf(cgiOut, "Artist [%d] does not exist<br />\n", artistid);
	    fprintf(cgiOut, "<a href=\"./?page=artist&amp;hash=%d\">[View All Artists]</a>\n", _currUserLogon);
	    return;
	}
	
	/* Everyone can view these */
	printAllArtistCommentsForArtist(artistid);
    }
}

static void printAllArtistCommentsByUser(int userid) {
    int *allArtistComments=NULL;
    int curr_id=0;
    int count=0;

    fprintf(cgiOut, "<div class=\"head1\">Viewing Artist Comments written by %s</div>", getUserName(userid));

    allArtistComments=getArtistCommentsByUser(userid);

    if(allArtistComments == NULL) {
	fprintf(cgiOut, "<div class=\"head1\">Error retrieving all Artist Comments</div>");
    }
    else {
	if(getArtistCommentsByUserCount(userid) == 0) {
	    fprintf(cgiOut, "No artist comments\n");
	}
	else {
	    fprintf(cgiOut, "<table border=\"1\">\n");
	    fprintf(cgiOut, "\n");
	    fprintf(cgiOut, "<tbody>\n");
	    
	    curr_id=allArtistComments[count];
	    while (curr_id != LAST_ID_IN_ARRAY) {
		fprintf(cgiOut, "  <tr>\n");
		fprintf(cgiOut, "    <td class=\"topper\">Comment written about ");
		fprintf(cgiOut, "<a class=\"topper\" href=\"./?page=artist&amp;artistid=%d&amp;hash=%d\">%s</a>", getArtistCommentArtist(curr_id), _currUserLogon, getArtistName(getArtistCommentArtist(curr_id)));
		fprintf(cgiOut, "    </td>\n");
		fprintf(cgiOut, "  </tr>\n");
		fprintf(cgiOut, "  <tr>\n");
		fprintf(cgiOut, "    <td>");
		fprintf(cgiOut, "%s", getArtistCommentBody(curr_id));
		fprintf(cgiOut, "</td>\n");
		fprintf(cgiOut, "  </tr>\n");
		
		count++;
		curr_id=allArtistComments[count];
	    }
	    
	    fprintf(cgiOut, "</tbody>\n");
	    fprintf(cgiOut, "\n");
	    fprintf(cgiOut, "</table>\n");
	}
	
	free(allArtistComments);
    }

    fprintf(cgiOut, "<hr /><a href=\"./?page=usercomment&amp;userid=%d&amp;hash=%d\">Back to User Comment page</a>\n", userid, _currUserLogon);
    fprintf(cgiOut, "<hr /><a href=\"./?page=user&amp;userid=%d&amp;hash=%d\">Back to User page</a>\n", userid, _currUserLogon);
}

static void printAllArtistCommentsForArtist(int artistid) {
    int *allArtistComments=NULL;
    int curr_id=0;
    int count=0;

    fprintf(cgiOut, "<div class=\"head1\">Viewing Artist Comments written about %s</div>", getArtistName(artistid));

    allArtistComments=getArtistCommentsForArtist(artistid);

    if(allArtistComments == NULL) {
	fprintf(cgiOut, "<div class=\"head1\">Error retrieving all Artist Comments</div>");
    }
    else {
	if(getArtistCommentsForArtistCount(artistid) == 0) {
	    fprintf(cgiOut, "No artist comments\n");
	}
	else {
	    fprintf(cgiOut, "<table border=\"1\">\n");
	    fprintf(cgiOut, "\n");
	    fprintf(cgiOut, "<tbody>\n");
	    
	    curr_id=allArtistComments[count];
	    while (curr_id != LAST_ID_IN_ARRAY) {
		fprintf(cgiOut, "  <tr>\n");
		fprintf(cgiOut, "    <td class=\"topper\">Comment written by ");
		userLink(" class=\"topper\"", getArtistCommentOwner(curr_id), getUserName(getArtistCommentOwner(curr_id)), cgiOut);
		fprintf(cgiOut, "    </td>\n");
		fprintf(cgiOut, "  </tr>\n");
		fprintf(cgiOut, "  <tr>\n");
		fprintf(cgiOut, "    <td>");
		fprintf(cgiOut, "%s", getArtistCommentBody(curr_id));
		fprintf(cgiOut, "</td>\n");
		fprintf(cgiOut, "  </tr>\n");
		
		count++;
		curr_id=allArtistComments[count];
	    }
	    
	    fprintf(cgiOut, "</tbody>\n");
	    fprintf(cgiOut, "\n");
	    fprintf(cgiOut, "</table>\n");
	}
	
	free(allArtistComments);
    }

    fprintf(cgiOut, "<hr /><a href=\"./?page=artist&amp;artistid=%d&amp;hash=%d\">Back to Artist page</a>\n", artistid, _currUserLogon);
}
