#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "cgic.h"
#include "globals.h"
#include "../shared/defines.h"

static void doAddUserComment(void);
static void doViewUserComment(void);
static void doShowUserComment(void);

static int processAddForm(void);
static void printAddForm(int);

static void printAllUserCommentsByUser(int);
static void printAllUserCommentsForUser(int);

void printUserComment(void) {
    int result=0;
    char func[MAXSIZE_PAGENAME]={'\0'};

    /* if func field is set */
    result = cgiFormStringNoNewlines("func", func, MAXSIZE_PAGENAME);
    if(result != cgiFormSuccess || func == NULL) {
	/* Some sort of failure */
      strncpy(func, "main", MAXSIZE_PAGENAME);
    }

    if(strncmp(func, "add", MAXSIZE_PAGENAME) == 0) {
	/* Do add userComment etc */
	doAddUserComment();
    }
    else if(strncmp(func, "view", MAXSIZE_PAGENAME) == 0) {
	/* Do view userComment etc */
	doViewUserComment();
    }
    else {
	/* Do view userComment etc */
	doShowUserComment();
    }
}

static void doAddUserComment(void) {
    int result=0;
    Boolean isAdding=-1;

    fprintf(cgiOut, "<div class=\"head1\">Adding New User Comment</div>\n");

    if(isUserLibrarian(_currUserLogon) == FALSE) {
      /* curr User is not privileged */
      fprintf(cgiOut, "You can not add user comments\n");
      return;
    }

    /* if adding field is set */
    result=cgiFormIntegerBounded("adding", &isAdding, FALSE, TRUE, FALSE);
    if(result != cgiFormSuccess) {
	/* Some sort of failure */
	isAdding=FALSE;
    }

    if(isAdding) {
      /* The curr data is ready for processing */
      int newuserCommentid=processAddForm();
      if(newuserCommentid != -1) {
	/* User Comment added ok */
	fprintf(cgiOut, "Adding successful<br />\n");
	fprintf(cgiOut, "<a href=\"./?page=user&amp;userid=%d&hash=%d\">[View User]</a><br />\n", getUserCommentUser(newuserCommentid), _currUserLogon);
	fprintf(cgiOut, "<a href=\"./?page=usercomment&amp;func=view&amp;userid=%d&hash=%d\">[View User Comments for User]</a>\n", getUserCommentUser(newuserCommentid), _currUserLogon);
      }
      else {
	/* Some sort of failure */
	fprintf(cgiOut, "Adding failed - %d\n", newuserCommentid);
      }
    }
    else {
      /* Check for userid */
      int userid=-1;
      result = cgiFormInteger("userid", &userid, -1);
      if(result != cgiFormSuccess || userid == -1) {
	/* No userid */
	fprintf(cgiOut, "No user specified\n");
	return;
      }

      if(getUserExists(userid) == FALSE) {
	/* No userid */
	fprintf(cgiOut, "User [%d] does not exist in the database\n", userid);
	return;
      }

      /* Need to print form */
      fprintf(cgiOut, "Should we print add form even if no userid???? [Let user specify etc]\n");
      printAddForm(userid);
    }
}

static int processAddForm(void) {
    int result=0;
    int newUserCommentid=-1;
    int userid=-1;
    char *combody=malloc(sizeof(char)*MAXSIZE_USERCOMMENT);

    result = cgiFormInteger("usrid", &userid, -1);
    if(result != cgiFormSuccess || userid == -1) {
	return -1;
    }
    
    result = cgiFormStringNoNewlines("combody", combody, MAXSIZE_USERCOMMENT);
    if(result != cgiFormSuccess || combody == NULL) {
	printf("no comment\n");
	return -1;
    }

    newUserCommentid=addUserComment(userid, _currUserLogon, combody);
    if(newUserCommentid < 0) {
	return -1;
    }
    return newUserCommentid;
}

