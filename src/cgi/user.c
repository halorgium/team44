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
    /* Do add album etc */
    doAddUser();
  }
  else {
    /* Default */
    /* Do view album etc */
    doViewUser();
  }
}

static void doAddUser(void) {
  int result=0;
  Boolean isAdding=-1;

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
    if(newuserid != -1) {
      /* User added ok */
      fprintf(cgiOut, "Adding successful<br />\n");
      fprintf(cgiOut, "<a href=\"./?page=user&amp;userid=%d&amp;hash=%d\">[View User]</a>", newuserid, _currUserLogon);
    }
    else {
      /* Some sort of failure */
      fprintf(cgiOut, "Adding failed\n");
    }
  }
  else {
    /* Need to print form */
    printAddForm();
  }
}

static int processAddForm(void) {
  return -1;
}

static void printAddForm(void) {
  fprintf(cgiOut, "<form method=\"post\" action=\"./\">\n");
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
  int *allUsers=NULL;
    int curr_id=0;
    int count=0;

    fprintf(cgiOut, "<div class=\"head1\">Viewing All Users</div>");

    allUsers=getUsers();

    if(allUsers == NULL) {
      fprintf(cgiOut, "<div class=\"head1\">Error retrieving all Users</div>");
    }
    else {
      fprintf(cgiOut, "<table border=\"1\">\n");
      fprintf(cgiOut, "\n");
      fprintf(cgiOut, "<thead>\n");
      fprintf(cgiOut, "  <tr>\n");
      fprintf(cgiOut, "    <td class=\"thead\">User Code</td>\n");
      fprintf(cgiOut, "    <td class=\"thead\">User Name</td>\n");
      fprintf(cgiOut, "    <td class=\"thead\">Email Address</td>\n");
      fprintf(cgiOut, "    <td class=\"thead\">Is Librarian?</td>\n");
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
	  fprintf(cgiOut, "  <tr>\n");
	  fprintf(cgiOut, "    <td>");
	  userLink(curr_id, getUserCode(curr_id), cgiOut);
	  fprintf(cgiOut, "    </td>\n");
	  fprintf(cgiOut, "    <td>%s</td>\n", getUserName(curr_id));
	  fprintf(cgiOut, "    <td>%s</td>\n", getUserEmail(curr_id));
	  if(isUserLibrarian(curr_id) == TRUE) {
	    fprintf(cgiOut, "    <td>Yes</td>\n");
	  }
	  else {
	    fprintf(cgiOut, "    <td>No</td>\n");
	  }
	  fprintf(cgiOut, "  </tr>\n");

	  count++;
	  curr_id=allUsers[count];
        }

	fprintf(cgiOut, "</tbody>\n");
	fprintf(cgiOut, "\n");
	fprintf(cgiOut, "</table>\n");

        free(allUsers);
    }
}

void printSpecificUser(int userid) {
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

    fprintf(cgiOut, "<a href=\"./?page=loan&amp;func=view&amp;userid=%d&amp;hash=%d\">View users borrowing history</a>\n", userid, _currUserLogon);
}
