#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "cgic.h"
#include "globals.h"
#include "../shared/defines.h"

static void doAddUser(void);
static void doViewUser(void);

static int processAddForm(void);
static void printAddForm(void);

static void printAllUsers(void);
static void printAllUsersByType(Boolean);

static void printSpecificUser(int);

void printUser(void) {
    int result=0;
    char func[MAXSIZE_PAGENAME]={'\0'};

    /* if func field is set */
    result = cgiFormStringNoNewlines("func", func, MAXSIZE_PAGENAME);
    if(result != cgiFormSuccess || func == NULL) {
	/* Some sort of failure */
	/* Default to viewUsers */
	strncpy(func, "view", MAXSIZE_PAGENAME);
    }

    if(strncmp(func, "add", MAXSIZE_PAGENAME) == 0) {
	/* Do add user etc */
	doAddUser();
    }
    else {
	/* Default */
	/* Do view user etc */
	doViewUser();
    }
}

static void doAddUser(void) {
    int result=0;
    Boolean isAdding=-1;

    /* Check privileges of current user */
    if(isUserLibrarian(_currUserLogon) == FALSE) {
	fprintf(cgiOut, "You are not privleged to add new Users\n");
	return;
    }
    
    fprintf(cgiOut, "<div class=\"head1\">Adding New User</div>\n");

    /* if adding field is set */
    result=cgiFormIntegerBounded("adding", &isAdding, FALSE, TRUE, FALSE);
    if(result != cgiFormSuccess) {
	/* Some sort of failure */
	isAdding=FALSE;
    }

    if(isAdding) {
	/* The curr data is ready for processing */
	int newuserid=processAddForm();
	if(newuserid > 0) {
	    /* User added ok */
	    fprintf(cgiOut, "Adding successful<br />\n");
	    fprintf(cgiOut, "<a href=\"./?page=user&amp;userid=%d&amp;hash=%d\">[View User]</a><br />\n", newuserid, _currUserLogon);
	    fprintf(cgiOut, "<a href=\"./?page=user&amp;func=add&amp;hash=%d\">[Add Another User]</a>", _currUserLogon);
	}
	else {
	    /* User adding error */
	    /* Link back to add page */
	    fprintf(cgiOut, "<a href=\"./?page=user&amp;func=add&amp;hash=%d\">[Add Another User]</a>", _currUserLogon);
	}
    }
    else {
	/* Need to print form */
	printAddForm();
    }
}

static int processAddForm(void) {
    int result=0;
    int newUserid=-1;
    char *userCode=malloc(sizeof(char)*MAXSIZE_USERCODE);
    char *userName=malloc(sizeof(char)*MAXSIZE_USERNAME);
    char *userEmail=malloc(sizeof(char)*MAXSIZE_USEREMAIL);
    Boolean isLib=FALSE;

    result = cgiFormStringNoNewlines("usrcode", userCode, MAXSIZE_USERCODE);
    if(result != cgiFormSuccess || userCode == NULL) {
	newUserid = E_INVALID_PARAM;
    }
    else {
	result = cgiFormStringNoNewlines("usrname", userName, MAXSIZE_USERNAME);
	if(result != cgiFormSuccess || userName == NULL) {
	    newUserid = E_INVALID_PARAM;
	}
	else {
	    result = cgiFormStringNoNewlines("usremail", userEmail, MAXSIZE_USEREMAIL);
	    if(result != cgiFormSuccess || userEmail == NULL) {
		newUserid = E_INVALID_PARAM;
	    }
	    else {
		result = cgiFormCheckboxSingle("islib");
		if(result != cgiFormSuccess) {
		    isLib=FALSE;
		}
		else {
		    isLib=TRUE;
		}
		newUserid=addUser(userCode, userName, userEmail, isLib);
	    }
	}
    }
    
    if(newUserid < 0) {
	switch(newUserid) {
	case DB_NEXTID_ERROR:
	    fprintf(cgiOut, "Database failure: ID allocation failed<br />\n");
	    break;
	case DB_SAVE_FAILURE:
	    fprintf(cgiOut, "Database failure: User save incomplete<br />\n");
	    break;
	case E_INVALID_PARAM:
	    fprintf(cgiOut, "One or more input fields are invalid<br />\n");
	    break;
	case ALREADY_ADDED:
	    fprintf(cgiOut, "User Code &quot;%s&quot; already exists<br />\n", userCode);
	    break;
	case E_MALLOC_FAILED:
	    fprintf(cgiOut, "Memory Allocation Error<br />\n");
	    break;
	default:
	    fprintf(cgiOut, "Unknown error: Adding failed<br />\n");
	    return E_UNKNOWN;
	}
    }
    
    return newUserid;
}

