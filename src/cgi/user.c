/*================= Preprocessor statements===============================*/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "cgic.h"
#include "globals.h"
#include "../shared/defines.h"
#include "../database/structs.h"

/*======================Function Declarations=============================*/

static void doAddUser(void);
static void doViewUser(void);

static int processAddForm(int *, userNode_t *);
static void printAddForm(Boolean, int *, userNode_t *);

static void printAllUsers(void);
static void printAllUsersByType(Boolean);

static void printSpecificUser(int);

/*======================Function Definitions=============================*/

void printUser(funcName_t func) {
    switch(func) {
    case FUNC_ADD:
	/* Do add user etc */
	doAddUser();
	break;
    default:
	/* Default */
	/* Do view user etc */
	doViewUser();
    }
}

static void doAddUser(void) {
    Boolean needAddForm=TRUE;

    int result=0;

    /* Temporary Struct to store form data */
    userNode_t *formdata=NULL;
    /* Array to store errors */
    int *errors=NULL;
    Boolean isAdding=FALSE;

    /* Check privileges of current user */
    if(isUserLibrarian(_currUserLogon) == FALSE) {
	fprintf(cgiOut, "You are not privileged to add new Users\n");
	return;
    }
    
    fprintf(cgiOut, "<div class=\"head1\">Adding New User</div>\n");

    /* if adding field is set */
    result=cgiFormIntegerBounded("adding", &isAdding, FALSE, TRUE, FALSE);
    if(result != cgiFormSuccess) {
	/* Some sort of failure */
	isAdding=FALSE;
    }

    /* Malloc space for form data */
    formdata=malloc(sizeof(userNode_t));
    if(formdata == NULL) {
      isAdding=FALSE;
    }
    else {
      Boolean formOK=FALSE;
      
      formdata->userCode=NULL;
      formdata->userName=NULL;
      formdata->emailAddress=NULL;
      formdata->isLibrarian=FALSE;
      
      /* Malloc space for error code of each field */
      errors=malloc(sizeof(int)*4);
      if(errors == NULL) {
	free(formdata);
	isAdding=FALSE;
      }
      
      /* Set errors to E_NOERROR */
      {
	int i=0;
	for(i=0; i < 4; i++) {
	  errors[i]=E_NOERROR;
	}
      }
      
      /* The curr data is ready for processing */
      formOK=processAddForm(errors, formdata);
      
      if(isAdding == TRUE && formOK == TRUE) {
	int newUserid=-1;
	
	/* All form data is good */
	/* Add user to database */
	newUserid=addUser(formdata->userCode, formdata->userName, formdata->emailAddress, formdata->isLibrarian);
	
	if(newUserid > 0) {
	  /* User added ok */
	  needAddForm=FALSE;
	  fprintf(cgiOut, "Adding successful<br />\n");
	  fprintf(cgiOut, "<a href=\"./?page=user&amp;userid=%d&amp;hash=%d\">[View User]</a><br />\n", newUserid, _currUserLogon);
	  fprintf(cgiOut, "<a href=\"./?page=user&amp;func=add&amp;hash=%d\">[Add Another User]</a>", _currUserLogon);
	}
	else {
	  /* User adding error */
	  fprintf(cgiOut, "DB Save Error<br />\n");
	}
      }
    }

    if(needAddForm == TRUE) {
      /* Need to print form */
      printAddForm(isAdding, errors, formdata);
    }

    /* Free the memory */
    if(errors != NULL) {
      free(errors);
    }
    if(formdata != NULL) {
      if(formdata->userCode != NULL) {
	free(formdata->userCode);
      }
      if(formdata->userName != NULL) {
	free(formdata->userName);
      }
      if(formdata->emailAddress != NULL) {
	free(formdata->emailAddress);
      }
      free(formdata);
    }
}

