#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "cgic.h"
#include "globals.h"
#include "../shared/defines.h"

static void doAddAlbumComment(void);
static void doViewAlbumComment(void);

static int processAddForm(void);
static void printAddForm(void);

static void printAllAlbumCommentsByUser(int);
static void printAllAlbumCommentsForAlbum(int);

void printAlbumComment(void) {
    int result=0;
    char func[MAXSIZE_PAGENAME]={'\0'};

    /* if func field is set */
    result = cgiFormStringNoNewlines("func", func, MAXSIZE_PAGENAME);
    if(result != cgiFormSuccess || func == NULL) {
	/* Some sort of failure */
	strncpy(func, "view", MAXSIZE_PAGENAME);
    }

    if(strncmp(func, "add", MAXSIZE_PAGENAME) == 0) {
	/* Do add albumComment etc */
	doAddAlbumComment();
    }
    else {
	/* Default */
	/* Do view albumComment etc */
	doViewAlbumComment();
    }
}

static void doAddAlbumComment(void) {
    int result=0;
    Boolean isAdding=FALSE;

    fprintf(cgiOut, "<div class=\"head1\">Adding New Album Comment</div>\n");

    /* if adding field is set */
    result=cgiFormIntegerBounded("adding", &isAdding, FALSE, TRUE, FALSE);
    if(result != cgiFormSuccess) {
	/* Some sort of failure */
	isAdding=FALSE;
    }

    if(isAdding == TRUE) {
	/* The curr data is ready for processing */
	int newalbumCommentid=processAddForm();
	if(newalbumCommentid > 0) {
	    /* Album Comment added ok */
	    int albumid = getAlbumCommentAlbum(newalbumCommentid);
	    char *albumtitle=getAlbumTitle(albumid);
	  
	    fprintf(cgiOut, "Adding successful<br />\n");
	    fprintf(cgiOut, "<a href=\"./?page=album&amp;albumid=%d&amp;hash=%d\">[View Info about &quot;%s&quot;]</a><br />\n", albumid, _currUserLogon, albumtitle);
	    fprintf(cgiOut, "<a href=\"./?page=albumcomment&amp;func=view&amp;albumid=%d&amp;hash=%d\">[View All Comments about &quot;%s&quot;]</a><br />\n", albumid, _currUserLogon, albumtitle);
	    fprintf(cgiOut, " <a href=\"./?page=albumcomment&amp;func=add&amp;albumid=%d&amp;hash=%d\">[Write another Comment about &quot;%s&quot;]</a>\n", albumid, _currUserLogon, albumtitle);
	    
	    free(albumtitle);
	}
	else {
	    /* Some sort of failure */
	    fprintf(cgiOut, "<a href=\"./?page=albumcomment&amp;func=add&amp;hash=%d\">[Write another Comment about an Album]</a>\n", _currUserLogon);
	}
    }
    else {
	/* Need to print form */
	printAddForm();
    }
}