static void printAddForm(void) {
    fprintf(cgiOut, "<form method=\"get\" action=\"./\">\n");
    fprintf(cgiOut, "<table>\n");
    fprintf(cgiOut, "<tbody>\n");
    fprintf(cgiOut, "  <tr class=\"hidden\">\n");
    fprintf(cgiOut, "    <td>\n");
    fprintf(cgiOut, "    <input type=\"hidden\" name=\"page\" value=\"user\" />\n");
    fprintf(cgiOut, "    <input type=\"hidden\" name=\"func\" value=\"add\" />\n");
    fprintf(cgiOut, "    <input type=\"hidden\" name=\"adding\" value=\"%d\" />\n", TRUE);
    fprintf(cgiOut, "    <input type=\"hidden\" name=\"hash\" value=\"%d\" />\n", _currUserLogon);
    fprintf(cgiOut, "    </td>\n");
    fprintf(cgiOut, "  </tr>\n");

    fprintf(cgiOut, "  <tr>\n");
    fprintf(cgiOut, "    <td class=\"describe\"><label for=\"usrcode\" title=\"User Code\">User Code: </label></td>\n");
    fprintf(cgiOut, "  </tr>\n");
    fprintf(cgiOut, "  <tr>\n");
    fprintf(cgiOut, "    <td class=\"field\"><input type=\"text\" id=\"usrcode\" name=\"usrcode\" size=\"%d\" /></td>\n", MAXSIZE_USERCODE);
    fprintf(cgiOut, "  </tr>\n");

    fprintf(cgiOut, "  <tr>\n");
    fprintf(cgiOut, "    <td class=\"describe\"><label for=\"usrname\" title=\"User Name\">User Name: </label></td>\n");
    fprintf(cgiOut, "  </tr>\n");
    fprintf(cgiOut, "  <tr>\n");
    fprintf(cgiOut, "    <td class=\"field\"><input type=\"text\" id=\"usrname\" name=\"usrname\" size=\"%d\" /></td>\n", MAXSIZE_USERNAME);
    fprintf(cgiOut, "  </tr>\n");

    fprintf(cgiOut, "  <tr>\n");
    fprintf(cgiOut, "    <td class=\"describe\"><label for=\"usremail\" title=\"Email Address\">Email Address: </label></td>\n");
    fprintf(cgiOut, "  </tr>\n");
    fprintf(cgiOut, "  <tr>\n");
    fprintf(cgiOut, "    <td class=\"field\"><input type=\"text\" id=\"usremail\" name=\"usremail\" size=\"%d\" /></td>\n", MAXSIZE_USEREMAIL);
    fprintf(cgiOut, "  </tr>\n");

    fprintf(cgiOut, "  <tr>\n");
    fprintf(cgiOut, "    <td class=\"field\">");
    fprintf(cgiOut, "<input type=\"checkbox\" id=\"islib\" name=\"islib\" value=\"%d\" />&nbsp;", TRUE);
    fprintf(cgiOut, "<label for=\"islib\" title=\"Is Librarian\">Is Librarian</label>\n");
    fprintf(cgiOut, "    </td>\n");
    fprintf(cgiOut, "  </tr>\n");

    fprintf(cgiOut, "\n");
    fprintf(cgiOut, "  <tr>\n");
    fprintf(cgiOut, "    <td><input type=\"submit\" value=\"Add User\" /></td>\n");
    fprintf(cgiOut, "  </tr>\n");
    fprintf(cgiOut, "</tbody>\n");
    fprintf(cgiOut, "</table>\n");
    fprintf(cgiOut, "</form>\n");
}