static Boolean processAddForm(int *errors, userNode_t *formdata) {
    int result=0;
    int size=-1;
    
    /* Check arguments */
    if(errors == NULL || formdata == NULL) {
      return FALSE;
    }

    /* Get user code */
    result = cgiFormStringSpaceNeeded("usrcode", &size);
    if(result != cgiFormSuccess) {
      errors[0]=E_FORM;
    }
    else if(size > MAXLEN_USERCODE+1) {
      errors[0]=E_TOOBIG;
    }
    else {
      formdata->userCode=malloc(sizeof(char)*size);
      if(formdata->userCode == NULL) {
	errors[0]=E_MALLOC_FAILED;
      }
      else {
	result = cgiFormStringNoNewlines("usrcode", formdata->userCode, size);
	if(result != cgiFormSuccess) {
	  errors[0]=E_FORM;
	}
	else if(checkString2(formdata->userCode) == FALSE) {
	  errors[0]=E_INVALID_PARAM;
	}
	else if(getUserExists(makeUserID(formdata->userCode)) == TRUE) {
	  errors[0]=ALREADY_ADDED;
	}
      }
    }
    /* Get user name */
    result = cgiFormStringSpaceNeeded("usrname", &size);
    if(result != cgiFormSuccess) {
      errors[1]=E_FORM;
    }
    else if(size > MAXLEN_USERNAME+1) {
      errors[1]=E_TOOBIG;
    }
    else {
	formdata->userName=malloc(sizeof(char)*size);
	if(formdata->userName == NULL) {
	    errors[1]=E_MALLOC_FAILED;
	}
	else {
	    result = cgiFormStringNoNewlines("usrname", formdata->userName, size);
	    if(result != cgiFormSuccess) {
	      errors[1]=E_FORM;
	    }
	    else if(checkString2(formdata->userName) == FALSE) {
	      errors[1]=E_INVALID_PARAM;
	    }
	}
    }
    /* Get email address */
    result = cgiFormStringSpaceNeeded("usremail", &size);
    if(result != cgiFormSuccess) {
	errors[2]=E_FORM;
    }
    else if(size > MAXLEN_USEREMAIL+1) {
      errors[1]=E_TOOBIG;
    }
    else {
	formdata->emailAddress=malloc(sizeof(char)*size);
	if(formdata->emailAddress == NULL) {
	    errors[2]=E_MALLOC_FAILED;
	}
	else {
	    result = cgiFormStringNoNewlines("usremail", formdata->emailAddress, size);
	    if(result != cgiFormSuccess) {
	      errors[2]=E_FORM;
	    }
	    else if(checkString2(formdata->emailAddress) == FALSE) {
	      errors[2]=E_INVALID_PARAM;
	    }
	}
    }
    /* Get isLibrarian */
    result = cgiFormCheckboxSingle("islib");
    if(result != cgiFormSuccess) {
	formdata->isLibrarian=FALSE;
    }
    else {
	formdata->isLibrarian=TRUE;
    }

    if(errors[0] != E_NOERROR ||
       errors[1] != E_NOERROR ||
       errors[2] != E_NOERROR ||
       errors[3] != E_NOERROR) {
      return FALSE;
    }
    return TRUE;
}

