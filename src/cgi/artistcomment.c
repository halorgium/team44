/*
 *
 * artistcomment.c
 * ==========
 * provides functions which deal with artist comments in the cgi
 * 
*/

/*===================Preprocessor Statements===============================*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "cgic.h"
#include "globals.h"
#include "../shared/defines.h"
#include "../database/structs.h"

/*======================Function Declarations=============================*/

static void doAddArtistComment(void);
static void doViewArtistComment(void);

static Boolean processAddForm(int *, artistCommentNode_t *);
static void printAddForm(Boolean, int *, artistCommentNode_t *);

static void printAllArtistComments(void);
static void printAllArtistCommentsByUser(int);
static void printAllArtistCommentsForArtist(int);


/*======================Function Definitions=============================*/


/*
 * Function: printArtistComment
 * Parameters: funcName stuct
 * Returns: (void)
 *
 * checks to see if the function name is for adding an artist.
 * if not it calls the doViewArtist function
 */
void printArtistComment(funcName_t func)
{
    switch (func) {
    case FUNC_ADD:
        /* Do add artistComment etc */
        doAddArtistComment();
        break;
    default:
        /* Do view artistComment etc */
        doViewArtistComment();
        break;
    }
}

/*
 * Function: doAddArtistComment
 * Parameters: (void)
 * Returns: (void)
 *
 * This function communicates with the database and adds artistcomments. Then checks that there
 * are artists in the database which we can attribute the comment with. Providing there are no errors the
 * artist comment is then added to the database and a message is sent back to the user - Adding Successful.
 */
static void doAddArtistComment(void)
{
    Boolean needAddForm = TRUE;

    int result = 0;

    /* Temporary Struct to store form data */
    artistCommentNode_t *formdata = NULL;
    /* Array to store errors */
    int *errors = NULL;
    Boolean isAdding = FALSE;

    fprintf(cgiOut, "<div class=\"head1\">Adding New Artist Comment</div>\n");

    if (getArtistsCount() == 0) {
        fprintf(cgiOut, "No artists<br />\n");
        if (isUserLibrarian(_currUserLogon) == TRUE) {
            fprintf(cgiOut,
                    "<a href=\"./?page=artist&amp;func=add&amp;hash=%d\">[Add Artist]</a>\n",
                    _currUserLogon);
        }
        return;
    }

    /* if adding field is set */
    result = cgiFormIntegerBounded("adding", &isAdding, FALSE, TRUE, FALSE);
    if (result != cgiFormSuccess) {
        /* Some sort of failure */
        isAdding = FALSE;
    }

    /* Malloc space for form data */
    formdata = malloc(sizeof(artistCommentNode_t));
    if (formdata == NULL) {
        isAdding = FALSE;
    }
    else {
        Boolean formOK = FALSE;

        formdata->artistID = -1;
        formdata->comment = NULL;

        /* Malloc space for error code of each field */
        errors = malloc(sizeof(int) * 2);
        if (errors == NULL) {
            free(formdata);
            isAdding = FALSE;
        }
        else {
            /* Set errors to E_NOERROR */
            int i = 0;
            for (i = 0; i < 2; i++) {
                errors[i] = E_NOERROR;
            }
        }

        /* The curr data is ready for processing */
        formOK = processAddForm(errors, formdata);

        if (isAdding == TRUE && formOK == TRUE) {
            int newArtistCommentid = -1;

            /* All form data is good */
            /* Add artist comment to database */
            newArtistCommentid =
                addArtistComment(formdata->artistID, _currUserLogon,
                                 formdata->comment);

            if (newArtistCommentid > 0) {
                /* Artist Comment added ok */
                int artistid = getArtistCommentArtist(newArtistCommentid);
                char *artistname = getArtistName(artistid);

                needAddForm = FALSE;

                fprintf(cgiOut, "Adding successful<br />\n");
                fprintf(cgiOut,
                        "<a href=\"./?page=artist&amp;artistid=%d&amp;hash=%d\">[View Info about &quot;%s&quot;]</a><br />\n",
                        artistid, _currUserLogon, artistname);
                fprintf(cgiOut,
                        "<a href=\"./?page=artistcomment&amp;func=view&amp;artistid=%d&amp;hash=%d\">[View All Comments about &quot;%s&quot;]</a><br />\n",
                        artistid, _currUserLogon, artistname);
                fprintf(cgiOut,
                        " <a href=\"./?page=artistcomment&amp;func=add&amp;artistid=%d&amp;hash=%d\">[Write another Comment about &quot;%s&quot;]</a><br />\n",
                        artistid, _currUserLogon, artistname);
                fprintf(cgiOut,
                        " <a href=\"./?page=artistcomment&amp;func=add&amp;hash=%d\">[Write a Comment about another Artist]</a>\n",
                        _currUserLogon);

                free(artistname);
            }
            else {
                /* Artist Comment adding error */
                fprintf(cgiOut, "DB Save Error<br />\n");
            }
        }
    }

    if (needAddForm == TRUE) {
        /* Need to print form */
        printAddForm(isAdding, errors, formdata);
    }

    /* Free the memory */
    if (errors != NULL) {
        free(errors);
    }
    if (formdata != NULL) {
        if (formdata->comment != NULL) {
            free(formdata->comment);
        }
        free(formdata);
    }
}

