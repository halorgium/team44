#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "cgic.h"
#include "globals.h"
#include "../shared/structs.h"
#include "../shared/defines.h"
#include "../shared/lib.h"

static void doAddLoan(void);
static void doViewLoan(void);

static int processAddForm(void);
static void printAddForm(void);

static void printAllLoansByUser(int);

void printLoan(void) {
    int result=0;
    char func[MAXSIZE_PAGENAME]={'\0'};

    /* if func field is set */
    result = cgiFormStringNoNewlines("func", func, MAXSIZE_PAGENAME);
    if(result != cgiFormSuccess || func == NULL) {
	/* Some sort of failure */
	/* Default to viewAlbums */
	strncpy(func, "view", MAXSIZE_PAGENAME);
    }

    if(strncmp(func, "add", MAXSIZE_PAGENAME) == 0) {
	/* Do add loan etc */
	doAddLoan();
    }
    else {
	/* Default */
	/* Do view loan etc */
	doViewLoan();
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
	    fprintf(cgiOut, "<a href=\"./?page=album&amp;albumid=%d&hash=%d\">[View Album]</a>", newalbumid, _currUserLogon->ID);
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
    int *allArtists=getAllArtists();
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
    fprintf(cgiOut, "    <input type=\"hidden\" name=\"hash\" value=\"%d\" />\n", _currUserLogon->ID);
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

static void doViewLoan(void) {
    int result=0;
    int userid=0;

    /* if userid field is set */
    result = cgiFormInteger("userid", &userid, -1);
    if(result != cgiFormSuccess || userid == -1) {
	/* Some sort of failure */
	/* Default to view for currUser */
	userid=_currUserLogon->ID;
    }

    printAllLoansByUser(userid);
}

static void printAllLoansByUser(int userid) {
    int *allLoans=NULL;
    int curr_id=0;
    int count=0;

    fprintf(cgiOut, "<div class=\"head1\">Viewing All Loans for <b>%s</b></div>", _currUserLogon->userCode);

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
	    fprintf(cgiOut, "<a href=\"./?page=album&amp;albumid=%d&amp;hash=%d\">%s</a>", getLoanAlbum(curr_id), _currUserLogon->ID, getAlbumTitle(getLoanAlbum(curr_id)));
	    fprintf(cgiOut, "    </td>\n");
	    if(isLoanReturned(curr_id) == FALSE) {
		fprintf(cgiOut, "    <td>On Loan</td>\n");
	    }
	    else {
		fprintf(cgiOut, "    <td>Returned</td>\n");
	    }
	    fprintf(cgiOut, "    <td>%d</td>\n", getLoanTimeIn(curr_id));
	    fprintf(cgiOut, "    <td>%d</td>\n", getLoanTimeOut(curr_id));
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

    
