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
static void printAllLoansByAlbum(int);

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
    if(strncmp(func, "return", MAXSIZE_PAGENAME) == 0) {
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

    fprintf(cgiOut, "<div class=\"head1\">Adding New Loan</div>\n");

    /* if adding field is set */
    result=cgiFormIntegerBounded("adding", &isAdding, FALSE, TRUE, FALSE);
    if(result != cgiFormSuccess) {
	/* Some sort of failure */
	isAdding=FALSE;
    }

    if(isAdding) {
	/* The curr data is ready for processing */
	int newalbumid=processAddForm();
	if(newalbumid != -1) {
	    /* Album added ok */
	    fprintf(cgiOut, "Adding successful\n");
	    fprintf(cgiOut, "<a href=\"./?page=album&amp;albumid=%d&hash=%d\">[View Album]</a>", newalbumid, _currUserLogon);
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
    int *allArtists=getArtists();
    int curr_id=0;
    int count=0;

    if(allArtists == NULL) {
	fprintf(cgiOut, "<div class=\"head1\">Error retrieving all Artists</div>");
	return;
    }

    fprintf(cgiOut, "<form method=\"POST\" action=\"./\">\n");
    fprintf(cgiOut, "<table>\n");
    fprintf(cgiOut, "    <input type=\"hidden\" name=\"page\" value=\"album\" />\n");
    fprintf(cgiOut, "    <input type=\"hidden\" name=\"func\" value=\"add\" />\n");
    fprintf(cgiOut, "    <input type=\"hidden\" name=\"adding\" value=\"%d\" />\n", TRUE);
    fprintf(cgiOut, "    <input type=\"hidden\" name=\"hash\" value=\"%d\" />\n", _currUserLogon);
    fprintf(cgiOut, "<tbody>\n");
    fprintf(cgiOut, "  <tr>\n");
    fprintf(cgiOut, "    <td class=\"describe\"><label for=\"title\" title=\"Album Title\">Album Title: </label></td>\n");
    fprintf(cgiOut, "  </tr>\n");
    fprintf(cgiOut, "  <tr>\n");
    fprintf(cgiOut, "    <td class=\"field\"><input type=\"text\" id=\"title\" name=\"title\" size=\"%d\"/></td>\n", MAXSIZE_ALBUMTITLE);
    fprintf(cgiOut, "  </tr>\n");
    fprintf(cgiOut, "  <tr>\n");
    fprintf(cgiOut, "    <td class=\"describe\"><label for=\"artist\" title=\"Artist Name\">Artist Name: </label></td>\n");
    fprintf(cgiOut, "  </tr>\n");
    fprintf(cgiOut, "  <tr>\n");
    fprintf(cgiOut, "    <td class=\"field\">\n");
    fprintf(cgiOut, "<select id=\"artist\" name=\"artist\" size=\"5\">\n");

    curr_id=allArtists[count];
    while (curr_id != LAST_ID_IN_ARRAY) {
	fprintf(cgiOut, "  <option value=\"%d\">%s</option>\n", curr_id, getArtistName(curr_id));
	count++;
	curr_id=allArtists[count];
    }

    free(allArtists);

    fprintf(cgiOut, "</select>\n");
    fprintf(cgiOut, "    </td>\n");
    fprintf(cgiOut, "  </tr>\n");
    fprintf(cgiOut, "\n");
    fprintf(cgiOut, "  <tr>\n");
    fprintf(cgiOut, "    <td><input type=\"submit\" value=\"Add Album\" /></td>\n");
    fprintf(cgiOut, "  </tr>\n");
    fprintf(cgiOut, "</tbody>\n");
    fprintf(cgiOut, "</table>\n");
    fprintf(cgiOut, "</form>\n");

}

static void doReturnLoan(void) {
    int result=0;
    Boolean isReturning=-1;

    fprintf(cgiOut, "<div class=\"head1\">Returning Album</div>\n");

    /* if adding field is set */
    result=cgiFormIntegerBounded("returning", &isReturning, FALSE, TRUE, FALSE);
    if(result != cgiFormSuccess || isReturning == FALSE) {
	/* Some sort of failure */
	fprintf(cgiOut, "Invalid request\n");
    }

    if(isReturning) {
	/* The curr data is ready for processing */
	int success=processReturnForm();
	if(success != -1) {
	    /* Album added ok */
	    fprintf(cgiOut, "Album returned successful\n");
	}
	else {
	    /* Some sort of failure */
	    fprintf(cgiOut, "Returning failed\n");
	}
    }
}

static int processReturnForm(void) {
    return -1;
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
	    /* Check privileges */
	    if(isUserLibrarian(_currUserLogon) == TRUE) {
		/* Will use albumid */
		printAllLoansByAlbum(albumid);
	    }
	    else {
		fprintf(cgiOut, "You can not view the borrowing history of the Album [%d]\n", albumid);
	    }
	}
    }
    else {
	/* Check privileges */
	if(userid == _currUserLogon || isUserLibrarian(_currUserLogon) == TRUE) {
	    /* Will use userid */
	    printAllLoansByUser(userid);
	}
	else {
	    fprintf(cgiOut, "You can not view the borrowing history of the User [%d]\n", userid);
	}
    }
}

