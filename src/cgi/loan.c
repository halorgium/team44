#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "cgic.h"
#include "globals.h"
#include "../shared/defines.h"

static void doBorrowLoan(void);
static void doReturnLoan(void);
static void doUserLoan(funcName_t);
static void doAlbumLoan(void);

static int processBorrowForm(int *, int *);
static int processReturnForm(int *, int *);

static void printCurrentLoansByUser(int);
static void printPreviousLoansByUser(int);

static void printAllLoansByAlbum(int);
static void printCurrentLoanByAlbum(int);
static void printPreviousLoansByAlbum(int);

void printLoan(funcName_t func)
{
    switch (func) {
    case FUNC_BORROW:
        /* Do borrow loan etc */
        doBorrowLoan();
        break;
    case FUNC_RETURN:
        /* Do Return loan etc */
        doReturnLoan();
        break;
    case FUNC_CURRLOAN:
    case FUNC_PREVLOAN:
        /* Do view User loans etc */
        doUserLoan(func);
        break;
    case FUNC_VIEW:
        /* Do view Album loans etc */
        doAlbumLoan();
        break;
    default:
        /* Invalid function */
        fprintf(cgiOut,
                "<div class=\"head1\">Loans</div>\nInvalid request\n");
    }
}

static void doBorrowLoan(void)
{
    Boolean formOK = FALSE;

    /* Form error? */
    int error = E_NOERROR;

    int albumid = -1;

    fprintf(cgiOut, "<div class=\"head1\">Borrowing an Album</div>\n");

    /* The curr data is ready for processing */
    formOK = processBorrowForm(&error, &albumid);
    if (formOK == TRUE) {
        int newLoanid = addLoan(albumid, _currUserLogon);

        if (newLoanid > 0) {
            /* Loan added ok */
            char *albumtitle = getAlbumTitle(albumid);

            fprintf(cgiOut, "Album borrowed successfully<br />\n");
            fprintf(cgiOut,
                    "<a href=\"./?page=album&amp;albumid=%d&amp;hash=%d\">[View Info about &quot;%s&quot;]</a><br />\n",
                    albumid, _currUserLogon, albumtitle);
            if (isUserLibrarian(_currUserLogon) == TRUE) {
                fprintf(cgiOut,
                        "<a href=\"./?page=loan&amp;func=view&amp;albumid=%d&amp;hash=%d\">[View Borrowing History of &quot;%s&quot;]</a><br />\n",
                        albumid, _currUserLogon, albumtitle);
            }
            fprintf(cgiOut,
                    "<a href=\"./?page=loan&amp;func=currloan&amp;userid=%d&amp;hash=%d\">[View your Current Loans]</a><br />\n",
                    _currUserLogon, _currUserLogon);
            fprintf(cgiOut,
                    "<a href=\"./?page=loan&amp;func=prevloan&amp;userid=%d&amp;hash=%d\">[View your Previous Loans]</a>\n",
                    _currUserLogon, _currUserLogon);


            free(albumtitle);
        }
        else {
            /* Album borrowing error */
            fprintf(cgiOut, "DB Save Error<br />\n");
        }
    }

    if (error != E_NOERROR) {
        /* Process error */
        char *albumTitle = NULL;

        /* Album title */
        switch (error) {
        case E_NOERROR:
            break;
        case E_FORM:
            fprintf(cgiOut, "Album not specified<br />\n");
            break;
        case E_NOALBUM:
            fprintf(cgiOut, "Album [%d] does not exist<br />\n", albumid);
            break;
        case ALREADY_ADDED:
            albumTitle = getAlbumTitle(albumid);
            fprintf(cgiOut,
                    "Album &quot;<a href=\"./?page=album&amp;albumid=%d&amp;hash=%d\">%s</a>&quot; is already on loan",
                    albumid, _currUserLogon, albumTitle);

            if (isUserLibrarian(_currUserLogon) == TRUE) {
                int userID = getLoanUser(getAlbumCurrentLoan(albumid));
                char *name = getUserName(userID);

                fprintf(cgiOut, " to ");
                userLink("", userID, name, cgiOut);

                free(name);
            }
            fprintf(cgiOut, "<br />\n");

            free(albumTitle);
            break;
        default:
            fprintf(cgiOut, "Unknown form error<br />\n");
            break;
        }
    }
}