static int processAddForm(void) {
    int result=0;
    int newAlbumCommentid=-1;
    int albumid=-1;
    char *combody = NULL;
    combody = malloc(sizeof(char)*MAXSIZE_ALBUMCOMMENT);
    if(combody == NULL){
	fprintf(cgiOut, "Memory Allocation Error<br />\n");
	return E_MALLOC_FAILED;
    }

    result = cgiFormInteger("albid", &albumid, -1);
    if(result != cgiFormSuccess || albumid == -1) {
	newAlbumCommentid = E_FORM;
    }
    else {
	result = cgiFormStringNoNewlines("combody", combody, MAXSIZE_ALBUMCOMMENT);
	if(result != cgiFormSuccess || combody == NULL) {
	    newAlbumCommentid = E_INVALID_PARAM;
	}
	else {
	    newAlbumCommentid=addAlbumComment(albumid, _currUserLogon, combody);
	}
    }

    if(newAlbumCommentid < 0) {
	switch(newAlbumCommentid) {
	case DB_NEXTID_ERROR:
	    fprintf(cgiOut, "Database failure: ID allocation failed<br />\n");
	    break;
	case DB_SAVE_FAILURE:
	    fprintf(cgiOut, "Database failure: Album Comment save incomplete<br />\n");
	    break;
	case E_NOALBUM:
	    fprintf(cgiOut, "Album [%d] does not exist<br />\n", albumid);
	    break;
	case E_NOUSER:
	    fprintf(cgiOut, "User [%d] attempting to add comment does not exist<br />\n", _currUserLogon);
	    break;
	case E_FORM:
	    fprintf(cgiOut, "Album not selected<br />\n");
	    break;
	case E_INVALID_PARAM:
	    fprintf(cgiOut, "Comment body is invalid<br />\n");
	    break;
	case E_MALLOC_FAILED:
	    fprintf(cgiOut, "Memory Allocation Error<br />\n");
	    break;
	default:
	    fprintf(cgiOut, "Unknown error: Adding failed<br />\n");
	    newAlbumCommentid = E_UNKNOWN;
	    break;
	}
	free(combody);
    }
    return newAlbumCommentid;
}

static void printAddForm(void) {
    int result=-1;
    
    /* Check for albumid */
    int albumid=-1;
    result = cgiFormInteger("albumid", &albumid, -1);
    if(result != cgiFormSuccess || albumid == -1) {
	/* No albumid */
	albumid=-1;
    }
    
    if(albumid != -1 && getAlbumExists(albumid) == FALSE) {
	/* No albumid */
	fprintf(cgiOut, "Album [%d] does not exist in the database\n", albumid);
	albumid=-1;
    }
    
    if(getAlbumsCount() == 0) {
	fprintf(cgiOut, "No albums in database<br />\n");
	fprintf(cgiOut, "<a href=\"./?page=album&amp;func=add&hash=%d\">[Add Album]</a><br />\n", _currUserLogon);
	return;
    }

    fprintf(cgiOut, "<form method=\"get\" action=\"./\">\n");
    fprintf(cgiOut, "<table>\n");
    fprintf(cgiOut, "<tbody>\n");
    fprintf(cgiOut, "  <tr><td>\n");
    fprintf(cgiOut, "    <input type=\"hidden\" name=\"page\" value=\"albumcomment\" />\n");
    fprintf(cgiOut, "    <input type=\"hidden\" name=\"func\" value=\"add\" />\n");
    if(albumid != -1) {
	fprintf(cgiOut, "    <input type=\"hidden\" name=\"albid\" value=\"%d\" />\n", albumid);
    }
    fprintf(cgiOut, "    <input type=\"hidden\" name=\"adding\" value=\"%d\" />\n", TRUE);
    fprintf(cgiOut, "    <input type=\"hidden\" name=\"hash\" value=\"%d\" />\n", _currUserLogon);
    fprintf(cgiOut, "  </td></tr>\n");
    fprintf(cgiOut, "  <tr>\n");
    fprintf(cgiOut, "    <td class=\"describe\"><label title=\"Album Title\">Album Title: </label></td>\n");
    fprintf(cgiOut, "  </tr>\n");
    fprintf(cgiOut, "  <tr>\n");
    if(albumid == -1) {
	int *allAlbums=getAlbums();
	int count=0;
	int curr_id=0;
	
	fprintf(cgiOut, "    <td class=\"field\">\n");
	/* Print out the <select> for all albums */
	fprintf(cgiOut, "<select id=\"albid\" name=\"albid\" size=\"5\">\n");
	
	curr_id=allAlbums[count];
	while (curr_id != LAST_ID_IN_ARRAY) {
	    char *title = getAlbumTitle(curr_id);	    
	    fprintf(cgiOut, "  <option value=\"%d\">%s</option>\n", curr_id, title);
	    count++;
	    curr_id=allAlbums[count];	    
	    free(title);
	}
	
	fprintf(cgiOut, "</select>\n");
	fprintf(cgiOut, "</td>\n");

	free(allAlbums);
    }
    else {
	char *title = getAlbumTitle(albumid);
	fprintf(cgiOut, "    <td class=\"field\">%s</td>\n", title);
	free(title);
    }
    fprintf(cgiOut, "  </tr>\n");
    fprintf(cgiOut, "  <tr>\n");
    fprintf(cgiOut, "    <td class=\"describe\"><label for=\"combody\" title=\"The Comment\">The Comment: </label></td>\n");
    fprintf(cgiOut, "  </tr>\n");
    fprintf(cgiOut, "  <tr>\n");
    fprintf(cgiOut, "    <td class=\"field\"><input type=\"text\" id=\"combody\" name=\"combody\" size=\"%d\" /></td>\n", MAXSIZE_ALBUMCOMMENT);
    fprintf(cgiOut, "  </tr>\n");
    fprintf(cgiOut, "\n");
    fprintf(cgiOut, "  <tr>\n");
    fprintf(cgiOut, "    <td><input type=\"submit\" value=\"Add Comment\" /></td>\n");
    fprintf(cgiOut, "  </tr>\n");
    fprintf(cgiOut, "</tbody>\n");
    fprintf(cgiOut, "</table>\n");
    fprintf(cgiOut, "</form>\n");

    fprintf(cgiOut, "<hr /><a href=\"./?page=album&amp;albumid=%d&amp;hash=%d\">Back to Album page</a>\n", albumid, _currUserLogon);
}