static void printAddForm(int userid) {
    fprintf(cgiOut, "<form method=\"get\" action=\"./\">\n");
    fprintf(cgiOut, "<table>\n");
    fprintf(cgiOut, "<tbody>\n");
    fprintf(cgiOut, "  <tr><td>\n");
    fprintf(cgiOut, "    <input type=\"hidden\" name=\"page\" value=\"usercomment\" />\n");
    fprintf(cgiOut, "    <input type=\"hidden\" name=\"func\" value=\"add\" />\n");
    fprintf(cgiOut, "    <input type=\"hidden\" name=\"usrid\" value=\"%d\" />\n", userid);
    fprintf(cgiOut, "    <input type=\"hidden\" name=\"adding\" value=\"%d\" />\n", TRUE);
    fprintf(cgiOut, "    <input type=\"hidden\" name=\"hash\" value=\"%d\" />\n", _currUserLogon);
    fprintf(cgiOut, "  </td></tr>\n");
    fprintf(cgiOut, "  <tr>\n");
    fprintf(cgiOut, "    <td class=\"describe\"><label title=\"User Name\">User Name: </label></td>\n");
    fprintf(cgiOut, "  </tr>\n");
    fprintf(cgiOut, "  <tr>\n");
    fprintf(cgiOut, "    <td class=\"field\">%s</td>\n", getUserName(userid));
    fprintf(cgiOut, "  </tr>\n");
    fprintf(cgiOut, "  <tr>\n");
    fprintf(cgiOut, "    <td class=\"describe\"><label for=\"combody\" title=\"The Comment\">The Comment: </label></td>\n");
    fprintf(cgiOut, "  </tr>\n");
    fprintf(cgiOut, "  <tr>\n");
    fprintf(cgiOut, "    <td class=\"field\"><input type=\"text\" id=\"combody\" name=\"combody\" size=\"%d\" /></td>\n", MAXSIZE_USERCOMMENT);
    fprintf(cgiOut, "  </tr>\n");
    fprintf(cgiOut, "\n");
    fprintf(cgiOut, "  <tr>\n");
    fprintf(cgiOut, "    <td><input type=\"submit\" value=\"Add Comment\" /></td>\n");
    fprintf(cgiOut, "  </tr>\n");
    fprintf(cgiOut, "</tbody>\n");
    fprintf(cgiOut, "</table>\n");
    fprintf(cgiOut, "</form>\n");

    fprintf(cgiOut, "<hr /><a href=\"./?page=user&amp;userid=%d&amp;hash=%d\">Back to User page</a>\n", userid, _currUserLogon);
}

static void doViewUserComment(void) {
    int result=0;
    int userid=0;
    int owner=0;

    /* check for userid field */
    result = cgiFormInteger("userid", &userid, -1);
    if(result != cgiFormSuccess || userid == -1) {
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
		printAllUserCommentsByUser(owner);
	    }
	    else {
		fprintf(cgiOut, "You can not view the User Comments written by User [%d]\n", owner);
	    }
	}
    }
    else {
	/* Check privileges */
	if(userid == _currUserLogon || isUserLibrarian(_currUserLogon) == TRUE) {
	    /* Will use userid */
	    printAllUserCommentsForUser(userid);
	}
	else {
	    fprintf(cgiOut, "You can not view the User Comments written about User [%d]\n", userid);
	}
    }
}

static void printAllUserCommentsByUser(int userid) {
    int *allUserComments=NULL;
    int curr_id=0;
    int count=0;

    fprintf(cgiOut, "<div class=\"head1\">Viewing User Comments written by %s</div>", getUserName(userid));

    allUserComments=getUserCommentsByUser(userid);

    if(allUserComments == NULL) {
	fprintf(cgiOut, "<div class=\"head1\">Error retrieving all User Comments</div>");
    }
    else {
	fprintf(cgiOut, "<table border=\"1\">\n");
	fprintf(cgiOut, "\n");
	fprintf(cgiOut, "<tbody>\n");

	if(getUserCommentsByUserCount(userid) == 0) {
	    fprintf(cgiOut, "  <tr>\n");
	    fprintf(cgiOut, "    <td>No user comments</td>\n");
	    fprintf(cgiOut, "  </tr>\n");
	}

	curr_id=allUserComments[count];
        while (curr_id != LAST_ID_IN_ARRAY) {
	    fprintf(cgiOut, "  <tr>\n");
	    fprintf(cgiOut, "    <td class=\"topper\">Comment written about ");
	    fprintf(cgiOut, "<a class=\"topper\" href=\"./?page=user&amp;userid=%d&amp;hash=%d\">%s</a>", getUserCommentUser(curr_id), _currUserLogon, getUserName(getUserCommentUser(curr_id)));
	    fprintf(cgiOut, "    </td>\n");
	    fprintf(cgiOut, "  </tr>\n");
	    fprintf(cgiOut, "  <tr>\n");
	    fprintf(cgiOut, "    <td>");
	    fprintf(cgiOut, "%s", getUserCommentBody(curr_id));
	    fprintf(cgiOut, "</td>\n");
	    fprintf(cgiOut, "  </tr>\n");

	    count++;
	    curr_id=allUserComments[count];
	}
	
	fprintf(cgiOut, "</tbody>\n");
	fprintf(cgiOut, "\n");
	fprintf(cgiOut, "</table>\n");

	free(allUserComments);
    }

    fprintf(cgiOut, "<hr /><a href=\"./?page=usercomment&amp;userid=%d&amp;hash=%d\">Back to User Comments page</a>\n", userid, _currUserLogon);
    fprintf(cgiOut, "<hr /><a href=\"./?page=user&amp;userid=%d&amp;hash=%d\">Back to User page</a>\n", userid, _currUserLogon);
}