/*
 * Function: processAddForm
 * Parameters: int *, artistCommentNode_t *
 * Returns: Boolean
 *
 * This is used to process the form that adds the new artistcomment..
 */
static Boolean processAddForm(int *errors, artistCommentNode_t * formdata)
{
    int result = 0;
    int size = -1;

    /* Check arguments */
    if (errors == NULL || formdata == NULL) {
        return FALSE;
    }

    /* Get Artist ID */
    result = cgiFormInteger("artid", &formdata->artistID, -1);
    if (result != cgiFormSuccess || formdata->artistID == -1) {
        errors[0] = E_FORM;
    }
    else if (getArtistExists(formdata->artistID) == FALSE) {
        /* No artistid */
        errors[0] = E_NOARTIST;
    }
    /* Get Comment body */
    result = cgiFormStringSpaceNeeded("combody", &size);
    if (result != cgiFormSuccess) {
        errors[1] = E_FORM;
    }
    else if (size > MAXLEN_ARTISTCOMMENT + 1) {
        errors[1] = E_TOOBIG;
    }
    else {
        formdata->comment = malloc(sizeof(char) * size);
        if (formdata->comment == NULL) {
            errors[1] = E_MALLOC_FAILED;
        }
        else {
            result = cgiFormString("combody", formdata->comment, size);
            if (result != cgiFormSuccess) {
                errors[1] = E_FORM;
            }
            else if (checkString2(formdata->comment) == FALSE) {
                errors[1] = E_INVALID_PARAM;
            }
            else {
                replaceCharWithChar(formdata->comment, '\n', NEWLINE_SUBST);
            }
        }
    }

    if (errors[0] != E_NOERROR || errors[1] != E_NOERROR) {
        return FALSE;
    }
    return TRUE;
}

/*
 * Function: processAddForm
 * Parameters: Boolean, int *, artistCommentNode_t *
 * Returns: (void)
 *
 * This is used to process the form that adds the new artistcomment..
 */