static Boolean processBorrowForm(int *error, int *albumid)
{
    int result = 0;

    result = cgiFormInteger("albid", albumid, -1);
    if (result != cgiFormSuccess || *albumid == -1) {
        *error = E_FORM;
    }
    else if (getAlbumExists(*albumid) == FALSE) {
        *error = E_NOALBUM;
    }
    else if (getAlbumCurrentLoan(*albumid) != E_NOLOAN) {
        *error = ALREADY_ADDED;
    }

    if (*error != E_NOERROR) {
        return FALSE;
    }

    return TRUE;
}

static void doReturnLoan(void)
{
    Boolean formOK = FALSE;

    /* Form error? */
    int error = E_NOERROR;

    int loanid = -1;

    fprintf(cgiOut, "<div class=\"head1\">Returning an Album</div>\n");

    /* The curr data is ready for processing */
    formOK = processReturnForm(&error, &loanid);
    if (formOK == TRUE) {
        int success = addLoanReturned(loanid);

        if (success > 0) {
            /* Loan added ok */
            int albumID = getLoanAlbum(loanid);
            char *albumtitle = getAlbumTitle(albumID);

            fprintf(cgiOut, "Album returned successfully<br />\n");
            fprintf(cgiOut,
                    "<a href=\"./?page=album&amp;albumid=%d&amp;hash=%d\">[View Info about &quot;%s&quot;]</a><br />\n",
                    albumID, _currUserLogon, albumtitle);
            if (isUserLibrarian(_currUserLogon) == TRUE) {
                fprintf(cgiOut,
                        "<a href=\"./?page=loan&amp;func=view&amp;albumid=%d&amp;hash=%d\">[View Borrowing History of &quot;%s&quot;]</a><br />\n",
                        albumID, _currUserLogon, albumtitle);
            }
            fprintf(cgiOut,
                    "<a href=\"./?page=loan&amp;func=currloan&amp;userid=%d&amp;hash=%d\">[View your Current Loans]</a><br />\n",
                    _currUserLogon, _currUserLogon);
            fprintf(cgiOut,
                    "<a href=\"./?page=loan&amp;func=prevloan&amp;userid=%d&amp;hash=%d\">[View your Previous Loans]</a>\n",
                    _currUserLogon, _currUserLogon);

            free(albumtitle);
        }
        else {
            /* Album returning error */
            fprintf(cgiOut, "DB Save Error<br />\n");
        }
    }

    if (error != E_NOERROR) {
        /* Process error */
        int albumid = -1;
        char *albumTitle = NULL;

        /* Album title */
        switch (error) {
        case E_NOERROR:
            break;
        case E_FORM:
            fprintf(cgiOut, "Loan not specified<br />\n");
            break;
        case E_NOLOAN:
            fprintf(cgiOut, "Loan [%d] does not exist<br />\n", loanid);
            break;
        case E_NOUSER:
            albumid = getLoanAlbum(loanid);
            albumTitle = getAlbumTitle(albumid);

            fprintf(cgiOut,
                    "Album &quot;<a href=\"./?page=album&amp;albumid=%d&amp;hash=%d\">%s</a>&quot; is on loan to ",
                    albumid, _currUserLogon, albumTitle);
            if (isUserLibrarian(_currUserLogon) == TRUE) {
                int userID = getLoanUser(loanid);
                char *name = getUserName(userID);

                userLink("", userID, name, cgiOut);

                free(name);
            }
            else {
                fprintf(cgiOut, "another user");
            }
            fprintf(cgiOut, "<br />\n");

            free(albumTitle);
            break;
        case ALREADY_ADDED:
            fprintf(cgiOut, "Album has already been returned<br />\n");
            break;
        default:
            fprintf(cgiOut, "Unknown form error<br />\n");
            break;
        }
    }
}

static Boolean processReturnForm(int *error, int *loanid)
{
    int result = 0;

    result = cgiFormInteger("loanid", loanid, -1);
    if (result != cgiFormSuccess || *loanid == -1) {
        *error = E_FORM;
    }
    else if (getLoanExists(*loanid) == FALSE) {
        *error = E_NOLOAN;
    }
    else if (getLoanUser(*loanid) != _currUserLogon) {
        /* No permission */
        *error = E_NOUSER;
    }
    else if (isLoanReturned(*loanid) == TRUE) {
        *error = ALREADY_ADDED;
    }

    if (*error != E_NOERROR) {
        return FALSE;
    }

    return TRUE;
}

