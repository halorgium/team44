/*
 *
 * main.c
 * ==========
 * provides functions which deal with the main section in the cgi
 * 
*/

/*===================== Preprocessor statements===========================*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "cgic.h"
#include "globals.h"
#include "../shared/defines.h"

/*======================Function Declarations=============================*/

static void printLinks(FILE *);
static void printHeader(FILE *);
static void printFooter(FILE *);
static void theRealWork(void);

/*======================Function Definitions=============================*/

/*
 * Function: cgiMain
 * Parameters: (none)
 * Returns: int
 *
 * calls all of the appropriate functions
 */
int cgiMain()
{
    _currUserLogon = E_NOUSER;

    /* Start outputting the header */
    printHeader(cgiOut);

    /* Do the real work */
    theRealWork();

    /* Start outputting the footer */
    printFooter(cgiOut);

    return 0;
}

/*
 * Function: theRealWork
 * Parameters: (void)
 * Returns: (void)
 *
 * checks the user logged in correctly and displays all information that the user needs to see
 * depending on whether they are a librarian or a standerd user.
 * This is the dynamic section of the page
 */
static void theRealWork(void)
{
    int result = 0;

    pageName_t pageName = PAGE_UNKNOWN;
    funcName_t funcName = FUNC_UNKNOWN;

    int userHash = 0;

    /* At this point we start the dynamic content, error page may be shown */

    /* We might need the database so load it up */
    result = loadDatabase();
    if (result != DB_LOAD_SUCCESS) {
        /* some problem with loading */
        fprintf(cgiOut, "<td class=\"errormsg\">DB Load problem<br />\n");

        switch (result) {
        case NEXTID_LOAD_FAILURE:
            fprintf(cgiOut, "Next ID Load failure");
            break;
        case USER_LOAD_FAILURE:
            fprintf(cgiOut, "Users Load failure");
            break;
        case ALBUM_LOAD_FAILURE:
            fprintf(cgiOut, "Albums Load failure");
            break;
        case ARTIST_LOAD_FAILURE:
            fprintf(cgiOut, "Artists Load failure");
            break;
        case USR_COM_LOAD_FAILURE:
            fprintf(cgiOut, "User Comment Load failure");
            break;
        case ALB_COM_LOAD_FAILURE:
            fprintf(cgiOut, "Album Comments Load failure");
            break;
        case ART_COM_LOAD_FAILURE:
            fprintf(cgiOut, "Artist Comments Load failure");
            break;
        case LOAN_LOAD_FAILURE:
            fprintf(cgiOut, "Loans Load failure");
            break;
        case LOANRET_LOAD_FAILURE:
            fprintf(cgiOut, "Loans Load failure");
            break;
        default:
        case DB_LOAD_FAILURE:
            fprintf(cgiOut, "General Load failure");
            break;
        }

        fprintf(cgiOut, "</td>\n</tr>\n<tr>\n");
        return;
    }

    result = cgiFormInteger("hash", &userHash, -1);
    if (result != cgiFormSuccess || userHash == -1) {
        /* No hash */

        /* check for login attempt */
        int dologin = 0;
        result = cgiFormInteger("dologin", &dologin, FALSE);

        if (result != cgiFormSuccess || dologin == FALSE) {
            /* No attempt */
            /* Display login */
            pageName = PAGE_LOGIN;
        }
        else {
            /* Hey someone is trying to login */
            /* Get the userCode */
            int size = -1;

            result = cgiFormStringSpaceNeeded("usercode", &size);
            if (result != cgiFormSuccess) {
                /* Unsuccessful */
                /* Display Login */
                fprintf(cgiOut,
                        "<td class=\"errormsg\">No such user</td></tr>\n<tr>\n");
                pageName = PAGE_LOGIN;
            }
            else {
                /* Get the usercode variable */
                char *tempuserCode = malloc(sizeof(char) * size);
                if (tempuserCode == NULL) {
                    fprintf(cgiOut,
                            "<td class=\"errormsg\">Memory Error</td></tr>\n<tr>\n");
                    return;
                }

                result =
                    cgiFormStringNoNewlines("usercode", tempuserCode, size);
                if (result != cgiFormSuccess) {
                    /* Unsuccessful */
                    /* Display login */
                    fprintf(cgiOut,
                            "<td class=\"errormsg\">No such user</td></tr>\n<tr>\n");
                    pageName = PAGE_LOGIN;
                }
                else {
                    /* Now check user */
                    int tempHash = makeUserID(toLowerCase(tempuserCode));
                    if (getUserExists(tempHash) == FALSE) {
                        /* No such user */
                        fprintf(cgiOut,
                                "<td class=\"errormsg\">No such user</td></tr>\n<tr>\n");
                        pageName = PAGE_LOGIN;
                    }
                    else {
                        _currUserLogon = tempHash;
                        pageName = PAGE_HOME;
                    }
                }

                free(tempuserCode);
            }
        }
    }
    else {
        /* Check & get user */
        /* Make sure logged in */
        if (getUserExists(userHash) == FALSE) {
            /* UserHash invalid */
            fprintf(cgiOut,
                    "<td class=\"errormsg\">Bad hash</td></tr>\n<tr>\n");
            pageName = PAGE_LOGIN;
        }
        else {
            int size = -1;

            _currUserLogon = userHash;

            result = cgiFormStringSpaceNeeded("page", &size);
            if (result != cgiFormSuccess) {
                /* Unsuccessful */
                /* Display Home */
                pageName = PAGE_HOME;
            }
            else {
                /* Get the page variable */
                char *temppageName = malloc(sizeof(char) * size);
                if (temppageName == NULL) {
                    /* Malloc fail */
                    /* Display Home */
                    pageName = PAGE_HOME;
                }
                else {
                    result =
                        cgiFormStringNoNewlines("page", temppageName, size);
                    if (result != cgiFormSuccess) {
                        /* Display Home */
                        pageName = PAGE_HOME;
                    }

                    if (strncmp(temppageName, "login", size) == 0) {
                        pageName = PAGE_LOGIN;
                    }
                    else if (strncmp(temppageName, "contact", size) == 0) {
                        pageName = PAGE_CONTACT;
                    }
                    else if (strncmp(temppageName, "user", size) == 0) {
                        pageName = PAGE_USER;
                    }
                    else if (strncmp(temppageName, "album", size) == 0) {
                        pageName = PAGE_ALBUM;
                    }
                    else if (strncmp(temppageName, "artist", size) == 0) {
                        pageName = PAGE_ARTIST;
                    }
                    else if (strncmp(temppageName, "usercomment", size) == 0) {
                        pageName = PAGE_USERCOMMENT;
                    }
                    else if (strncmp(temppageName, "albumcomment", size) == 0) {
                        pageName = PAGE_ALBUMCOMMENT;
                    }
                    else if (strncmp(temppageName, "artistcomment", size) ==
                             0) {
                        pageName = PAGE_ARTISTCOMMENT;
                    }
                    else if (strncmp(temppageName, "loan", size) == 0) {
                        pageName = PAGE_LOAN;
                    }
                    else {
                        /* Default */
                        pageName = PAGE_HOME;
                    }
                    free(temppageName);

                    /* Now get the func variable if necessary */
                    switch (pageName) {
                    case PAGE_USER:
                    case PAGE_ALBUM:
                    case PAGE_ARTIST:
                    case PAGE_USERCOMMENT:
                    case PAGE_ALBUMCOMMENT:
                    case PAGE_ARTISTCOMMENT:
                    case PAGE_LOAN:
                        result = cgiFormStringSpaceNeeded("func", &size);
                        if (result != cgiFormSuccess) {
                            /* Unsuccessful */
                            /* Func is unknown */
                            funcName = FUNC_UNKNOWN;
                        }
                        else {
                            /* Get the page variable */
                            char *tempfuncName = malloc(sizeof(char) * size);
                            if (tempfuncName == NULL) {
                                /* Malloc fail */
                                /* Func is unknown */
                                funcName = FUNC_UNKNOWN;
                            }
                            else {
                                result =
                                    cgiFormStringNoNewlines("func",
                                                            tempfuncName,
                                                            size);
                                if (result != cgiFormSuccess) {
                                    /* Func is unknown */
                                    funcName = FUNC_UNKNOWN;
                                }
                                else {
                                    if (strncmp(tempfuncName, "add", size) ==
                                        0) {
                                        /* Used when add an entity */
                                        funcName = FUNC_ADD;
                                    }
                                    else if (strncmp
                                             (tempfuncName, "borrow",
                                              size) == 0) {
                                        /* Used when borrowing albums */
                                        funcName = FUNC_BORROW;
                                    }
                                    else if (strncmp
                                             (tempfuncName, "return",
                                              size) == 0) {
                                        /* Used when returning albums */
                                        funcName = FUNC_RETURN;
                                    }
                                    else if (strncmp
                                             (tempfuncName, "currloan",
                                              size) == 0) {
                                        /* Used when viewing current user loans */
                                        funcName = FUNC_CURRLOAN;
                                    }
                                    else if (strncmp
                                             (tempfuncName, "prevloan",
                                              size) == 0) {
                                        /* Used when viewing previous user loans */
                                        funcName = FUNC_PREVLOAN;
                                    }
                                    else {
                                        /* Default */
                                        /* Used when viewing entities */
                                        funcName = FUNC_VIEW;
                                    }
                                }
                                free(tempfuncName);
                            }
                        }
                        break;
                    default:
                        /* Func is not necessary for these pages */
                        funcName = FUNC_UNKNOWN;
                    }
                }
            }
        }
    }

    /* If links are necessary print them */
    if (_currUserLogon != E_NOUSER) {
        printLinks(cgiOut);
    }

    {
        /* Start outputting the body */

        fprintf(cgiOut, "<td id=\"bodycontent\">\n");
        fprintf(cgiOut, "<a name=\"navskip\"></a>\n");
        fprintf(cgiOut, "<!-- Start Body -->\n");

        switch (pageName) {
        case PAGE_HOME:
            printHome();
            break;
        case PAGE_CONTACT:
            printContact();
            break;
        case PAGE_USER:
            printUser(funcName);
            break;
        case PAGE_ALBUM:
            printAlbum(funcName);
            break;
        case PAGE_ARTIST:
            printArtist(funcName);
            break;
        case PAGE_USERCOMMENT:
            printUserComment(funcName);
            break;
        case PAGE_ALBUMCOMMENT:
            printAlbumComment(funcName);
            break;
        case PAGE_ARTISTCOMMENT:
            printArtistComment(funcName);
            break;
        case PAGE_LOAN:
            printLoan(funcName);
            break;
        default:
            /* Default */
            printLogin();
        }

        fprintf(cgiOut, "<!-- End Body -->\n");
        fprintf(cgiOut, "</td>\n");
    }
}

