#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "cgic.h"
#include "globals.h"
#include "../shared/defines.h"

static void doAddLoan(void);
static void doReturnLoan(void);
static void doViewLoan(void);

static int processAddForm(void);
static void printAddForm(void);

static int processReturnForm(void);

static void printAllLoansByUser(int);
static void printCurrentLoansByUser(int);
static void printPreviousLoansByUser(int);

static void printAllLoansByAlbum(int);
static void printCurrentLoanByAlbum(int);
static void printPreviousLoansByAlbum(int);

void printLoan(void) {
    int result=0;
    char func[MAXSIZE_PAGENAME]={'\0'};

    /* if func field is set */
    result = cgiFormStringNoNewlines("func", func, MAXSIZE_PAGENAME);
    if(result != cgiFormSuccess || func == NULL) {
	/* Some sort of failure */
	fprintf(cgiOut, "Invalid request\n");
	return;
    }

    if(strncmp(func, "add", MAXSIZE_PAGENAME) == 0) {
	/* Do add loan etc */
	doAddLoan();
    }
    else if(strncmp(func, "return", MAXSIZE_PAGENAME) == 0) {
	/* Do add return etc */
	doReturnLoan();
    }
    else if(strncmp(func, "view", MAXSIZE_PAGENAME) == 0) {
	/* Do view loan etc */
	doViewLoan();
    }
    else {
	/* Invalid function */
	fprintf(cgiOut, "Invalid request\n");
    }
}