static void doViewAlbumComment(void) {
    int result=0;
    int albumid=0;
    int owner=0;

    /* check for albumid field */
    result = cgiFormInteger("albumid", &albumid, -1);
    if(result != cgiFormSuccess || albumid == -1) {
	/* Some sort of failure */
	/* Check for owner field */
	result = cgiFormInteger("owner", &owner, -1);
	if(result != cgiFormSuccess || owner == -1) {
	    /* Nothing specified */
	    fprintf(cgiOut, "No album specified\n");
	    fprintf(cgiOut, "<a href=\"./?page=album&amp;hash=%d\">[View All Albums]</a>\n", _currUserLogon);
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
		printAllAlbumCommentsByUser(owner);
	    }
	    else {
		fprintf(cgiOut, "You can not view the Album Comments written by User [%d]\n", owner);
	    }
	}
    }
    else {
	/* Check Album exists */
	if(getAlbumExists(albumid) == FALSE) {
	    fprintf(cgiOut, "Album [%d] does not exist<br />\n", albumid);
	    fprintf(cgiOut, "<a href=\"./?page=album&amp;hash=%d\">[View All Albums]</a>\n", _currUserLogon);
	    return;
	}
	
	/* Everyone can view these */
	printAllAlbumCommentsForAlbum(albumid);
    }
}