void doUserLoan(funcName_t func)
{
    int result = -1;
    int userid = 0;

    /* check for userid field */
    result = cgiFormInteger("userid", &userid, -1);
    if (result != cgiFormSuccess || userid == -1) {
        /* Nothing specified */
        fprintf(cgiOut, "No id specified\n");
        return;
    }
    else {
        /* Check user exists */
        if (getUserExists(userid) == TRUE) {
            /* Check privileges */
            if (userid == _currUserLogon
                || isUserLibrarian(_currUserLogon) == TRUE) {
                /* Will use userid */
                switch (func) {
                case FUNC_PREVLOAN:
                    printPreviousLoansByUser(userid);
                    break;
                default:
                case FUNC_CURRLOAN:
                    printCurrentLoansByUser(userid);
                    break;
                }
                fprintf(cgiOut,
                        "<hr /><a href=\"./?page=user&amp;userid=%d&amp;hash=%d\">Back to User page</a>\n",
                        userid, _currUserLogon);
            }
            else {
                fprintf(cgiOut,
                        "You can not view loan information of the User [%d]\n",
                        userid);
            }
        }
        else {
            fprintf(cgiOut, "User [%d] does not exist in the database\n",
                    userid);
        }
    }
}

static void printCurrentLoansByUser(int userid)
{
    int *allLoans = NULL;
    int curr_id = 0;
    int count = 0;

    char *name = getUserName(userid);
    fprintf(cgiOut,
            "<div class=\"head1\">Viewing User Current Loans for %s</div>",
            name);
    free(name);

    allLoans = getLoansByUser(userid, FALSE);

    if (allLoans == NULL) {
        fprintf(cgiOut,
                "<div class=\"head1\">Error retrieving all Loans</div>");
    }
    else {
        if (getLoansByUserCount(userid, FALSE) == 0) {
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
            if (userid == _currUserLogon) {
                fprintf(cgiOut, "    <td class=\"thead\">Return?</td>\n");
            }
            fprintf(cgiOut, "  </tr>\n");
            fprintf(cgiOut, "</thead>\n");
            fprintf(cgiOut, "\n");
            fprintf(cgiOut, "<tfoot>\n");
            fprintf(cgiOut, "  <tr>\n");
            if (userid == _currUserLogon) {
                fprintf(cgiOut,
                        "    <td class=\"tfoot\" colspan=\"4\">&nbsp;</td>\n");
            }
            else {
                fprintf(cgiOut,
                        "    <td class=\"tfoot\" colspan=\"3\">&nbsp;</td>\n");
            }
            fprintf(cgiOut, "  </tr>\n");
            fprintf(cgiOut, "</tfoot>\n");
            fprintf(cgiOut, "\n");
            fprintf(cgiOut, "<tbody>\n");

            curr_id = allLoans[count];
            while (curr_id != LAST_ID_IN_ARRAY) {
                int album = getLoanAlbum(curr_id);
                char *title = getAlbumTitle(getLoanAlbum(curr_id));

                fprintf(cgiOut, "  <tr>\n");
                fprintf(cgiOut, "    <td>");
                fprintf(cgiOut,
                        "<a href=\"./?page=album&amp;albumid=%d&amp;hash=%d\">%s</a>",
                        album, _currUserLogon, title);
                fprintf(cgiOut, "    </td>\n");
                fprintf(cgiOut, "    <td>On Loan</td>\n");

                fprintf(cgiOut, "    <td>");
                printTime(getLoanTimeIn(curr_id), cgiOut);
                fprintf(cgiOut, "</td>\n");

                if (getLoanUser(curr_id) == _currUserLogon) {
                    fprintf(cgiOut, "    <td>\n");
                    fprintf(cgiOut,
                            "<form method=\"get\" action=\"./\" style=\"display: inline;\">\n");
                    fprintf(cgiOut, "  <div>\n");
                    fprintf(cgiOut,
                            "    <input type=\"hidden\" name=\"page\" value=\"loan\" />\n");
                    fprintf(cgiOut,
                            "    <input type=\"hidden\" name=\"func\" value=\"return\" />\n");
                    fprintf(cgiOut,
                            "    <input type=\"hidden\" name=\"loanid\" value=\"%d\" />\n",
                            curr_id);
                    fprintf(cgiOut,
                            "    <input type=\"hidden\" name=\"hash\" value=\"%d\" />\n",
                            _currUserLogon);
                    fprintf(cgiOut,
                            "    <input type=\"submit\" value=\"Return Album\" />\n");
                    fprintf(cgiOut, "  </div>\n");
                    fprintf(cgiOut, "</form>\n");
                    fprintf(cgiOut, "</td>\n");
                }

                fprintf(cgiOut, "  </tr>\n");

                count++;
                curr_id = allLoans[count];

                free(title);
            }
            fprintf(cgiOut, "</tbody>\n");
            fprintf(cgiOut, "\n");
            fprintf(cgiOut, "</table>\n");
        }

        free(allLoans);
    }

    fprintf(cgiOut,
            "<hr /><a href=\"./?page=loan&amp;func=prevloan&amp;userid=%d&amp;hash=%d\">View User Previous Loans</a>\n",
            _currUserLogon, _currUserLogon);
}