static void doViewUser(void) {
    int result=0;
    int userid=-1;

    /* if userid field is set */
    result=cgiFormInteger("userid", &userid, -1);
    if(result != cgiFormSuccess) {
	/* Some sort of failure */
	userid=-1;
    }
    else {
	/* Check userid exists */
	if(getUserExists(userid) == FALSE) {
	    fprintf(cgiOut, "User [%d] does not exist in the database", userid);
	    return;
	}
    }

    if(userid == -1) {
	/* check accesss rights */
	if(isUserLibrarian(_currUserLogon) == TRUE) {
	    /* print all users */
	    printAllUsers();
	}
	else {
	    fprintf(cgiOut, "You can not view information about all Users\n");
	}
    }
    else {
	/* check accesss rights */
	if(userid == _currUserLogon || isUserLibrarian(_currUserLogon)) {
	    /* print specific user */
	    printSpecificUser(userid);
	}
	else {
	    fprintf(cgiOut, "You can not view information about this User [%d]\n", userid);
	}
    }
}

static void printAllUsers(void) {
    fprintf(cgiOut, "<div class=\"head1\">Viewing All Users</div>");

    fprintf(cgiOut, "<b>Librarians: </b>\n");
    printAllUsersByType(TRUE);

    fprintf(cgiOut, "<hr />\n");

    fprintf(cgiOut, "<b>Standard Users: </b>\n");
    printAllUsersByType(FALSE);
}

static void printAllUsersByType(Boolean isLib) {
    int *allUsers=NULL;
    int curr_id=0;
    int count=0;

    if(isLib != TRUE) {
	isLib = FALSE;
    }
    else {
	isLib = TRUE;
    }
    
    allUsers=getUsersByType(isLib);

    if(allUsers == NULL) {
	fprintf(cgiOut, "<div class=\"head1\">Error retrieving all Users</div>");
    }
    else {
	if(getUsersByTypeCount(isLib) == 0) {
	    fprintf(cgiOut, "<br />No users\n");
	}
	else {
	    fprintf(cgiOut, "<table border=\"1\">\n");
	    fprintf(cgiOut, "\n");
	    fprintf(cgiOut, "<thead>\n");
	    fprintf(cgiOut, "  <tr>\n");
	    fprintf(cgiOut, "    <td class=\"thead\">User Code</td>\n");
	    fprintf(cgiOut, "    <td class=\"thead\">User Name</td>\n");
	    fprintf(cgiOut, "    <td class=\"thead\">Email Address</td>\n");
	    fprintf(cgiOut, "  </tr>\n");
	    fprintf(cgiOut, "</thead>\n");
	    fprintf(cgiOut, "\n");
	    fprintf(cgiOut, "<tfoot>\n");
	    fprintf(cgiOut, "  <tr>\n");
	    fprintf(cgiOut, "    <td class=\"tfoot\" colspan=\"4\">&nbsp;</td>\n");
	    fprintf(cgiOut, "  </tr>\n");
	    fprintf(cgiOut, "</tfoot>\n");
	    fprintf(cgiOut, "\n");
	    fprintf(cgiOut, "<tbody>\n");
	    
	    curr_id=allUsers[count];
	    while (curr_id != LAST_ID_IN_ARRAY) {
		if(isUserLibrarian(curr_id) == isLib) {
		  char *userCode=getUserCode(curr_id);
		  char *userName=getUserName(curr_id);
		  char *userEmail=getUserEmail(curr_id);

		    fprintf(cgiOut, "  <tr>\n");
		    fprintf(cgiOut, "    <td>");
		    userLink("", curr_id, userCode, cgiOut);
		    fprintf(cgiOut, "    </td>\n");
		    fprintf(cgiOut, "    <td>%s</td>\n", userName);
		    fprintf(cgiOut, "    <td>%s</td>\n", userEmail);
		    fprintf(cgiOut, "  </tr>\n");

		    free(userCode);
		    free(userName);
		    free(userEmail);
		}
		
		count++;
		curr_id=allUsers[count];
	    }
	    
	    fprintf(cgiOut, "</tbody>\n");
	    fprintf(cgiOut, "\n");
	    fprintf(cgiOut, "</table>\n");
	}
	
        free(allUsers);
    }
}