static void printAllAlbumCommentsByUser(int userid) {
    int *allAlbumComments=NULL;
    int curr_id=0;
    int count=0;

    char *userName=getUserName(userid);
    fprintf(cgiOut, "<div class=\"head1\">Viewing Album Comments written by %s</div>", userName);
    free(userName);
    
    allAlbumComments=getAlbumCommentsByUser(userid);

    if(allAlbumComments == NULL) {
	fprintf(cgiOut, "<div class=\"head1\">Error retrieving all Album Comments</div>");
    }
    else {
	if(getAlbumCommentsByUserCount(userid) == 0) {
	    fprintf(cgiOut, "No album comments\n");
	}
	else {
	    fprintf(cgiOut, "<table border=\"1\">\n");
	    fprintf(cgiOut, "\n");
	    fprintf(cgiOut, "<tbody>\n");
	    
	    curr_id=allAlbumComments[count];
	    while (curr_id != LAST_ID_IN_ARRAY) {
		int albumid = getAlbumCommentAlbum(curr_id);
		char *commentBody = getAlbumCommentBody(curr_id);
		char *title = getAlbumTitle(albumid);
		
		fprintf(cgiOut, "  <tr>\n");
		fprintf(cgiOut, "    <td class=\"topper\">Comment written about ");
		fprintf(cgiOut, "<a class=\"topper\" href=\"./?page=album&amp;albumid=%d&amp;hash=%d\">%s</a>", albumid, _currUserLogon, title);
		fprintf(cgiOut, "    </td>\n");
		fprintf(cgiOut, "  </tr>\n");
		fprintf(cgiOut, "  <tr>\n");
		fprintf(cgiOut, "    <td>");
		fprintf(cgiOut, "%s", commentBody);
		fprintf(cgiOut, "</td>\n");
		fprintf(cgiOut, "  </tr>\n");
		
		count++;
		curr_id=allAlbumComments[count];

		free(commentBody);
		free(title);
	    }
	    
	    fprintf(cgiOut, "</tbody>\n");
	    fprintf(cgiOut, "\n");
	    fprintf(cgiOut, "</table>\n");
	}
	
	free(allAlbumComments);
    }

    fprintf(cgiOut, "<hr /><a href=\"./?page=usercomment&amp;userid=%d&amp;hash=%d\">Back to User Comment page</a>\n", userid, _currUserLogon);
    fprintf(cgiOut, "<hr /><a href=\"./?page=user&amp;userid=%d&amp;hash=%d\">Back to User page</a>\n", userid, _currUserLogon);
}


/*function prints all the comments written about this album.*/
static void printAllAlbumCommentsForAlbum(int albumid) {
    int *allAlbumComments=NULL;
    int curr_id=0;
    int count=0;
    
    char *title = getAlbumTitle(albumid);
    fprintf(cgiOut, "<div class=\"head1\">Viewing Album Comments written about %s</div>", title);
    free(title);
    
    allAlbumComments=getAlbumCommentsForAlbum(albumid);

    if(allAlbumComments == NULL) {
	fprintf(cgiOut, "<div class=\"head1\">Error retrieving all Album Comments</div>");
    }
    else {
	/*checks whether any comments have been written about this album*/
	if(getAlbumCommentsForAlbumCount(albumid) == 0) {
	    fprintf(cgiOut, "No album comments\n");
	}
	else {
	    fprintf(cgiOut, "<table border=\"1\">\n");
	    fprintf(cgiOut, "\n");
	    fprintf(cgiOut, "<tbody>\n");
	    
	    curr_id=allAlbumComments[count];
	    while (curr_id != LAST_ID_IN_ARRAY) {
		int ownerID = getAlbumCommentOwner(curr_id);
		char *name = getUserName(ownerID);
		char *body = getAlbumCommentBody(curr_id);
		
		fprintf(cgiOut, "  <tr>\n");
		fprintf(cgiOut, "    <td class=\"topper\">Comment written by ");
		userLink(" class=\"topper\"", ownerID, name, cgiOut);
		fprintf(cgiOut, "    </td>\n");
		fprintf(cgiOut, "  </tr>\n");
		fprintf(cgiOut, "  <tr>\n");
		fprintf(cgiOut, "    <td>");
		fprintf(cgiOut, "%s", body);
		fprintf(cgiOut, "</td>\n");
		fprintf(cgiOut, "  </tr>\n");
		
		count++;
		curr_id=allAlbumComments[count];

		free(body);
		free(name);
	    }
	    
	    fprintf(cgiOut, "</tbody>\n");
	    fprintf(cgiOut, "\n");
	    fprintf(cgiOut, "</table>\n");
	}
	
	free(allAlbumComments);
    }

    fprintf(cgiOut, "<hr /><a href=\"./?page=album&amp;albumid=%d&amp;hash=%d\">Back to Album page</a>\n", albumid, _currUserLogon);
}