static void printAllUserCommentsForUser(int userid) {
    int *allUserComments=NULL;
    int curr_id=0;
    int count=0;

    fprintf(cgiOut, "<div class=\"head1\">Viewing User Comments written about %s</div>", getUserName(userid));

    allUserComments=getUserCommentsForUser(userid);

    if(allUserComments == NULL) {
	fprintf(cgiOut, "<div class=\"head1\">Error retrieving all User Comments</div>");
    }
    else {
	fprintf(cgiOut, "<table border=\"1\">\n");
	fprintf(cgiOut, "\n");
	fprintf(cgiOut, "<tbody>\n");

	if(getUserCommentsForUserCount(userid) == 0) {
	    fprintf(cgiOut, "  <tr>\n");
	    fprintf(cgiOut, "    <td>No user comments</td>\n");
	    fprintf(cgiOut, "  </tr>\n");
	}

	curr_id=allUserComments[count];
        while (curr_id != LAST_ID_IN_ARRAY) {
	    fprintf(cgiOut, "  <tr>\n");
	    fprintf(cgiOut, "    <td class=\"topper\">Comment written by ");
	    fprintf(cgiOut, "<a class=\"topper\" href=\"./?page=user&amp;userid=%d&amp;hash=%d\">%s</a>", getUserCommentOwner(curr_id), _currUserLogon, getUserName(getUserCommentOwner(curr_id)));
	    fprintf(cgiOut, "    </td>\n");
	    fprintf(cgiOut, "  </tr>\n");
	    fprintf(cgiOut, "  <tr>\n");
	    fprintf(cgiOut, "    <td>");
	    fprintf(cgiOut, "%s", getUserCommentBody(curr_id));
	    fprintf(cgiOut, "</td>\n");
	    fprintf(cgiOut, "  </tr>\n");

	    count++;
	    curr_id=allUserComments[count];
	}
	
	fprintf(cgiOut, "</tbody>\n");
	fprintf(cgiOut, "\n");
	fprintf(cgiOut, "</table>\n");

	free(allUserComments);
    }

    fprintf(cgiOut, "<hr /><a href=\"./?page=usercomment&amp;userid=%d&amp;hash=%d\">Back to User Comments page</a>\n", userid, _currUserLogon);
    fprintf(cgiOut, "<hr /><a href=\"./?page=user&amp;userid=%d&amp;hash=%d\">Back to User page</a>\n", userid, _currUserLogon);
}

void doShowUserComment(void) {
  int result=0;
  int userid=0;
  
  /* check for userid field */
  result = cgiFormInteger("userid", &userid, -1);
  if(result != cgiFormSuccess || userid == -1) {
    /* Some sort of failure */
    /* Nothing specified */
    fprintf(cgiOut, "No id specified\n");
    return;
  }
  else {
    /* Check privileges */
    if(userid != _currUserLogon && isUserLibrarian(_currUserLogon) == FALSE) {
      fprintf(cgiOut, "You can not view info about Comments written by/about User [%d]\n", userid);
      return;
    }
  }
  
  /* Will use userid */
  fprintf(cgiOut, "<div class=\"head1\">Comments written by and written about %s</div>\n", getUserName(userid));

  fprintf(cgiOut, "<p><a href=\"./?page=usercomment&amp;func=view&amp;userid=%d&amp;hash=%d\">View User Comments written about <b>%s</b> (%d)</a></p>\n\n", userid, _currUserLogon, getUserCode(userid), getUserCommentsForUserCount(userid));

  if(isUserLibrarian(userid) == TRUE) {
	fprintf(cgiOut, "<p><a href=\"./?page=usercomment&amp;func=view&amp;owner=%d&amp;hash=%d\">View User Comments written by <b>%s</b> (%d)</a></p>\n\n", userid, _currUserLogon, getUserCode(userid), getUserCommentsByUserCount(userid));
  }

  fprintf(cgiOut, "<p><a href=\"./?page=albumcomment&amp;func=view&amp;owner=%d&amp;hash=%d\">View Album Comments written by <b>%s</b> (%d)</a></p>\n\n", userid, _currUserLogon, getUserCode(userid), getAlbumCommentsByUserCount(userid));

  fprintf(cgiOut, "<p><a href=\"./?page=artistcomment&amp;func=view&amp;owner=%d&amp;hash=%d\">View Artist Comments written by <b>%s</b> (%d)</a></p>\n\n", userid, _currUserLogon, getUserCode(userid), getArtistCommentsByUserCount(userid));

  fprintf(cgiOut, "<hr /><a href=\"./?page=user&amp;userid=%d&amp;hash=%d\">Back to User page</a>\n", userid, _currUserLogon);
}