static void doAddLoan(void) {
    int result=0;
    Boolean isAdding=-1;

    fprintf(cgiOut, "<div class=\"head1\">Borrowing an Album</div>\n");

    /* if adding field is set */
    result=cgiFormIntegerBounded("adding", &isAdding, FALSE, TRUE, FALSE);
    if(result != cgiFormSuccess) {
	/* Some sort of failure */
	isAdding=FALSE;
    }

    if(isAdding) {
	/* The curr data is ready for processing */
	int newloanid=processAddForm();
	if(newloanid != -1) {
	    /* Loan added ok */
	    const char *albumtitle=getAlbumTitle(getLoanAlbum(newloanid));
	    
	    fprintf(cgiOut, "Borrowing successful<br />\n");
	    fprintf(cgiOut, "<a href=\"./?page=album&amp;albumid=%d&hash=%d\">[View Info about &quot;%s&quot;]</a><br />\n", getLoanAlbum(newloanid), _currUserLogon, getAlbumTitle(getLoanAlbum(newloanid)));
	    if(isUserLibrarian(_currUserLogon) == TRUE) {
		fprintf(cgiOut, "<a href=\"./?page=loan&amp;func=view&amp;albumid=%d&hash=%d\">[View Borrowing History of &quot;%s&quot;]</a><br />\n", getLoanAlbum(newloanid), _currUserLogon, albumtitle);
	    }
	    fprintf(cgiOut, "<a href=\"./?page=loan&amp;func=view&amp;userid=%d&hash=%d\">[View your Borrowing History]</a>\n", _currUserLogon, _currUserLogon);
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
    int result=0;
    int newLoanid=-1;
    int albumid=-1;

    result = cgiFormInteger("albid", &albumid, -1);
    if(result != cgiFormSuccess || albumid == -1) {
	return -1;
    }
    
    newLoanid=addLoan(albumid, _currUserLogon);
    if(newLoanid < 0) {
	return -1;
    }
    return newLoanid;
}

static void printAddForm(void) {
    int *allAlbums=getAlbums();
    int curr_id=0;
    int count=0;

    if(allAlbums == NULL) {
	fprintf(cgiOut, "<div class=\"head1\">Error retrieving all Albums.</div>");
	return;
    }

    fprintf(cgiOut, "<form method=\"get\" action=\"./\">\n");
    fprintf(cgiOut, "<table>\n");
    fprintf(cgiOut, "    <input type=\"hidden\" name=\"page\" value=\"loan\" />\n");
    fprintf(cgiOut, "    <input type=\"hidden\" name=\"func\" value=\"add\" />\n");
    fprintf(cgiOut, "    <input type=\"hidden\" name=\"adding\" value=\"%d\" />\n", TRUE);
    fprintf(cgiOut, "    <input type=\"hidden\" name=\"hash\" value=\"%d\" />\n", _currUserLogon);
    fprintf(cgiOut, "<tbody>\n");
    fprintf(cgiOut, "  <tr>\n");
    fprintf(cgiOut, "    <td class=\"describe\"><label for=\"albtitle\" title=\"Album Title\">Album Title: </label></td>\n");
    fprintf(cgiOut, "  </tr>\n");
    fprintf(cgiOut, "  <tr>\n");
    fprintf(cgiOut, "    <td class=\"field\">\n");
    fprintf(cgiOut, "<select id=\"albtitle\" name=\"artist\" size=\"5\">\n");

    curr_id=allAlbums[count];
    while (curr_id != LAST_ID_IN_ARRAY) {
	if(getAlbumCurrentLoan(curr_id) == E_NOLOAN) {
	    fprintf(cgiOut, "  <option value=\"%d\">%s</option>\n", curr_id, getAlbumTitle(curr_id));
	}
	count++;
	curr_id=allAlbums[count];
    }

    free(allAlbums);

    fprintf(cgiOut, "</select>\n");
    fprintf(cgiOut, "    </td>\n");
    fprintf(cgiOut, "  </tr>\n");
    fprintf(cgiOut, "\n");
    fprintf(cgiOut, "  <tr>\n");
    fprintf(cgiOut, "    <td><input type=\"submit\" value=\"Borrow Album\" /></td>\n");
    fprintf(cgiOut, "  </tr>\n");
    fprintf(cgiOut, "</tbody>\n");
    fprintf(cgiOut, "</table>\n");
    fprintf(cgiOut, "</form>\n");

}

static void doReturnLoan(void) {
    int result=0;
    Boolean isReturning=-1;

    fprintf(cgiOut, "<div class=\"head1\">Returning an Album</div>\n");

    /* if adding field is set */
    result=cgiFormIntegerBounded("returning", &isReturning, FALSE, TRUE, FALSE);
    if(result != cgiFormSuccess || isReturning == FALSE) {
	/* Some sort of failure */
	fprintf(cgiOut, "Invalid request\n");
    }

    if(isReturning) {
	/* The curr data is ready for processing */
	int loanid=processReturnForm();
	if(loanid != -1) {
	    /* Album added ok */
	    const char* albumtitle=getAlbumTitle(getLoanAlbum(loanid));
	    
	    fprintf(cgiOut, "Album returned successfully<br />\n");
	    fprintf(cgiOut, "<a href=\"./?page=album&amp;albumid=%d&hash=%d\">[View Info about &quot;%s&quot;]</a><br />\n", getLoanAlbum(loanid), _currUserLogon, albumtitle);
	    if(isUserLibrarian(_currUserLogon) == TRUE) {
		fprintf(cgiOut, "<a href=\"./?page=loan&amp;func=view&amp;albumid=%d&hash=%d\">[View Borrowing History of &quot;%s&quot;]</a><br />\n", getLoanAlbum(loanid), _currUserLogon, albumtitle);
	    }
	    fprintf(cgiOut, "<a href=\"./?page=loan&amp;func=view&amp;userid=%d&hash=%d\">[View your Borrowing History]</a>\n", _currUserLogon, _currUserLogon);
	}
	else {
	    /* Some sort of failure */
	    fprintf(cgiOut, "Returning failed\n");
	}
    }
}

static int processReturnForm(void) {
    int result=0;
    int success=-1;
    int loanid=-1;

    result = cgiFormInteger("loanid", &loanid, -1);
    if(result != cgiFormSuccess || loanid == -1) {
	success=E_FORM;
    }
    else {
	if(getLoanExists(loanid) == FALSE) {
	    success=E_NOLOAN;
	}
	else {
	    if(getLoanUser(loanid) != _currUserLogon) {
		fprintf(cgiOut, "You cannot return this album; It is already on loan to ");
		userLink("", getLoanUser(loanid), getUserName(getLoanUser(loanid)), cgiOut);
		fprintf(cgiOut, "<br />\n");
	    }
	    else {
		if(isLoanReturned(loanid) == TRUE) {
		    success=ALREADY_ADDED;
		}
		else {
		    success=addLoanReturned(loanid);
		}
	    }
	}
    }
    
    return success;
}

static void doViewLoan(void) {
    int result=0;
    int userid=0;
    int albumid=0;

    /* check for userid field */
    result = cgiFormInteger("userid", &userid, -1);
    if(result != cgiFormSuccess || userid == -1) {
	/* Some sort of failure */
	/* Check for albumid field */
	result = cgiFormInteger("albumid", &albumid, -1);
	if(result != cgiFormSuccess || albumid == -1) {
	    /* Nothing specified */
	    fprintf(cgiOut, "No id specified\n");
	    return;
	}
	else {
	    /* Check album exists */
	    if(getAlbumExists(albumid) == TRUE) {
		/* Check privileges */
		if(isUserLibrarian(_currUserLogon) == TRUE) {
		    /* Will use albumid */
		    printAllLoansByAlbum(albumid);
		}
		else {
		    fprintf(cgiOut, "You can not view the borrowing history of the Album [%d]\n", albumid);
		}
	    }
	    else {
		fprintf(cgiOut, "Album [%d] does not exist in the database\n", albumid);
	    }
	}
    }
    else {
	/* Check user exists */
	if(getUserExists(userid) == TRUE) {
	    /* Check privileges */
	    if(userid == _currUserLogon || isUserLibrarian(_currUserLogon) == TRUE) {
		/* Will use userid */
		printAllLoansByUser(userid);
	    }
	    else {
		fprintf(cgiOut, "You can not view the borrowing history of the User [%d]\n", userid);
	    }
	}
	else {
	    fprintf(cgiOut, "User [%d] does not exist in the database\n", userid);
	}
    }
}

static void printAllLoansByUser(int userid) {
    fprintf(cgiOut, "<div class=\"head1\">Viewing Users Borrowing History for %s</div>", getUserName(userid));

    fprintf(cgiOut, "<b>Current Loans: </b><br />\n");
    printCurrentLoansByUser(userid);

    fprintf(cgiOut, "<hr />\n");
    
    fprintf(cgiOut, "<b>Previous Loans: </b><br />\n");    
    printPreviousLoansByUser(userid);
    fprintf(cgiOut, "<hr /><a href=\"./?page=user&amp;userid=%d&amp;hash=%d\">Back to User page</a>\n", userid, _currUserLogon);
}

static void printCurrentLoansByUser(int userid) {
    int *allLoans=NULL;
    int curr_id=0;
    int count=0;

    allLoans=getLoansByUser(userid, FALSE);

    if(allLoans == NULL) {
	fprintf(cgiOut, "<div class=\"head1\">Error retrieving all Loans</div>");
    }
    else {
	if(getLoansByUserCount(userid, FALSE) == 0) {
		fprintf(cgiOut, "No albums on loan\n");
	}
	else {
	    fprintf(cgiOut, "<table border=\"1\">\n");
	    fprintf(cgiOut, "\n");
	    fprintf(cgiOut, "<thead>\n");
	    fprintf(cgiOut, "  <tr>\n");
	    fprintf(cgiOut, "    <td class=\"thead\">Album Title</td>\n");
	    fprintf(cgiOut, "    <td class=\"thead\">Status</td>\n");
	    fprintf(cgiOut, "    <td class=\"thead\">Start</td>\n");
	    if(userid == _currUserLogon) {
		fprintf(cgiOut, "    <td class=\"thead\">Return?</td>\n");
	    }
	    fprintf(cgiOut, "  </tr>\n");
	    fprintf(cgiOut, "</thead>\n");
	    fprintf(cgiOut, "\n");
	    fprintf(cgiOut, "<tfoot>\n");
	    fprintf(cgiOut, "  <tr>\n");
	    if(userid == _currUserLogon) {
		fprintf(cgiOut, "    <td class=\"tfoot\" colspan=\"4\">&nbsp;</td>\n");
	    }
	    else {
		fprintf(cgiOut, "    <td class=\"tfoot\" colspan=\"3\">&nbsp;</td>\n");
	    }
	    fprintf(cgiOut, "  </tr>\n");
	    fprintf(cgiOut, "</tfoot>\n");
	    fprintf(cgiOut, "\n");
	    fprintf(cgiOut, "<tbody>\n");

	    curr_id=allLoans[count];
	    while (curr_id != LAST_ID_IN_ARRAY) {
		fprintf(cgiOut, "  <tr>\n");
		fprintf(cgiOut, "    <td>");
		fprintf(cgiOut, "<a href=\"./?page=album&amp;albumid=%d&amp;hash=%d\">%s</a>", getLoanAlbum(curr_id), _currUserLogon, getAlbumTitle(getLoanAlbum(curr_id)));
		fprintf(cgiOut, "    </td>\n");
		fprintf(cgiOut, "    <td>On Loan</td>\n");
		
		fprintf(cgiOut, "    <td>");
		printTime(getLoanTimeIn(curr_id), cgiOut);
		fprintf(cgiOut, "</td>\n");
		
		if(getLoanUser(curr_id) == _currUserLogon) {
		    fprintf(cgiOut, "    <td>");
		    fprintf(cgiOut, "<form method=\"get\" action=\"./\" style=\"display: inline;\">\n");
		    fprintf(cgiOut, "  <div>\n");
		    fprintf(cgiOut, "    <input type=\"hidden\" name=\"page\" value=\"loan\" />\n");
		    fprintf(cgiOut, "    <input type=\"hidden\" name=\"func\" value=\"return\" />\n");
		    fprintf(cgiOut, "    <input type=\"hidden\" name=\"returning\" value=\"%d\" />\n", TRUE);
		    fprintf(cgiOut, "    <input type=\"hidden\" name=\"loanid\" value=\"%d\" />\n", curr_id);
		    fprintf(cgiOut, "    <input type=\"hidden\" name=\"hash\" value=\"%d\" />\n", _currUserLogon);
		    fprintf(cgiOut, "    <input type=\"submit\" value=\"Return Album\" />\n");
		    fprintf(cgiOut, "  </div>\n");
		    fprintf(cgiOut, "</form>\n");
		    fprintf(cgiOut, "</td>\n");
		}
		
		fprintf(cgiOut, "  </tr>\n");
		
		count++;
		curr_id=allLoans[count];
	    }
	    
	    fprintf(cgiOut, "</tbody>\n");
	    fprintf(cgiOut, "\n");
	    fprintf(cgiOut, "</table>\n");
	}
	
	free(allLoans);
    }

}

static void printPreviousLoansByUser(int userid) {
    int *allLoans=NULL;
    int curr_id=0;
    int count=0;

    allLoans=getLoansByUser(userid, TRUE);

    if(allLoans == NULL) {
	fprintf(cgiOut, "<div class=\"head1\">Error retrieving all Loans</div>");
    }
    else {
	if(getLoansByUserCount(userid, TRUE) == 0) {
	    fprintf(cgiOut, "No loan history\n");
	}
	else {
	    fprintf(cgiOut, "<table border=\"1\">\n");
	    fprintf(cgiOut, "\n");
	    fprintf(cgiOut, "<thead>\n");
	    fprintf(cgiOut, "  <tr>\n");
	    fprintf(cgiOut, "    <td class=\"thead\">Album Title</td>\n");
	    fprintf(cgiOut, "    <td class=\"thead\">Status</td>\n");
	    fprintf(cgiOut, "    <td class=\"thead\">Start</td>\n");
	    fprintf(cgiOut, "    <td class=\"thead\">End</td>\n");
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

	    curr_id=allLoans[count];
	    while (curr_id != LAST_ID_IN_ARRAY) {
		fprintf(cgiOut, "  <tr>\n");
		fprintf(cgiOut, "    <td>");
		fprintf(cgiOut, "<a href=\"./?page=album&amp;albumid=%d&amp;hash=%d\">%s</a>", getLoanAlbum(curr_id), _currUserLogon, getAlbumTitle(getLoanAlbum(curr_id)));
		fprintf(cgiOut, "    </td>\n");
		fprintf(cgiOut, "    <td>Returned</td>\n");
		
		fprintf(cgiOut, "    <td>");
		printTime(getLoanTimeIn(curr_id), cgiOut);
		fprintf(cgiOut, "</td>\n");
		
		fprintf(cgiOut, "    <td>");
		printTime(getLoanTimeOut(curr_id), cgiOut);
		fprintf(cgiOut, "</td>\n");
		
		fprintf(cgiOut, "  </tr>\n");
		
		count++;
		curr_id=allLoans[count];
	    }
	    
	    fprintf(cgiOut, "</tbody>\n");
	    fprintf(cgiOut, "\n");
	    fprintf(cgiOut, "</table>\n");
	}
	    
	free(allLoans);
    }
}

static void printAllLoansByAlbum(int albumid) {
    fprintf(cgiOut, "<div class=\"head1\">Viewing Albums Borrowing History for %s</div>", getAlbumTitle(albumid));

    printCurrentLoanByAlbum(albumid);

    fprintf(cgiOut, "<hr />\n");

    fprintf(cgiOut, "<b>Previous Loans: </b><br />\n");
    printPreviousLoansByAlbum(albumid);
    fprintf(cgiOut, "<hr /><a href=\"./?page=album&amp;albumid=%d&amp;hash=%d\">Back to Album page</a>\n", albumid, _currUserLogon);
}

static void printCurrentLoanByAlbum(int albumid) {
    /* is album on loan */
    int currLoan=getAlbumCurrentLoan(albumid);
    if(currLoan != E_NOLOAN) {
	int tempUserID=0;
	tempUserID=getLoanUser(currLoan);

	if(tempUserID == _currUserLogon) {
	    fprintf(cgiOut, "You have this album on loan<br />\n");
	    fprintf(cgiOut, "<form method=\"get\" action=\"./\">\n");
	    fprintf(cgiOut, "  <p>\n");
	    fprintf(cgiOut, "    <input type=\"hidden\" name=\"page\" value=\"loan\" />\n");
	    fprintf(cgiOut, "    <input type=\"hidden\" name=\"func\" value=\"return\" />\n");
	    fprintf(cgiOut, "    <input type=\"hidden\" name=\"returning\" value=\"%d\" />\n", TRUE);
	    fprintf(cgiOut, "    <input type=\"hidden\" name=\"loanid\" value=\"%d\" />\n", currLoan);
	    fprintf(cgiOut, "    <input type=\"hidden\" name=\"hash\" value=\"%d\" />\n", _currUserLogon);
	    fprintf(cgiOut, "    <input type=\"submit\" value=\"Return Album\" />\n");
	    fprintf(cgiOut, "  </p>\n");
	    fprintf(cgiOut, "</form>\n");
	}
	else {
	    fprintf(cgiOut, "Album is on loan ");
	    if(isUserLibrarian(_currUserLogon) == TRUE) {
		fprintf(cgiOut, "to <b>");
		userLink("", tempUserID, getUserName(tempUserID), cgiOut);
		fprintf(cgiOut, "</b><br />\n");
		fprintf(cgiOut, "[Taken at ");
		printTime(getLoanTimeIn(currLoan), cgiOut);
		fprintf(cgiOut, "]\n");
	    }
	}
    }
    else {
	fprintf(cgiOut, "Album is not on loan\n");

	fprintf(cgiOut, "<form method=\"get\" action=\"./\">\n");
	fprintf(cgiOut, "  <p>\n");
	fprintf(cgiOut, "    <input type=\"hidden\" name=\"page\" value=\"loan\" />\n");
	fprintf(cgiOut, "    <input type=\"hidden\" name=\"func\" value=\"add\" />\n");
	fprintf(cgiOut, "    <input type=\"hidden\" name=\"adding\" value=\"%d\" />\n", TRUE);
	fprintf(cgiOut, "    <input type=\"hidden\" name=\"albid\" value=\"%d\" />\n", albumid);
	fprintf(cgiOut, "    <input type=\"hidden\" name=\"hash\" value=\"%d\" />\n", _currUserLogon);
	fprintf(cgiOut, "    <input type=\"submit\" value=\"Borrow Album\" />\n");
	fprintf(cgiOut, "  </p>\n");
	fprintf(cgiOut, "</form>\n");
    }
}

static void printPreviousLoansByAlbum(int albumid) {    
    /* view borrowing history */
    int *allLoans=NULL;
    int curr_id=0;
    int count=0;
	
    allLoans=getLoansByAlbum(albumid, TRUE);

    if(allLoans == NULL) {
	fprintf(cgiOut, "<div class=\"head1\">Error retrieving all Loans</div>");
    }
    else {
	if(getLoansByAlbumCount(albumid, TRUE) == 0) {
	    fprintf(cgiOut, "No loan history\n");
	}
	else {
	    fprintf(cgiOut, "<table border=\"1\">\n");
	    fprintf(cgiOut, "\n");
	    fprintf(cgiOut, "<thead>\n");
	    fprintf(cgiOut, "  <tr>\n");
	    fprintf(cgiOut, "    <td class=\"thead\">User Name</td>\n");
	    fprintf(cgiOut, "    <td class=\"thead\">Status</td>\n");
	    fprintf(cgiOut, "    <td class=\"thead\">Start</td>\n");
	    fprintf(cgiOut, "    <td class=\"thead\">End</td>\n");
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
	    
	    curr_id=allLoans[count];
	    while (curr_id != LAST_ID_IN_ARRAY) {
		fprintf(cgiOut, "  <tr>\n");
		fprintf(cgiOut, "    <td>");
		userLink("", getLoanUser(curr_id), getUserName(getLoanUser(curr_id)), cgiOut);
		fprintf(cgiOut, "    </td>\n");
		fprintf(cgiOut, "    <td>Returned</td>\n");
		
		fprintf(cgiOut, "    <td>");
		printTime(getLoanTimeIn(curr_id), cgiOut);
		fprintf(cgiOut, "</td>\n");
		
		fprintf(cgiOut, "    <td>");
		printTime(getLoanTimeOut(curr_id), cgiOut);
		fprintf(cgiOut, "</td>\n");
		
		fprintf(cgiOut, "  </tr>\n");
		
		count++;
		curr_id=allLoans[count];
	    }
	    
	    fprintf(cgiOut, "</tbody>\n");
	    fprintf(cgiOut, "\n");
	    fprintf(cgiOut, "</table>\n");
	}
	
	free(allLoans);
    }
}