static void printAddForm(Boolean isAdding, int *errors, userNode_t *formdata) {
  Boolean freshForm=FALSE;

  /* Check arguments */
  if(isAdding == FALSE || errors == NULL || formdata == NULL) {
    /* Print a fresh form */
    freshForm=TRUE;
  }
  else {
    /* Process errors */

    /* User code */
    switch(errors[0]) {
    case E_NOERROR:
      break;
    case E_FORM:
      fprintf(cgiOut, "User code is empty<br />\n");
      break;
    case E_TOOBIG:
      fprintf(cgiOut, "User code is too big<br />\n");
      break;
    case E_INVALID_PARAM:
      fprintf(cgiOut, "User code is invalid<br />\n");
      break;
    case ALREADY_ADDED:
      fprintf(cgiOut, "User Code &quot;%s&quot; already exists<br />\n", formdata->userCode);
      break;
    case E_MALLOC_FAILED:
      fprintf(cgiOut, "Memory Allocation Error<br />\n");
      break;
    default:
      fprintf(cgiOut, "Unknown form error<br />\n");
      break;
    }

    /* User Name */
    switch(errors[1]) {
    case E_NOERROR:
      break;
    case E_FORM:
      fprintf(cgiOut, "User name is empty<br />\n");
      break;
    case E_TOOBIG:
      fprintf(cgiOut, "User name is too big<br />\n");
      break;
    case E_INVALID_PARAM:
      fprintf(cgiOut, "User name is invalid<br />\n");
      break;
    case E_MALLOC_FAILED:
      fprintf(cgiOut, "Memory Allocation Error<br />\n");
      break;
    default:
      fprintf(cgiOut, "Unknown form error<br />\n");
      break;
    }

    /* Email Address */
    switch(errors[2]) {
    case E_NOERROR:
      break;
    case E_FORM:
      fprintf(cgiOut, "Email Address is empty<br />\n");
      break;
    case E_TOOBIG:
      fprintf(cgiOut, "Email Address is too big<br />\n");
      break;
    case E_INVALID_PARAM:
      fprintf(cgiOut, "Email Address is invalid<br />\n");
      break;
    case E_MALLOC_FAILED:
      fprintf(cgiOut, "Memory Allocation Error<br />\n");
      break;
    default:
      fprintf(cgiOut, "Unknown form error<br />\n");
      break;
    }
  }

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
    fprintf(cgiOut, "    <td class=\"describe%s\"><label for=\"usrcode\" title=\"User Code\">User <u>C</u>ode: </label></td>\n", ((isAdding == TRUE && errors != NULL && errors[0] != E_NOERROR)?"2":""));
    fprintf(cgiOut, "  </tr>\n");
    fprintf(cgiOut, "  <tr>\n");
    fprintf(cgiOut, "    <td class=\"field\"><input type=\"text\" id=\"usrcode\" name=\"usrcode\" size=\"%d\" maxlength=\"%d\" value=\"%s\" accesskey=\"c\" /></td>\n", MAXLEN_USERCODE, MAXLEN_USERCODE, ((errors != NULL && errors[0] == E_NOERROR && formdata != NULL && formdata->userCode != NULL)?formdata->userCode:""));
    fprintf(cgiOut, "  </tr>\n");

    fprintf(cgiOut, "  <tr>\n");
    fprintf(cgiOut, "    <td class=\"describe%s\"><label for=\"usrname\" title=\"User Name\">User <u>N</u>ame: </label></td>\n", ((isAdding == TRUE && errors != NULL && errors[1] != E_NOERROR)?"2":""));
    fprintf(cgiOut, "  </tr>\n");
    fprintf(cgiOut, "  <tr>\n");
    fprintf(cgiOut, "    <td class=\"field\"><input type=\"text\" id=\"usrname\" name=\"usrname\" size=\"%d\" maxlength=\"%d\" value=\"%s\" accesskey=\"n\" /></td>\n", MAXLEN_USERNAME, MAXLEN_USERNAME, ((errors != NULL && errors[1] == E_NOERROR && formdata != NULL && formdata->userName != NULL)?formdata->userName:""));
    fprintf(cgiOut, "  </tr>\n");

    fprintf(cgiOut, "  <tr>\n");
    fprintf(cgiOut, "    <td class=\"describe%s\"><label for=\"usremail\" title=\"Email Address\"><u>E</u>mail Address: </label></td>\n", ((isAdding == TRUE && errors != NULL && errors[2] != E_NOERROR)?"2":""));
    fprintf(cgiOut, "  </tr>\n");
    fprintf(cgiOut, "  <tr>\n");
    fprintf(cgiOut, "    <td class=\"field\"><input type=\"text\" id=\"usremail\" name=\"usremail\" size=\"%d\" maxlength=\"%d\" value=\"%s\" accesskey=\"e\" /></td>\n", MAXLEN_USEREMAIL, MAXLEN_USEREMAIL, ((errors != NULL && errors[2] == E_NOERROR && formdata != NULL && formdata->emailAddress != NULL)?formdata->emailAddress:""));
    fprintf(cgiOut, "  </tr>\n");

    fprintf(cgiOut, "  <tr>\n");
    fprintf(cgiOut, "    <td class=\"field\">");
    fprintf(cgiOut, "<input type=\"checkbox\" id=\"islib\" name=\"islib\" value=\"%d\" accesskey=\"l\"%s />&nbsp;", TRUE, ((errors != NULL && errors[3] == E_NOERROR && formdata != NULL && formdata->isLibrarian == TRUE)?" checked":""));
    fprintf(cgiOut, "<label for=\"islib\" title=\"Is Librarian\">Is <u>L</u>ibrarian</label>\n");
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
    /*variables used to display data in following output*/
    char *userCode=getUserCode(userid);
    char *userName=getUserName(userid);
    char *userEmail=getUserEmail(userid);
    
    fprintf(cgiOut, "<div class=\"head1\">View User [%d]</div>", userid);

    fprintf(cgiOut, "<table>\n");
    fprintf(cgiOut, "<tbody>\n");
    fprintf(cgiOut, "  <tr>\n");
    fprintf(cgiOut, "    <td class=\"describe\">User Code: </td>\n");
    fprintf(cgiOut, "  </tr>\n");
    fprintf(cgiOut, "  <tr>\n");
    fprintf(cgiOut, "    <td class=\"field\">%s</td>\n", userCode);
    fprintf(cgiOut, "  </tr>\n");
    fprintf(cgiOut, "  <tr>\n");
    fprintf(cgiOut, "    <td class=\"describe\">User Name: </td>\n");
    fprintf(cgiOut, "  </tr>\n");
    fprintf(cgiOut, "  <tr>\n");
    fprintf(cgiOut, "    <td class=\"field\">%s</td>\n", userName);
    fprintf(cgiOut, "  </tr>\n");
    fprintf(cgiOut, "  <tr>\n");
    fprintf(cgiOut, "    <td class=\"describe\">Email Address: </td>\n");
    fprintf(cgiOut, "  </tr>\n");
    fprintf(cgiOut, "  <tr>\n");
    fprintf(cgiOut, "    <td class=\"field\">%s</td>\n", userEmail);
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

    free(userCode);
    free(userName);
    free(userEmail);
}
