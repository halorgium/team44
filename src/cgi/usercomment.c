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
static void printAddForm(void);

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
	if(newuserCommentid > 0) {
	    /* User Comment added ok */
	  int commentUser=getUserCommentUser(newuserCommentid);
	    char *username=getUserName(commentUser);
	    
	    fprintf(cgiOut, "Adding successful<br />\n");
	    fprintf(cgiOut, "<a href=\"./?page=user&amp;userid=%d&hash=%d\">[View Info about %s]</a><br />\n", commentUser, _currUserLogon, username);
	    fprintf(cgiOut, "<a href=\"./?page=usercomment&amp;func=view&amp;userid=%d&hash=%d\">[View All Comments about %s]</a><br />\n", commentUser, _currUserLogon, username);
	    fprintf(cgiOut, "<a href=\"./?page=usercomment&amp;func=add&amp;userid=%d&hash=%d\">[Write another comment about %s]</a>\n", commentUser, _currUserLogon, username);

	    free(username);
	}
	else {
	    /* Some sort of failure */
	    fprintf(cgiOut, "<a href=\"./?page=usercomment&amp;func=add&amp;hash=%d\">[Write another Comment about a User]</a>\n", _currUserLogon);
	}
    }
    else {
	/* Need to print form */
	printAddForm();
    }
}