static void printSpecificUser(int userid) {
    fprintf(cgiOut, "<div class=\"head1\">View User [%d]</div>", userid);

    fprintf(cgiOut, "<table>\n");
    fprintf(cgiOut, "<tbody>\n");
    fprintf(cgiOut, "  <tr>\n");
    fprintf(cgiOut, "    <td class=\"describe\">User Code: </td>\n");
    fprintf(cgiOut, "  </tr>\n");
    fprintf(cgiOut, "  <tr>\n");
    fprintf(cgiOut, "    <td class=\"field\">%s</td>\n", getUserCode(userid));
    fprintf(cgiOut, "  </tr>\n");
    fprintf(cgiOut, "  <tr>\n");
    fprintf(cgiOut, "    <td class=\"describe\">User Name: </td>\n");
    fprintf(cgiOut, "  </tr>\n");
    fprintf(cgiOut, "  <tr>\n");
    fprintf(cgiOut, "    <td class=\"field\">%s</td>\n", getUserName(userid));
    fprintf(cgiOut, "  </tr>\n");
    fprintf(cgiOut, "  <tr>\n");
    fprintf(cgiOut, "    <td class=\"describe\">Email Address: </td>\n");
    fprintf(cgiOut, "  </tr>\n");
    fprintf(cgiOut, "  <tr>\n");
    fprintf(cgiOut, "    <td class=\"field\">%s</td>\n", getUserEmail(userid));
    fprintf(cgiOut, "  </tr>\n");
    fprintf(cgiOut, "  <tr>\n");
    fprintf(cgiOut, "    <td class=\"describe\">Is A Librarian: </td>\n");
    fprintf(cgiOut, "  </tr>\n");
    fprintf(cgiOut, "  <tr>\n");
    if(isUserLibrarian(userid) == TRUE) {
	fprintf(cgiOut, "    <td class=\"field\">Yes</td>\n");
    }
    else {
	fprintf(cgiOut, "    <td class=\"field\">No</td>\n");
    }
    fprintf(cgiOut, "  </tr>\n");
    fprintf(cgiOut, "</tbody>\n");
    fprintf(cgiOut, "</table>\n");

    fprintf(cgiOut, "<hr />\n");

    fprintf(cgiOut, "User has <b>%d</b> albums on loan<br />\n", getLoansByUserCount(userid, FALSE));
    fprintf(cgiOut, "<a href=\"./?page=loan&amp;func=view&amp;userid=%d&amp;hash=%d\">View users borrowing history</a>\n", userid, _currUserLogon);

    fprintf(cgiOut, "<hr />\n");

    fprintf(cgiOut, "<a href=\"./?page=usercomment&amp;userid=%d&amp;hash=%d\">View Comments written by/about user</a>\n", userid, _currUserLogon);

    if(isUserLibrarian(_currUserLogon) == TRUE) {
      fprintf(cgiOut, "<br /><a href=\"./?page=usercomment&amp;func=add&amp;userid=%d&amp;hash=%d\">Add Comment about this user</a>\n", userid, _currUserLogon);
    }
}
