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
    Boolean isAdding=-1;

    fprintf(cgiOut, "<div class=\"head1\">Adding New Album Comment</div>\n");

    /* if adding field is set */
    result=cgiFormIntegerBounded("adding", &isAdding, FALSE, TRUE, FALSE);
    if(result != cgiFormSuccess) {
	/* Some sort of failure */
	isAdding=FALSE;
    }

    if(isAdding) {
      /* The curr data is ready for processing */
      int newalbumCommentid=processAddForm();
      if(newalbumCommentid > 0) {
	/* Album Comment added ok */
	fprintf(cgiOut, "Adding successful<br />\n");
	fprintf(cgiOut, "<a href=\"./?page=album&amp;albumid=%d&amp;hash=%d\">[View Album]</a><br />\n", getAlbumCommentAlbum(newalbumCommentid), _currUserLogon);
	fprintf(cgiOut, "<a href=\"./?page=albumcomment&amp;func=view&amp;albumid=%d&amp;hash=%d\">[View Album Comments for Album]</a><br />\n", getAlbumCommentAlbum(newalbumCommentid), _currUserLogon);
	fprintf(cgiOut, "<a href=\"./?page=albumcomment&amp;func=add&amp;albumid=%d&amp;hash=%d\">[Add Another Comment for this Album]</a>\n", getAlbumCommentAlbum(newalbumCommentid), _currUserLogon);
      }
      else {
	  /* Some sort of failure */
	  fprintf(cgiOut, "<a href=\"./?page=albumcomment&amp;func=add&amp;hash=%d\">[Add Another Comment on an Album]</a>\n", _currUserLogon);
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
    char *combody=malloc(sizeof(char)*MAXSIZE_ALBUMCOMMENT);

    result = cgiFormInteger("artid", &albumid, -1);
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
	    return E_UNKNOWN;
	}
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
	    fprintf(cgiOut, "  <option value=\"%d\">%s</option>\n", curr_id, getAlbumTitle(curr_id));
	    count++;
	    curr_id=allAlbums[count];
	}
	
	fprintf(cgiOut, "</select>\n");
	fprintf(cgiOut, "</td>\n");
    }
    else {
	fprintf(cgiOut, "    <td class=\"field\">%s</td>\n", getAlbumTitle(albumid));
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

    fprintf(cgiOut, "<div class=\"head1\">Viewing Album Comments written by %s</div>", getUserName(userid));

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
		fprintf(cgiOut, "  <tr>\n");
		fprintf(cgiOut, "    <td class=\"topper\">Comment written about ");
		fprintf(cgiOut, "<a class=\"topper\" href=\"./?page=album&amp;albumid=%d&amp;hash=%d\">%s</a>", getAlbumCommentAlbum(curr_id), _currUserLogon, getAlbumTitle(getAlbumCommentAlbum(curr_id)));
		fprintf(cgiOut, "    </td>\n");
		fprintf(cgiOut, "  </tr>\n");
		fprintf(cgiOut, "  <tr>\n");
		fprintf(cgiOut, "    <td>");
		fprintf(cgiOut, "%s", getAlbumCommentBody(curr_id));
		fprintf(cgiOut, "</td>\n");
		fprintf(cgiOut, "  </tr>\n");
		
		count++;
		curr_id=allAlbumComments[count];
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

static void printAllAlbumCommentsForAlbum(int albumid) {
    int *allAlbumComments=NULL;
    int curr_id=0;
    int count=0;

    fprintf(cgiOut, "<div class=\"head1\">Viewing Album Comments written about %s</div>", getAlbumTitle(albumid));

    allAlbumComments=getAlbumCommentsForAlbum(albumid);

    if(allAlbumComments == NULL) {
	fprintf(cgiOut, "<div class=\"head1\">Error retrieving all Album Comments</div>");
    }
    else {
	if(getAlbumCommentsForAlbumCount(albumid) == 0) {
	    fprintf(cgiOut, "No album comments\n");
	}
	else {
	    fprintf(cgiOut, "<table border=\"1\">\n");
	    fprintf(cgiOut, "\n");
	    fprintf(cgiOut, "<tbody>\n");
	    
	    curr_id=allAlbumComments[count];
	    while (curr_id != LAST_ID_IN_ARRAY) {
		fprintf(cgiOut, "  <tr>\n");
		fprintf(cgiOut, "    <td class=\"topper\">Comment written by ");
		userLink(" class=\"topper\"", getAlbumCommentOwner(curr_id), getUserName(getAlbumCommentOwner(curr_id)), cgiOut);
		fprintf(cgiOut, "    </td>\n");
		fprintf(cgiOut, "  </tr>\n");
		fprintf(cgiOut, "  <tr>\n");
		fprintf(cgiOut, "    <td>");
		fprintf(cgiOut, "%s", getAlbumCommentBody(curr_id));
		fprintf(cgiOut, "</td>\n");
		fprintf(cgiOut, "  </tr>\n");
		
		count++;
		curr_id=allAlbumComments[count];
	    }
	    
	    fprintf(cgiOut, "</tbody>\n");
	    fprintf(cgiOut, "\n");
	    fprintf(cgiOut, "</table>\n");
	}
	
	free(allAlbumComments);
    }

    fprintf(cgiOut, "<hr /><a href=\"./?page=album&amp;albumid=%d&amp;hash=%d\">Back to Album page</a>\n", albumid, _currUserLogon);
}