static int processAddForm(void) {
    int result=0;
    int newUserCommentid=-1;
    int userid=-1;
    char *combody=malloc(sizeof(char)*MAXSIZE_USERCOMMENT);

    result = cgiFormInteger("usrid", &userid, -1);
    if(result != cgiFormSuccess || userid == -1) {
	newUserCommentid = E_FORM;
    }
    else {
	result = cgiFormStringNoNewlines("combody", combody, MAXSIZE_ALBUMCOMMENT);
	if(result != cgiFormSuccess || combody == NULL) {
	    newUserCommentid = E_INVALID_PARAM;
	}
	else {
	    newUserCommentid=addUserComment(userid, _currUserLogon, combody);
	}
    }

    if(newUserCommentid < 0) {
	switch(newUserCommentid) {
	case DB_NEXTID_ERROR:
	    fprintf(cgiOut, "Database failure: ID allocation failed<br />\n");
	    break;
	case DB_SAVE_FAILURE:
	    fprintf(cgiOut, "Database failure: User Comment save incomplete<br />\n");
	    break;
	case E_NOUSER:
	    fprintf(cgiOut, "User [%d] does not exist<br />\n", userid);
	    break;
	case E_FORM:
	    fprintf(cgiOut, "User not selected<br />\n");
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
    return newUserCommentid;
}

static void printAddForm(void) {
    int result=-1;
    
    /* Check for userid */
    int userid=-1;
    result = cgiFormInteger("userid", &userid, -1);
    if(result != cgiFormSuccess || userid == -1) {
	/* No userid */
	userid=-1;
    }
    
    if(userid != -1 && getUserExists(userid) == FALSE) {
	/* No userid */
	fprintf(cgiOut, "User [%d] does not exist in the database\n", userid);
	userid=-1;
    }

    if(getUsersCount() == 0) {
	fprintf(cgiOut, "No users in database<br />\n");
	fprintf(cgiOut, "<a href=\"./?page=user&amp;func=add&hash=%d\">[Add User]</a><br />\n", _currUserLogon);
	return;
    }
    
    fprintf(cgiOut, "<form method=\"get\" action=\"./\">\n");
    fprintf(cgiOut, "<table>\n");
    fprintf(cgiOut, "<tbody>\n");
    fprintf(cgiOut, "  <tr><td>\n");
    fprintf(cgiOut, "    <input type=\"hidden\" name=\"page\" value=\"usercomment\" />\n");
    fprintf(cgiOut, "    <input type=\"hidden\" name=\"func\" value=\"add\" />\n");
    if(userid != -1) {
	fprintf(cgiOut, "    <input type=\"hidden\" name=\"usrid\" value=\"%d\" />\n", userid);
    }
    fprintf(cgiOut, "    <input type=\"hidden\" name=\"adding\" value=\"%d\" />\n", TRUE);
    fprintf(cgiOut, "    <input type=\"hidden\" name=\"hash\" value=\"%d\" />\n", _currUserLogon);
    fprintf(cgiOut, "  </td></tr>\n");
    fprintf(cgiOut, "  <tr>\n");
    fprintf(cgiOut, "    <td class=\"describe\"><label title=\"User Name\">User Name: </label></td>\n");
    fprintf(cgiOut, "  </tr>\n");
    fprintf(cgiOut, "  <tr>\n");
    if(userid == -1) {
	int *allUsers=getUsers();
	int count=0;
	int curr_id=0;
	
	fprintf(cgiOut, "    <td class=\"field\">\n");
	/* Print out the <select> for all users */
	fprintf(cgiOut, "<select id=\"usrid\" name=\"usrid\" size=\"5\">\n");
	
	curr_id=allUsers[count];
	while (curr_id != LAST_ID_IN_ARRAY) {
	    fprintf(cgiOut, "  <option value=\"%d\">%s</option>\n", curr_id, getUserName(curr_id));
	    count++;
	    curr_id=allUsers[count];
	}
	
	fprintf(cgiOut, "</select>\n");
	fprintf(cgiOut, "</td>\n");
    }
    else {
	fprintf(cgiOut, "    <td class=\"field\">%s</td>\n", getUserName(userid));
    }
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

    if(userid != -1) {    
	fprintf(cgiOut, "<hr /><a href=\"./?page=user&amp;userid=%d&amp;hash=%d\">Back to User page</a>\n", userid, _currUserLogon);
    }
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
	    fprintf(cgiOut, "No user specified\n");
	    fprintf(cgiOut, "<a href=\"./?page=user&amp;hash=%d\">[View All Users]</a>\n", _currUserLogon);
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
		printAllUserCommentsByUser(owner);
	    }
	    else {
		fprintf(cgiOut, "You can not view the User Comments by User [%d]\n", owner);
	    }
	}
    }
    else {
	/* Check User exists */
	if(getUserExists(userid) == FALSE) {
	    fprintf(cgiOut, "User [%d] does not exist<br />\n", userid);
	    fprintf(cgiOut, "<a href=\"./?page=user&amp;hash=%d\">[View All Users]</a>\n", _currUserLogon);
	    return;
	}

	/* Check privileges */
	if(userid == _currUserLogon || isUserLibrarian(_currUserLogon) == TRUE) {
	    /* Will use userid */
	    printAllUserCommentsForUser(userid);
	}
	else {
	    fprintf(cgiOut, "You can not view the User Comments about User [%d]\n", userid);
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
	if(getUserCommentsByUserCount(userid) == 0) {
	    fprintf(cgiOut, "No user comments\n");
	}
	else {
	    fprintf(cgiOut, "<table border=\"1\">\n");
	    fprintf(cgiOut, "\n");
	    fprintf(cgiOut, "<tbody>\n");
	    
	    curr_id=allUserComments[count];
	    while (curr_id != LAST_ID_IN_ARRAY) {
		fprintf(cgiOut, "  <tr>\n");
		fprintf(cgiOut, "    <td class=\"topper\">Comment written about ");
		userLink(" class=\"topper\"", getUserCommentUser(curr_id), getUserName(getUserCommentUser(curr_id)), cgiOut);
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
	}
	
	free(allUserComments);
    }

    fprintf(cgiOut, "<hr /><a href=\"./?page=usercomment&amp;userid=%d&amp;hash=%d\">Back to User Comments page</a>\n", userid, _currUserLogon);
    fprintf(cgiOut, "<hr /><a href=\"./?page=user&amp;userid=%d&amp;hash=%d\">Back to User page</a>\n", userid, _currUserLogon);
}

static void printAllUserCommentsForUser(int userid) {
    int *allUserComments=NULL;
    int curr_id=0;
    int count=0;

    fprintf(cgiOut, "<div class=\"head1\">Viewing User Comments about %s</div>", getUserName(userid));

    allUserComments=getUserCommentsForUser(userid);

    if(allUserComments == NULL) {
	fprintf(cgiOut, "<div class=\"head1\">Error retrieving all User Comments</div>");
    }
    else {
	if(getUserCommentsForUserCount(userid) == 0) {
	    fprintf(cgiOut, "No user comments\n");
	}
	else {
	    fprintf(cgiOut, "<table border=\"1\">\n");
	    fprintf(cgiOut, "\n");
	    fprintf(cgiOut, "<tbody>\n");
	    
	    curr_id=allUserComments[count];
	    while (curr_id != LAST_ID_IN_ARRAY) {
		fprintf(cgiOut, "  <tr>\n");
		fprintf(cgiOut, "    <td class=\"topper\">Comment written by ");
		userLink(" class=\"topper\"", getUserCommentOwner(curr_id), getUserName(getUserCommentOwner(curr_id)), cgiOut);
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
	}
		    
	free(allUserComments);
    }

    fprintf(cgiOut, "<hr /><a href=\"./?page=usercomment&amp;userid=%d&amp;hash=%d\">Back to User Comments page</a>\n", userid, _currUserLogon);
    fprintf(cgiOut, "<hr /><a href=\"./?page=user&amp;userid=%d&amp;hash=%d\">Back to User page</a>\n", userid, _currUserLogon);
}

static void doShowUserComment(void) {
    int result=0;
    int userid=0;
  
    /* check for userid field */
    result = cgiFormInteger("userid", &userid, -1);
    if(result != cgiFormSuccess || userid == -1) {
	/* Some sort of failure */
	/* Nothing specified */
	fprintf(cgiOut, "No user specified<br />\n");
	fprintf(cgiOut, "<a href=\"./?page=user&amp;hash=%d\">[View All Users]</a>\n", _currUserLogon);
	return;
    }
    else {
	/* Check User exists */
	if(getUserExists(userid) == FALSE) {
	    fprintf(cgiOut, "User [%d] does exist<br />\n", userid);
	    fprintf(cgiOut, "<a href=\"./?page=user&amp;hash=%d\">[View All Users]</a>\n", _currUserLogon);
	    return;
	}
	
	/* Check privileges */
	if(userid != _currUserLogon && isUserLibrarian(_currUserLogon) == FALSE) {
	    fprintf(cgiOut, "You can not view info about Comments by/about User [%d]\n", userid);
	    return;
	}
    }

    {
      /* Will use userid */
      char *userCode=getUserCode(userid);
      char *userName=getUserName(userid);

      fprintf(cgiOut, "<div class=\"head1\">Comments written by and written about %s</div>\n", userName);
      
      fprintf(cgiOut, "<p><a href=\"./?page=usercomment&amp;func=view&amp;userid=%d&amp;hash=%d\">View Comments about <b>%s</b> (%d)</a></p>\n\n", userid, _currUserLogon, userCode, getUserCommentsForUserCount(userid));
      
      if(isUserLibrarian(userid) == TRUE) {
	fprintf(cgiOut, "<p><a href=\"./?page=usercomment&amp;func=view&amp;owner=%d&amp;hash=%d\">View Comments <b>%s</b> has written about Users (%d)</a></p>\n\n", userid, _currUserLogon, userCode, getUserCommentsByUserCount(userid));
      }
      
      fprintf(cgiOut, "<p><a href=\"./?page=albumcomment&amp;func=view&amp;owner=%d&amp;hash=%d\">View Comments <b>%s</b> has written about Albums (%d)</a></p>\n\n", userid, _currUserLogon, userCode, getAlbumCommentsByUserCount(userid));
      
      fprintf(cgiOut, "<p><a href=\"./?page=artistcomment&amp;func=view&amp;owner=%d&amp;hash=%d\">View Comments <b>%s</b> has written about Artists (%d)</a></p>\n\n", userid, _currUserLogon, userCode, getArtistCommentsByUserCount(userid));
      
      fprintf(cgiOut, "<hr /><a href=\"./?page=user&amp;userid=%d&amp;hash=%d\">Back to User page</a>\n", userid, _currUserLogon);

      free(userCode);
      free(userName);
    }
}