static void printAllLoansByUser(int userid) {
    int *allLoans=NULL;
    int curr_id=0;
    int count=0;

    fprintf(cgiOut, "<div class=\"head1\">Viewing All Loans for <b>%s</b></div>", getUserName(userid));

    allLoans=getLoansByUser(userid);

    if(allLoans == NULL) {
	fprintf(cgiOut, "<div class=\"head1\">Error retrieving all Loans</div>");
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

	if(getLoansByUserCount(userid) == 0) {
	    fprintf(cgiOut, "  <tr>\n");
	    fprintf(cgiOut, "    <td colspan=\"4\">No loan history</td>\n");
	    fprintf(cgiOut, "  </tr>\n");
	}

	curr_id=allLoans[count];
        while (curr_id != LAST_ID_IN_ARRAY) {
	    fprintf(cgiOut, "  <tr>\n");
	    fprintf(cgiOut, "    <td>");
	    fprintf(cgiOut, "<a href=\"./?page=album&amp;albumid=%d&amp;hash=%d\">%s</a>", getLoanAlbum(curr_id), _currUserLogon, getAlbumTitle(getLoanAlbum(curr_id)));
	    fprintf(cgiOut, "    </td>\n");
	    if(isLoanReturned(curr_id) == FALSE) {
		fprintf(cgiOut, "    <td>On Loan</td>\n");
	    }
	    else {
		fprintf(cgiOut, "    <td>Returned</td>\n");
	    }

	    fprintf(cgiOut, "    <td>");
	    printTime(getLoanTimeIn(curr_id), cgiOut);
	    fprintf(cgiOut, "</td>\n");

	    if(getLoanTimeOut(curr_id) == -1) {
		fprintf(cgiOut, "    <td>N/A</td>\n");
	    }
	    else {
		fprintf(cgiOut, "    <td>");
		printTime(getLoanTimeOut(curr_id), cgiOut);
		fprintf(cgiOut, "</td>\n");
	    }

	    fprintf(cgiOut, "  </tr>\n");

	    count++;
	    curr_id=allLoans[count];
	}
	
	fprintf(cgiOut, "</tbody>\n");
	fprintf(cgiOut, "\n");
	fprintf(cgiOut, "</table>\n");

	free(allLoans);
    }
}

static void printAllLoansByAlbum(int albumid) {
    /* view borrowing history */
    int *allLoans=NULL;
    int curr_id=0;
    int count=0;
	
    fprintf(cgiOut, "<div class=\"head1\">Borrowing History for %s</div>", getAlbumTitle(albumid));

    allLoans=getLoansByAlbum(albumid);

    if(allLoans == NULL) {
	fprintf(cgiOut, "<div class=\"head1\">Error retrieving all Loans</div>");
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

	if(getLoansByAlbumCount(albumid) == 0) {
	    fprintf(cgiOut, "  <tr>\n");
	    fprintf(cgiOut, "    <td colspan=\"4\">No loan history</td>\n");
	    fprintf(cgiOut, "  </tr>\n");
	}

	curr_id=allLoans[count];
	while (curr_id != LAST_ID_IN_ARRAY) {
	    fprintf(cgiOut, "  <tr>\n");
	    fprintf(cgiOut, "    <td>");
	    userLink(getLoanUser(curr_id), getUserName(getLoanUser(curr_id)), cgiOut);
	    fprintf(cgiOut, "    </td>\n");
	    if(isLoanReturned(curr_id) == FALSE) {
		fprintf(cgiOut, "    <td>On Loan</td>\n");
	    }
	    else {
		fprintf(cgiOut, "    <td>Returned</td>\n");
	    }
	    
	    fprintf(cgiOut, "    <td>");
	    printTime(getLoanTimeIn(curr_id), cgiOut);
	    fprintf(cgiOut, "</td>\n");

	    if(getLoanTimeOut(curr_id) == -1) {
		fprintf(cgiOut, "    <td>N/A</td>\n");
	    }
	    else {
		fprintf(cgiOut, "    <td>");
		printTime(getLoanTimeOut(curr_id), cgiOut);
		fprintf(cgiOut, "</td>\n");
	    }
		
	    fprintf(cgiOut, "  </tr>\n");

	    count++;
	    curr_id=allLoans[count];
	}
	
	fprintf(cgiOut, "</tbody>\n");
	fprintf(cgiOut, "\n");
	fprintf(cgiOut, "</table>\n");

	free(allLoans);
    }
    
}