/*
 * Function: printLinks
 * Parameters: FILE
 * Returns: (void)
 *
 * Prints the links that are on the left hand side margin that let you orientate
 * your way around the page.
 */
static void printLinks(FILE * output)
{
    char *userCode = getUserCode(_currUserLogon);

    fprintf(cgiOut, "<td class=\"linkholder\">\n");
    fprintf(cgiOut, "<!-- Start Links -->\n");
    fprintf(cgiOut, "<table class=\"links\">\n");
    fprintf(cgiOut, "  <tbody>\n");

    fprintf(cgiOut,
            "  <tr>\n    <td class=\"username\">You are logged on as&nbsp;<b>%s</b></td>\n  </tr>\n",
            userCode);
    fprintf(cgiOut,
            "  <tr>\n    <td class=\"spacer\">&nbsp;</td>\n  </tr>\n");

    fprintf(output,
            "  <tr>\n    <td class=\"link\"><a class=\"buttonref\" href=\"./?hash=%d\">Home</a></td>\n  </tr>\n",
            _currUserLogon);
    fprintf(output,
            "  <tr>\n    <td class=\"link\"><a class=\"buttonref\" href=\"./?page=contact&amp;hash=%d\">Contact&nbsp;Us</a></td>\n  </tr>\n",
            _currUserLogon);
    fprintf(output,
            "  <tr>\n    <td class=\"link\"><a class=\"buttonref\" href=\"./help/index.html\">Help!</a></td>\n  </tr>\n");
    fprintf(output,
            "  <tr>\n    <td class=\"link\"><a class=\"buttonref\" href=\"./?page=logout\">Logout</a></td>\n  </tr>\n");
    fprintf(output,
            "  <tr>\n    <td class=\"spacer\">&nbsp;</td>\n  </tr>\n");
    fprintf(output,
            "  <tr>\n    <td class=\"link\"><a class=\"buttonref\" href=\"./?page=user&amp;userid=%d&amp;hash=%d\">My&nbsp;Account</a></td>\n  </tr>\n",
            _currUserLogon, _currUserLogon);
    fprintf(output,
            "  <tr>\n    <td class=\"link\"><a class=\"buttonref\" href=\"./?page=loan&amp;func=currloan&amp;userid=%d&amp;hash=%d\">My&nbsp;Current&nbsp;Loans</a></td>\n  </tr>\n",
            _currUserLogon, _currUserLogon);
    fprintf(output,
            "  <tr>\n    <td class=\"link\"><a class=\"buttonref\" href=\"./?page=loan&amp;func=prevloan&amp;userid=%d&amp;hash=%d\">My&nbsp;Previous&nbsp;Loans</a></td>\n  </tr>\n",
            _currUserLogon, _currUserLogon);
    fprintf(output,
            "  <tr>\n    <td class=\"link\"><a class=\"buttonref\" href=\"./?page=usercomment&amp;userid=%d&amp;hash=%d\">My&nbsp;Comments</a></td>\n  </tr>\n",
            _currUserLogon, _currUserLogon);
    fprintf(output,
            "  <tr>\n    <td class=\"spacer\">&nbsp;</td>\n  </tr>\n");

    fprintf(output,
            "  <tr>\n    <td class=\"link\"><a class=\"buttonref\" href=\"./?page=artist&amp;hash=%d\">View&nbsp;All&nbsp;Artists</a></td>\n  </tr>\n",
            _currUserLogon);
    if (isUserLibrarian(_currUserLogon) == TRUE) {
        fprintf(output,
                "  <tr>\n    <td class=\"link\"><a class=\"buttonref\" href=\"./?page=artist&amp;func=add&amp;hash=%d\">Add&nbsp;New&nbsp;Artist</a></td>\n  </tr>\n",
                _currUserLogon);
    }
    fprintf(output,
            "  <tr>\n    <td class=\"link\"><a class=\"buttonref\" href=\"./?page=artistcomment&amp;hash=%d\">View&nbsp;Artist&nbsp;Comments</a></td>\n  </tr>\n",
            _currUserLogon);
    fprintf(output,
            "  <tr>\n    <td class=\"link\"><a class=\"buttonref\" href=\"./?page=artistcomment&amp;func=add&amp;hash=%d\">Add&nbsp;Artist&nbsp;Comment</a></td>\n  </tr>\n",
            _currUserLogon);
    fprintf(output,
            "  <tr>\n    <td class=\"spacer\">&nbsp;</td>\n  </tr>\n");

    fprintf(output,
            "  <tr>\n    <td class=\"link\"><a class=\"buttonref\" href=\"./?page=album&amp;hash=%d\">View&nbsp;All&nbsp;Albums</a></td>\n  </tr>\n",
            _currUserLogon);
    if (isUserLibrarian(_currUserLogon) == TRUE) {
        fprintf(output,
                "  <tr>\n    <td class=\"link\"><a class=\"buttonref\" href=\"./?page=album&amp;func=add&amp;hash=%d\">Add&nbsp;New&nbsp;Album</a></td>\n  </tr>\n",
                _currUserLogon);
    }
    fprintf(output,
            "  <tr>\n    <td class=\"link\"><a class=\"buttonref\" href=\"./?page=albumcomment&amp;hash=%d\">View&nbsp;Album&nbsp;Comments</a></td>\n  </tr>\n",
            _currUserLogon);
    fprintf(output,
            "  <tr>\n    <td class=\"link\"><a class=\"buttonref\" href=\"./?page=albumcomment&amp;func=add&amp;hash=%d\">Add&nbsp;Album&nbsp;Comment</a></td>\n  </tr>\n",
            _currUserLogon);

    if (isUserLibrarian(_currUserLogon) == TRUE) {
        fprintf(output,
                "  <tr>\n    <td class=\"spacer\">&nbsp;</td>\n  </tr>\n");
        fprintf(output,
                "  <tr>\n    <td class=\"link\"><a class=\"buttonref\" href=\"./?page=user&amp;hash=%d\">View&nbsp;All&nbsp;Users</a></td>\n  </tr>\n",
                _currUserLogon);
        fprintf(output,
                "  <tr>\n    <td class=\"link\"><a class=\"buttonref\" href=\"./?page=user&amp;func=add&amp;hash=%d\">Add&nbsp;New&nbsp;User</a></td>\n  </tr>\n",
                _currUserLogon);
        fprintf(output,
                "  <tr>\n    <td class=\"link\"><a class=\"buttonref\" href=\"./?page=usercomment&amp;hash=%d\">View&nbsp;User&nbsp;Comments</a></td>\n  </tr>\n",
                _currUserLogon);
        fprintf(output,
                "  <tr>\n    <td class=\"link\"><a class=\"buttonref\" href=\"./?page=usercomment&amp;func=add&amp;hash=%d\">Add&nbsp;User&nbsp;Comment</a></td>\n  </tr>\n",
                _currUserLogon);
    }

    fprintf(output, "  </tbody>\n");
    fprintf(output, "</table>\n");
    fprintf(output, "<!-- End Links -->\n");
    fprintf(output, "</td>\n");

    free(userCode);
}