static void printPreviousLoansByUser(int userid)
{
    int *allLoans = NULL;
    int curr_id = 0;
    int count = 0;

    char *name = getUserName(userid);
    fprintf(cgiOut,
            "<div class=\"head1\">Viewing User Previous Loans for %s</div>",
            name);
    free(name);

    allLoans = getLoansByUser(userid, TRUE);

    if (allLoans == NULL) {
        fprintf(cgiOut,
                "<div class=\"head1\">Error retrieving all Loans</div>");
    }
    else {
        if (getLoansByUserCount(userid, TRUE) == 0) {
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
            fprintf(cgiOut,
                    "    <td class=\"tfoot\" colspan=\"4\">&nbsp;</td>\n");
            fprintf(cgiOut, "  </tr>\n");
            fprintf(cgiOut, "</tfoot>\n");
            fprintf(cgiOut, "\n");
            fprintf(cgiOut, "<tbody>\n");

            curr_id = allLoans[count];
            while (curr_id != LAST_ID_IN_ARRAY) {
                int albumID = getLoanAlbum(curr_id);
                char *title = getAlbumTitle(albumID);

                fprintf(cgiOut, "  <tr>\n");
                fprintf(cgiOut, "    <td>");
                fprintf(cgiOut,
                        "<a href=\"./?page=album&amp;albumid=%d&amp;hash=%d\">%s</a>",
                        albumID, _currUserLogon, title);
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
                curr_id = allLoans[count];

                free(title);
            }

            fprintf(cgiOut, "</tbody>\n");
            fprintf(cgiOut, "\n");
            fprintf(cgiOut, "</table>\n");
        }

        free(allLoans);
    }

    fprintf(cgiOut,
            "<hr /><a href=\"./?page=loan&amp;func=currloan&amp;userid=%d&amp;hash=%d\">View User Current Loans</a><br />\n",
            _currUserLogon, _currUserLogon);
}

static void doAlbumLoan(void)
{
    int result = 0;
    int albumid = 0;

    /* Check for albumid field */
    result = cgiFormInteger("albumid", &albumid, -1);
    if (result != cgiFormSuccess || albumid == -1) {
        /* Nothing specified */
        fprintf(cgiOut, "No id specified\n");
        return;
    }
    else {
        /* Check album exists */
        if (getAlbumExists(albumid) == TRUE) {
            /* Check privileges */
            if (isUserLibrarian(_currUserLogon) == TRUE) {
                /* Will use albumid */
                printAllLoansByAlbum(albumid);
            }
            else {
                fprintf(cgiOut,
                        "You can not view the borrowing history of the Album [%d]\n",
                        albumid);
            }
        }
        else {
            fprintf(cgiOut, "Album [%d] does not exist in the database\n",
                    albumid);
        }
    }
}

static void printAllLoansByAlbum(int albumid)
{
    char *title = getAlbumTitle(albumid);

    fprintf(cgiOut,
            "<div class=\"head1\">Viewing Albums Borrowing History for %s</div>",
            title);

    printCurrentLoanByAlbum(albumid);

    fprintf(cgiOut, "<hr />\n");

    fprintf(cgiOut, "<b>Previous Loans: </b><br />\n");
    printPreviousLoansByAlbum(albumid);

    fprintf(cgiOut,
            "<hr /><a href=\"./?page=album&amp;albumid=%d&amp;hash=%d\">Back to Album page</a>\n",
            albumid, _currUserLogon);

    free(title);
}

