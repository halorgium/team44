#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "cgic.h"
#include "globals.h"
#include "../shared/defines.h"

static void doAddAlbumComment(void);
static void doViewAlbumComment(void);

static int processAddForm(void);
static void printAddForm(int);

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
      if(newalbumCommentid != -1) {
	/* Album Comment added ok */
	fprintf(cgiOut, "Adding successful<br />\n");
	fprintf(cgiOut, "<a href=\"./?page=album&amp;albumid=%d&hash=%d\">[View Album]</a><br />\n", getAlbumCommentAlbum(newalbumCommentid), _currUserLogon);
	fprintf(cgiOut, "<a href=\"./?page=albumcomment&amp;func=view&amp;albumid=%d&hash=%d\">[View Album Comments for Album]</a>\n", getAlbumCommentAlbum(newalbumCommentid), _currUserLogon);
      }
      else {
	/* Some sort of failure */
	fprintf(cgiOut, "Adding failed - %d\n", newalbumCommentid);
      }
    }
    else {
      /* Check for albumid */
      int albumid=-1;
      result = cgiFormInteger("albumid", &albumid, -1);
      if(result != cgiFormSuccess || albumid == -1) {
	/* No albumid */
	fprintf(cgiOut, "No album specified\n");
	return;
      }

      if(getAlbumExists(albumid) == FALSE) {
	/* No albumid */
	fprintf(cgiOut, "Album [%d] does not exist in the database\n", albumid);
	return;
      }

      /* Need to print form */
      fprintf(cgiOut, "Should we print add form even if no albumid???? [Let user specify etc]\n");
      printAddForm(albumid);
    }
}

static int processAddForm(void) {
    int result=0;
    int newAlbumCommentid=-1;
    int albumid=-1;
    char *combody=malloc(sizeof(char)*MAXSIZE_ALBUMCOMMENT);

    result = cgiFormInteger("albid", &albumid, -1);
    if(result != cgiFormSuccess || albumid == -1) {
	return -1;
    }
    
    result = cgiFormStringNoNewlines("combody", combody, MAXSIZE_ALBUMCOMMENT);
    if(result != cgiFormSuccess || combody == NULL) {
	printf("no comment\n");
	return -1;
    }

    newAlbumCommentid=addAlbumComment(albumid, _currUserLogon, combody);
    if(newAlbumCommentid < 0) {
	return -1;
    }
    return newAlbumCommentid;
}

static void printAddForm(int albumid) {
    fprintf(cgiOut, "<form method=\"get\" action=\"./\">\n");
    fprintf(cgiOut, "<table>\n");
    fprintf(cgiOut, "<tbody>\n");
    fprintf(cgiOut, "  <tr><td>\n");
    fprintf(cgiOut, "    <input type=\"hidden\" name=\"page\" value=\"albumcomment\" />\n");
    fprintf(cgiOut, "    <input type=\"hidden\" name=\"func\" value=\"add\" />\n");
    fprintf(cgiOut, "    <input type=\"hidden\" name=\"albid\" value=\"%d\" />\n", albumid);
    fprintf(cgiOut, "    <input type=\"hidden\" name=\"adding\" value=\"%d\" />\n", TRUE);
    fprintf(cgiOut, "    <input type=\"hidden\" name=\"hash\" value=\"%d\" />\n", _currUserLogon);
    fprintf(cgiOut, "  </td></tr>\n");
    fprintf(cgiOut, "  <tr>\n");
    fprintf(cgiOut, "    <td class=\"describe\"><label title=\"Album Title\">Album Title: </label></td>\n");
    fprintf(cgiOut, "  </tr>\n");
    fprintf(cgiOut, "  <tr>\n");
    fprintf(cgiOut, "    <td class=\"field\">%s</td>\n", getAlbumTitle(albumid));
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
	    fprintf(cgiOut, "No id specified\n");
	    return;
	}
	else {
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
		fprintf(cgiOut, "<a class=\"topper\" href=\"./?page=user&amp;userid=%d&amp;hash=%d\">%s</a>", getAlbumCommentOwner(curr_id), _currUserLogon, getUserName(getAlbumCommentOwner(curr_id)));
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