/*
 * Function: printHeader
 * Parameters: FILE
 * Returns: (void)
 *
 * Prints the header of the page. IE the title bar of the page
 * and sets the structure of the html source file
 */
static void printHeader(FILE * output)
{
    cgiHeaderContentType("text/html");

    fprintf(output, "<?xml version=\"1.0\" encoding=\"ISO-8859-1\"?>\n");
    fprintf(output,
            "<!DOCTYPE html PUBLIC \"-//W3C//DTD XHTML 1.0 Strict//EN\"\n");
    fprintf(output,
            " \"http://www.w3.org/TR/xhtml1/DTD/xhtml1-strict.dtd\">\n");
    fprintf(output, "<html>\n");
    fprintf(output, "  <head>\n");
    fprintf(output,
            "    <meta http-equiv=\"Content-Type\" content=\"text/html; charset=iso-8859-1\" />\n");
    fprintf(output,
            "    <meta http-equiv=\"Content-Style-Type\" content=\"text/css\" />\n");
    fprintf(output,
            "    <meta name=\"keywords\" content=\"music, library\" />\n");
    fprintf(output,
            "    <meta name=\"description\" content=\"The Music Library\" />\n");
    fprintf(output, "    <title>Team 44 Music Library</title>\n");
    fprintf(output,
            "    <link rel=\"stylesheet\" type=\"text/css\" href=\"musiclib.css\" />\n");
    fprintf(output,
            "    <link rel=\"made\" href=\"mailto:c204g18@cosc.canterbury.ac.nz\" />\n");
    fprintf(output,
            "    <!--link rel=\"shortcut icon\" href=\"favicon.ico\" /-->\n");
    fprintf(output, "  </head>\n");
    fprintf(output, "  <body>\n");
    fprintf(output,
            "    <div id=\"hiddenlink\"><a href=\"#navskip\"><img src=\"1px.gif\" width=\"1\" height=\"10\" alt=\"Go to main content\" style=\"border: 0px;\" /></a></div>\n");
    fprintf(output, "    <table id=\"container\">\n");
    fprintf(output, "    <thead>\n");
    fprintf(output, "  <tr>\n");
    fprintf(output, "<td id=\"header\" colspan=\"2\">\n");
    fprintf(output, "<!-- Start Header -->\n");
    fprintf(output,
            "  <div id=\"maintitle\"><img src=\"logo.png\" width=\"398\" height=\"140\" alt=\"Team44 Music Library\" /></div>\n");
    fprintf(output, "<!-- End Header -->\n");
    fprintf(output, "</td>\n");
    fprintf(output, "  </tr>\n");
    fprintf(output, "  <tr id=\"topspacera\">\n");
    fprintf(output, "<td id=\"topspacerb\" colspan=\"2\">&nbsp;</td>\n");
    fprintf(output, "  </tr>\n");
    fprintf(output, "    </thead>\n");
    fprintf(output, "    <tfoot>\n");
    fprintf(output, "  <tr>\n");
    fprintf(output, "<td id=\"footer\" colspan=\"2\">\n");
    fprintf(output, "&#169;&nbsp;Copyright&nbsp;Team&nbsp;44&nbsp;2003\n");
    fprintf(output, "</td>\n");
    fprintf(output, "  </tr>\n");
    fprintf(output, "    </tfoot>\n");
    fprintf(output, "    <tbody>\n");
    fprintf(output, "  <tr>\n");
}

/*
 * Function: printFooter
 * Parameters: FILE
 * Returns: (void)
 *
 * Prints the footer of the page. 
 * and closes off the html source file
 */
static void printFooter(FILE * output)
{
    fprintf(output, "  </tr>\n");
    fprintf(output, "    </tbody>\n");
    fprintf(output, "</table>\n");
    fprintf(output, "</body>\n");
    fprintf(output, "</html>\n");
}