static void printCurrentLoanByAlbum(int albumid)
{
    /* is album on loan */
    int currLoan = getAlbumCurrentLoan(albumid);
    if (currLoan != E_NOLOAN) {
        int tempUserID = 0;
        tempUserID = getLoanUser(currLoan);

        if (tempUserID == _currUserLogon) {
            fprintf(cgiOut, "You have this album on loan<br />\n");
            fprintf(cgiOut, "<form method=\"get\" action=\"./\">\n");
            fprintf(cgiOut, "  <p>\n");
            fprintf(cgiOut,
                    "    <input type=\"hidden\" name=\"page\" value=\"loan\" />\n");
            fprintf(cgiOut,
                    "    <input type=\"hidden\" name=\"func\" value=\"return\" />\n");
            fprintf(cgiOut,
                    "    <input type=\"hidden\" name=\"loanid\" value=\"%d\" />\n",
                    currLoan);
            fprintf(cgiOut,
                    "    <input type=\"hidden\" name=\"hash\" value=\"%d\" />\n",
                    _currUserLogon);
            fprintf(cgiOut,
                    "    <input type=\"submit\" value=\"Return Album\" />\n");
            fprintf(cgiOut, "  </p>\n");
            fprintf(cgiOut, "</form>\n");
        }
        else {
            fprintf(cgiOut, "Album is on loan ");
            if (isUserLibrarian(_currUserLogon) == TRUE) {
                char *name = getUserName(tempUserID);


                fprintf(cgiOut, "to <b>");
                userLink("", tempUserID, name, cgiOut);
                fprintf(cgiOut, "</b><br />\n");
                fprintf(cgiOut, "[Taken at ");
                printTime(getLoanTimeIn(currLoan), cgiOut);
                fprintf(cgiOut, "]\n");

                free(name);
            }
        }
    }
    else {
        fprintf(cgiOut, "Album is not on loan\n");

        fprintf(cgiOut, "<form method=\"get\" action=\"./\">\n");
        fprintf(cgiOut, "  <p>\n");
        fprintf(cgiOut,
                "    <input type=\"hidden\" name=\"page\" value=\"loan\" />\n");
        fprintf(cgiOut,
                "    <input type=\"hidden\" name=\"func\" value=\"borrow\" />\n");
        fprintf(cgiOut,
                "    <input type=\"hidden\" name=\"albid\" value=\"%d\" />\n",
                albumid);
        fprintf(cgiOut,
                "    <input type=\"hidden\" name=\"hash\" value=\"%d\" />\n",
                _currUserLogon);
        fprintf(cgiOut,
                "    <input type=\"submit\" value=\"Borrow Album\" />\n");
        fprintf(cgiOut, "  </p>\n");
        fprintf(cgiOut, "</form>\n");
    }
}

static void printPreviousLoansByAlbum(int albumid)
{
    /* view borrowing history */
    int *allLoans = NULL;
    int curr_id = 0;
    int count = 0;

    allLoans = getLoansByAlbum(albumid, TRUE);

    if (allLoans == NULL) {
        fprintf(cgiOut,
                "<div class=\"head1\">Error retrieving all Loans</div>");
    }
    else {
        if (getLoansByAlbumCount(albumid, TRUE) == 0) {
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
            fprintf(cgiOut,
                    "    <td class=\"tfoot\" colspan=\"4\">&nbsp;</td>\n");
            fprintf(cgiOut, "  </tr>\n");
            fprintf(cgiOut, "</tfoot>\n");
            fprintf(cgiOut, "\n");
            fprintf(cgiOut, "<tbody>\n");

            curr_id = allLoans[count];
            while (curr_id != LAST_ID_IN_ARRAY) {
                int userID = getLoanUser(curr_id);
                char *name = getUserName(userID);

                fprintf(cgiOut, "  <tr>\n");
                fprintf(cgiOut, "    <td>");
                userLink("", userID, name, cgiOut);
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
                curr_id = allLoans[count];

                free(name);
            }

            fprintf(cgiOut, "</tbody>\n");
            fprintf(cgiOut, "\n");
            fprintf(cgiOut, "</table>\n");
        }

        free(allLoans);
    }
}