static void printAddForm(Boolean isAdding, int *errors,
                         artistCommentNode_t * formdata)
{
    Boolean freshForm = FALSE;

    /* Check arguments */
    if (isAdding == FALSE || errors == NULL || formdata == NULL) {
        /* Print a fresh form */
        freshForm = TRUE;
    }
    else {
        /* Process errors */

        /* Artist ID */
        switch (errors[0]) {
        case E_NOERROR:
            break;
        case E_FORM:
            fprintf(cgiOut, "Artist not selected<br />\n");
            break;
        case E_NOARTIST:
            fprintf(cgiOut, "Artist [%d] does not exist<br />\n",
                    formdata->artistID);
            break;
        default:
            fprintf(cgiOut, "Unknown form error<br />\n");
            break;
        }

        /* Comment body */
        switch (errors[1]) {
        case E_NOERROR:
            break;
        case E_FORM:
            fprintf(cgiOut, "Comment body is empty<br />\n");
            break;
        case E_TOOBIG:
            fprintf(cgiOut, "Comment body is too big<br />\n");
            break;
        case E_INVALID_PARAM:
            fprintf(cgiOut, "Comment body is invalid<br />\n");
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
    fprintf(cgiOut, "  <tr><td>\n");
    fprintf(cgiOut,
            "    <input type=\"hidden\" name=\"page\" value=\"artistcomment\" />\n");
    fprintf(cgiOut,
            "    <input type=\"hidden\" name=\"func\" value=\"add\" />\n");
    fprintf(cgiOut,
            "    <input type=\"hidden\" name=\"adding\" value=\"%d\" />\n",
            TRUE);
    fprintf(cgiOut,
            "    <input type=\"hidden\" name=\"hash\" value=\"%d\" />\n",
            _currUserLogon);
    fprintf(cgiOut, "  </td></tr>\n");
    fprintf(cgiOut, "  <tr>\n");
    fprintf(cgiOut,
            "    <td class=\"describe%s\"><label title=\"Artist Name\">Artist <u>N</u>ame: </label></td>\n",
            ((isAdding == TRUE && errors != NULL
              && errors[0] != E_NOERROR) ? "2" : ""));
    fprintf(cgiOut, "  </tr>\n");

    fprintf(cgiOut, "  <tr>\n");
    fprintf(cgiOut, "    <td class=\"field\">\n");
    /* Print out the <select> for all artists */
    fprintf(cgiOut,
            "<select id=\"artid\" name=\"artid\" size=\"5\" accesskey=\"n\">\n");
    fprintf(cgiOut, "  <option value=\"-1\"%s>&nbsp;</option>\n",
            ((formdata == NULL
              || getArtistExists(formdata->artistID) ==
              FALSE) ? " selected=\"selected\"" : ""));
    {
        int *allArtists = getArtists();
        int count = 0;
        int curr_id = 0;

        curr_id = allArtists[count];
        while (curr_id != LAST_ID_IN_ARRAY) {
            char *title = getArtistName(curr_id);
            fprintf(cgiOut, "  <option value=\"%d\"%s>%s</option>\n", curr_id,
                    ((formdata != NULL
                      && curr_id ==
                      formdata->artistID) ? " selected=\"selected\"" : ""),
                    title);
            count++;
            curr_id = allArtists[count];
            free(title);
        }

        free(allArtists);
    }
    fprintf(cgiOut, "</select>\n");
    fprintf(cgiOut, "</td>\n");
    fprintf(cgiOut, "  </tr>\n");
    fprintf(cgiOut, "  <tr>\n");
    fprintf(cgiOut,
            "    <td class=\"describe%s\"><label for=\"combody\" title=\"The Comment\">The <u>C</u>omment: </label></td>\n",
            ((isAdding == TRUE && errors != NULL
              && errors[1] != E_NOERROR) ? "2" : ""));
    fprintf(cgiOut, "  </tr>\n");
    fprintf(cgiOut, "  <tr>\n");
    fprintf(cgiOut,
            "    <td class=\"field\"><textarea id=\"combody\" name=\"combody\"  accesskey=\"c\" cols=\"40\" rows=\"8\">");
    if (errors != NULL && errors[1] == E_NOERROR && formdata != NULL
        && formdata->comment != NULL) {
        char *newcomment = NULL;

        newcomment = malloc(sizeof(char) * strlen(formdata->comment) + 1);
        if (newcomment == NULL) {
            /* Cannot modify comment */
            /* Just print it */
            fprintf(cgiOut, "%s", formdata->comment);
        }
        else {
            strncpy(newcomment, formdata->comment,
                    strlen(formdata->comment) + 1);

            newcomment = replaceCharWithChar(newcomment, NEWLINE_SUBST, '\n');
            fprintf(cgiOut, "%s", newcomment);

            free(newcomment);
        }
    }
    fprintf(cgiOut, "</textarea></td>\n");
    fprintf(cgiOut, "  </tr>\n");
    fprintf(cgiOut, "\n");
    fprintf(cgiOut, "  <tr>\n");
    fprintf(cgiOut,
            "    <td><input type=\"submit\" value=\"Add Comment\" /></td>\n");
    fprintf(cgiOut, "  </tr>\n");
    fprintf(cgiOut, "</tbody>\n");
    fprintf(cgiOut, "</table>\n");
    fprintf(cgiOut, "</form>\n");

    if (formdata != NULL && getArtistExists(formdata->artistID) == TRUE) {
        fprintf(cgiOut,
                "<hr /><a href=\"./?page=artist&amp;artistid=%d&amp;hash=%d\">To Artist Info page</a>\n",
                formdata->artistID, _currUserLogon);
    }
}

/*
 * Function: doViewArtistComment
 * Parameters: (void)
 * Returns: (void)
 *
 * This function lets the user view the details of a specified ArtistComment
 */
static void doViewArtistComment(void)
{
    int result = 0;
    int artistid = 0;
    int owner = 0;

    /* Check for owner field */
    result = cgiFormInteger("owner", &owner, -1);
    if (result != cgiFormSuccess || owner == -1) {
        /* Some sort of failure */
        /* check for artistid field */
        result = cgiFormInteger("artistid", &artistid, -1);
        if (result != cgiFormSuccess || artistid == -1) {
            /* No artistid specified */
            /* Print out listing of artists */
            fprintf(cgiOut,
                    "<div class=\"head1\">Viewing Artists Comments </div>\n");
            printAllArtistComments();
            return;
        }
        else {
            /* Check Artist exists */
            if (getArtistExists(artistid) == FALSE) {
                fprintf(cgiOut,
                        "<div class=\"head1\">Viewing Artists Comments </div>\n");
                fprintf(cgiOut, "Artist [%d] does not exist<br />\n",
                        artistid);
                printAllArtistComments();
                return;
            }

            /* Everyone can view these */
            printAllArtistCommentsForArtist(artistid);
        }
    }
    else {
        /* Check User exists */
        if (getUserExists(owner) == FALSE) {
            fprintf(cgiOut, "User [%d] does not exist<br />\n", owner);
            fprintf(cgiOut,
                    "<a href=\"./?page=user&amp;hash=%d\">[View All Users]</a>\n",
                    _currUserLogon);
            return;
        }

        /* Check privileges */
        if (owner == _currUserLogon
            || isUserLibrarian(_currUserLogon) == TRUE) {
            /* Will use owner */
            printAllArtistCommentsByUser(owner);
        }
        else {
            fprintf(cgiOut,
                    "You can not view the Artist Comments written by User [%d]\n",
                    owner);
        }
    }
}

/*
 * Function: printAllArtistComments
 * Parameters: (void)
 * Returns: (void)
 *
 * This function lets the user view the comments written about Artists
 */
static void printAllArtistComments(void)
{
    int *allArtists = NULL;
    int curr_id = 0;
    int count = 0;

    allArtists = getArtists();

    if (allArtists == NULL) {
        fprintf(cgiOut,
                "<div class=\"head1\">Error retrieving all Artists</div>");
    }
    else {
        if (getArtistsCount() == 0) {
            fprintf(cgiOut, "No artists<br />\n");
            if (isUserLibrarian(_currUserLogon) == TRUE) {
                fprintf(cgiOut,
                        "<a href=\"./?page=artist&amp;func=add&amp;hash=%d\">[Add new Artist]</a>\n",
                        _currUserLogon);
            }
        }
        else {
            fprintf(cgiOut,
                    "Please select an artist to view comments: <br />\n");

            fprintf(cgiOut, "<table border=\"1\">\n");
            fprintf(cgiOut, "\n");
            fprintf(cgiOut, "<thead>\n");
            fprintf(cgiOut, "  <tr>\n");
            fprintf(cgiOut, "    <td class=\"thead\">Artist Name</td>\n");
            fprintf(cgiOut, "    <td class=\"thead\">Comment Count</td>\n");
            fprintf(cgiOut, "  </tr>\n");
            fprintf(cgiOut, "</thead>\n");
            fprintf(cgiOut, "\n");
            fprintf(cgiOut, "<tfoot>\n");
            fprintf(cgiOut, "  <tr>\n");
            fprintf(cgiOut,
                    "    <td class=\"tfoot\" colspan=\"2\">&nbsp;</td>\n");
            fprintf(cgiOut, "  </tr>\n");
            fprintf(cgiOut, "</tfoot>\n");
            fprintf(cgiOut, "\n");
            fprintf(cgiOut, "<tbody>\n");

            curr_id = allArtists[count];
            while (curr_id != LAST_ID_IN_ARRAY) {
                char *artistname = getArtistName(curr_id);

                fprintf(cgiOut, "  <tr>\n");
                fprintf(cgiOut, "    <td>");
                fprintf(cgiOut,
                        "<a href=\"./?page=artistcomment&amp;artistid=%d&amp;hash=%d\">%s</a>",
                        curr_id, _currUserLogon, artistname);
                fprintf(cgiOut, "    </td>\n");
                fprintf(cgiOut, "    <td>%d</td>\n",
                        getArtistCommentsForArtistCount(curr_id));
                fprintf(cgiOut, "  </tr>\n");

                count++;
                curr_id = allArtists[count];

                free(artistname);
            }

            fprintf(cgiOut, "</tbody>\n");
            fprintf(cgiOut, "\n");
            fprintf(cgiOut, "</table>\n");
        }

        free(allArtists);
    }
}


/*
 * Function: printAllArtistCommentsByUser
 * Parameters: int
 * Returns: (void)
 *
 * This lists all the ArtistCommentsByUser in the database in a table that the user can view.
 * The method goes through the list and adds each to a table
 */
static void printAllArtistCommentsByUser(int userid)
{
    int *allArtistComments = NULL;
    int curr_id = 0;
    int count = 0;

    char *userName = getUserName(userid);
    fprintf(cgiOut,
            "<div class=\"head1\">Viewing Artist Comments written by %s</div>",
            userName);
    free(userName);

    allArtistComments = getArtistCommentsByUser(userid);

    if (allArtistComments == NULL) {
        fprintf(cgiOut,
                "<div class=\"head1\">Error retrieving all Artist Comments</div>");
    }
    else {
        if (getArtistCommentsByUserCount(userid) == 0) {
            fprintf(cgiOut, "No artist comments\n");
        }
        else {
            fprintf(cgiOut, "<table border=\"1\" class=\"tcomment\">\n");
            fprintf(cgiOut, "\n");
            fprintf(cgiOut, "<tbody>\n");

            curr_id = allArtistComments[count];
            while (curr_id != LAST_ID_IN_ARRAY) {
                int artistid = getArtistCommentArtist(curr_id);
                char *commentBody = getArtistCommentBody(curr_id);
                char *title = getArtistName(artistid);

                fprintf(cgiOut, "  <tr>\n");
                fprintf(cgiOut,
                        "    <td class=\"topper\">Comment written about ");
                fprintf(cgiOut,
                        "<a class=\"topper\" href=\"./?page=artist&amp;artistid=%d&amp;hash=%d\">%s</a>",
                        artistid, _currUserLogon, title);
                fprintf(cgiOut, "    </td>\n");
                fprintf(cgiOut, "  </tr>\n");
                fprintf(cgiOut, "  <tr>\n");
                fprintf(cgiOut, "    <td>");
                fprintf(cgiOut, "%s", commentBody);
                fprintf(cgiOut, "</td>\n");
                fprintf(cgiOut, "  </tr>\n");

                count++;
                curr_id = allArtistComments[count];

                free(commentBody);
                free(title);
            }

            fprintf(cgiOut, "</tbody>\n");
            fprintf(cgiOut, "\n");
            fprintf(cgiOut, "</table>\n");
        }

        free(allArtistComments);
    }

    fprintf(cgiOut,
            "<hr /><a href=\"./?page=usercomment&amp;userid=%d&amp;hash=%d\">Back to User Comment page</a>\n",
            userid, _currUserLogon);
    fprintf(cgiOut,
            "<hr /><a href=\"./?page=user&amp;userid=%d&amp;hash=%d\">Back to User page</a>\n",
            userid, _currUserLogon);
}

/*
 * Function: printAllArtistCommentsForArtist
 * Parameters: int
 * Returns: (void)
 *
 * This lists all the ArtistCommentsForArtist in the database in a table that the user can view.
 * The method goes through the list and adds each to a table
 */
static void printAllArtistCommentsForArtist(int artistid)
{
    int *allArtistComments = NULL;
    int curr_id = 0;
    int count = 0;

    char *title = getArtistName(artistid);
    fprintf(cgiOut,
            "<div class=\"head1\">Viewing Artist Comments written about %s</div>",
            title);
    free(title);

    allArtistComments = getArtistCommentsForArtist(artistid);

    if (allArtistComments == NULL) {
        fprintf(cgiOut,
                "<div class=\"head1\">Error retrieving all Artist Comments</div>");
    }
    else {
        /*checks whether any comments have been written about this artist */
        if (getArtistCommentsForArtistCount(artistid) == 0) {
            fprintf(cgiOut, "No artist comments\n");
        }
        else {
            fprintf(cgiOut, "<table border=\"1\" class=\"tcomment\">\n");
            fprintf(cgiOut, "\n");
            fprintf(cgiOut, "<tbody>\n");

            curr_id = allArtistComments[count];
            while (curr_id != LAST_ID_IN_ARRAY) {
                int ownerID = getArtistCommentOwner(curr_id);
                char *name = getUserName(ownerID);
                char *body = getArtistCommentBody(curr_id);

                fprintf(cgiOut, "  <tr>\n");
                fprintf(cgiOut,
                        "    <td class=\"topper\">Comment written by ");
                userLink(" class=\"topper\"", ownerID, name, cgiOut);
                fprintf(cgiOut, "    </td>\n");
                fprintf(cgiOut, "  </tr>\n");
                fprintf(cgiOut, "  <tr>\n");
                fprintf(cgiOut, "    <td>");
                fprintf(cgiOut, "%s", body);
                fprintf(cgiOut, "</td>\n");
                fprintf(cgiOut, "  </tr>\n");

                count++;
                curr_id = allArtistComments[count];

                free(body);
                free(name);
            }

            fprintf(cgiOut, "</tbody>\n");
            fprintf(cgiOut, "\n");
            fprintf(cgiOut, "</table>\n");
        }

        free(allArtistComments);
    }

    fprintf(cgiOut,
            "<hr /><a href=\"./?page=artist&amp;artistid=%d&amp;hash=%d\">Back to Artist page</a>\n",
            artistid, _currUserLogon);
}
