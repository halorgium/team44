/*
 *
 * albumcomment.c
 * ==========
 * provides functions which deal with album comments in the cgi
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


static void doAddAlbumComment(void);
static void doViewAlbumComment(void);

static Boolean processAddForm(int *, albumCommentNode_t *);
static void printAddForm(Boolean, int *, albumCommentNode_t *);

static void printAllAlbumComments(void);
static void printAllAlbumCommentsByUser(int);
static void printAllAlbumCommentsForAlbum(int);


/*======================Function Definitions=============================*/

/*
 * Function: printAlbumComment
 * Parameters: funcName stuct
 * Returns: (void)
 *
 * checks to see if the function name is for adding an artist.
 * if not it calls the doViewAlbum method
 */
void printAlbumComment(funcName_t func)
{
    switch (func) {
    case FUNC_ADD:
        /* Do add albumComment etc */
        doAddAlbumComment();
        break;
    default:
        /* Do view albumComment etc */
        doViewAlbumComment();
        break;
    }
}

/*
 * Function: doAddAlbumComment
 * Parameters: (void)
 * Returns: (void)
 *
 * This function communicates with the database and adds albumcomments. Then checks that there
 * are albums in the database which we can attribute the comment with. Providing there are no errors the
 * album comment is then added to the database and a message is sent back to the user - Adding Successful.
 */
static void doAddAlbumComment(void)
{
    Boolean needAddForm = TRUE;

    int result = 0;

    /* Temporary Struct to store form data */
    albumCommentNode_t *formdata = NULL;
    /* Array to store errors */
    int *errors = NULL;
    Boolean isAdding = FALSE;

    fprintf(cgiOut, "<div class=\"head1\">Adding New Album Comment</div>\n");

    if (getAlbumsCount() == 0) {
        fprintf(cgiOut, "No albums<br />\n");
        if (isUserLibrarian(_currUserLogon) == TRUE) {
            fprintf(cgiOut,
                    "<a href=\"./?page=album&amp;func=add&amp;hash=%d\">[Add Album]</a>\n",
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
    formdata = malloc(sizeof(albumCommentNode_t));
    if (formdata == NULL) {
        isAdding = FALSE;
    }
    else {
        Boolean formOK = FALSE;

        formdata->albumID = -1;
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
            int newAlbumCommentid = -1;

            /* All form data is good */
            /* Add album comment to database */
            newAlbumCommentid =
                addAlbumComment(formdata->albumID, _currUserLogon,
                                formdata->comment);

            if (newAlbumCommentid > 0) {
                /* Album Comment added ok */
                int albumid = getAlbumCommentAlbum(newAlbumCommentid);
                char *albumtitle = getAlbumTitle(albumid);

                needAddForm = FALSE;

                fprintf(cgiOut, "Adding successful<br />\n");
                fprintf(cgiOut,
                        "<a href=\"./?page=album&amp;albumid=%d&amp;hash=%d\">[View Info about &quot;%s&quot;]</a><br />\n",
                        albumid, _currUserLogon, albumtitle);
                fprintf(cgiOut,
                        "<a href=\"./?page=albumcomment&amp;func=view&amp;albumid=%d&amp;hash=%d\">[View All Comments about &quot;%s&quot;]</a><br />\n",
                        albumid, _currUserLogon, albumtitle);
                fprintf(cgiOut,
                        " <a href=\"./?page=albumcomment&amp;func=add&amp;albumid=%d&amp;hash=%d\">[Write another Comment about &quot;%s&quot;]</a><br />\n",
                        albumid, _currUserLogon, albumtitle);
                fprintf(cgiOut,
                        " <a href=\"./?page=albumcomment&amp;func=add&amp;hash=%d\">[Write a Comment about another Album]</a>\n",
                        _currUserLogon);

                free(albumtitle);
            }
            else {
                /* Album Comment adding error */
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
 * Parameters: int *, albumCommentNode_t *
 * Returns: Boolean
 *
 * This is used to process the form that adds the new albumcomment..
 */
static Boolean processAddForm(int *errors, albumCommentNode_t * formdata)
{
    int result = 0;
    int size = -1;

    /* Check arguments */
    if (errors == NULL || formdata == NULL) {
        return FALSE;
    }

    /* Get Album ID */
    result = cgiFormInteger("albid", &formdata->albumID, -1);
    if (result != cgiFormSuccess || formdata->albumID == -1) {
        errors[0] = E_FORM;
    }
    else if (getAlbumExists(formdata->albumID) == FALSE) {
        /* No albumid */
        errors[0] = E_NOALBUM;
    }
    /* Get Comment body */
    result = cgiFormStringSpaceNeeded("combody", &size);
    if (result != cgiFormSuccess) {
        errors[1] = E_FORM;
    }
    else if (size > MAXLEN_ALBUMCOMMENT + 1) {
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
 * Function: printAddForm
 * Parameters: Boolean, int *, albumCommentNode_t *
 * Returns: (void)
 *
 * This function checks for errors like its too big, is invalid or already exist
 * and then once this has been processed the form(html code) can be written to the cgi output stream 
 */
static void printAddForm(Boolean isAdding, int *errors,
                         albumCommentNode_t * formdata)
{
    Boolean freshForm = FALSE;

    /* Check arguments */
    if (isAdding == FALSE || errors == NULL || formdata == NULL) {
        /* Print a fresh form */
        freshForm = TRUE;
    }
    else {
        /* Process errors */

        /* Album ID */
        switch (errors[0]) {
        case E_NOERROR:
            break;
        case E_FORM:
            fprintf(cgiOut, "Album not selected<br />\n");
            break;
        case E_NOALBUM:
            fprintf(cgiOut, "Album [%d] does not exist<br />\n",
                    formdata->albumID);
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
            "    <input type=\"hidden\" name=\"page\" value=\"albumcomment\" />\n");
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
            "    <td class=\"describe%s\"><label title=\"Album Title\">Album <u>T</u>itle: </label></td>\n",
            ((isAdding == TRUE && errors != NULL
              && errors[0] != E_NOERROR) ? "2" : ""));
    fprintf(cgiOut, "  </tr>\n");


    fprintf(cgiOut, "  <tr>\n");
    fprintf(cgiOut, "    <td class=\"field\">\n");
    /* Print out the <select> for all albums */
    fprintf(cgiOut,
            "<select id=\"albid\" name=\"albid\" size=\"5\" accesskey=\"t\">\n");
    fprintf(cgiOut, "  <option value=\"-1\"%s>&nbsp;</option>\n",
            ((formdata == NULL
              || getAlbumExists(formdata->albumID) ==
              FALSE) ? " selected=\"selected\"" : ""));
    {
        int *allAlbums = getAlbums();
        int count = 0;
        int curr_id = 0;

        curr_id = allAlbums[count];
        while (curr_id != LAST_ID_IN_ARRAY) {
            char *title = getAlbumTitle(curr_id);
            fprintf(cgiOut, "  <option value=\"%d\"%s>%s</option>\n", curr_id,
                    ((formdata != NULL
                      && curr_id ==
                      formdata->albumID) ? " selected=\"selected\"" : ""),
                    title);
            count++;
            curr_id = allAlbums[count];
            free(title);
        }

        free(allAlbums);
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

    if (formdata != NULL && getAlbumExists(formdata->albumID) == TRUE) {
        fprintf(cgiOut,
                "<hr /><a href=\"./?page=album&amp;albumid=%d&amp;hash=%d\">To Album Info page</a>\n",
                formdata->albumID, _currUserLogon);
    }
}

/*
 * Function: doViewAlbumComment
 * Parameters: (void)
 * Returns: (void)
 *
 * This function lets the user view the details of a specified AlbumComment
 */
static void doViewAlbumComment(void)
{
    int result = 0;
    int albumid = 0;
    int owner = 0;

    /* Check for owner field */
    result = cgiFormInteger("owner", &owner, -1);
    if (result != cgiFormSuccess || owner == -1) {
        /* Some sort of failure */
        /* check for albumid field */
        result = cgiFormInteger("albumid", &albumid, -1);
        if (result != cgiFormSuccess || albumid == -1) {
            /* No albumid specified */
            /* Print out listing of albums */
            fprintf(cgiOut,
                    "<div class=\"head1\">Viewing Albums Comments </div>\n");
            printAllAlbumComments();
            return;
        }
        else {
            /* Check Album exists */
            if (getAlbumExists(albumid) == FALSE) {
                fprintf(cgiOut,
                        "<div class=\"head1\">Viewing Albums Comments </div>\n");
                fprintf(cgiOut, "Album [%d] does not exist<br />\n", albumid);
                printAllAlbumComments();
                return;
            }

            /* Everyone can view these */
            printAllAlbumCommentsForAlbum(albumid);
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
            printAllAlbumCommentsByUser(owner);
        }
        else {
            fprintf(cgiOut,
                    "You can not view the Album Comments written by User [%d]\n",
                    owner);
        }
    }
}

/*
 * Function: printAllAlbumComments
 * Parameters: (void)
 * Returns: (void)
 *
 * This function lets the user view the comments written about Albums
 */
static void printAllAlbumComments(void)
{
    int *allAlbums = NULL;
    int curr_id = 0;
    int count = 0;

    allAlbums = getAlbums();

    if (allAlbums == NULL) {
        fprintf(cgiOut,
                "<div class=\"head1\">Error retrieving all Albums</div>");
    }
    else {
        if (getAlbumsCount() == 0) {
            fprintf(cgiOut, "No albums<br />\n");
            if (isUserLibrarian(_currUserLogon) == TRUE) {
                fprintf(cgiOut,
                        "<a href=\"./?page=album&amp;func=add&amp;hash=%d\">[Add new Album]</a>\n",
                        _currUserLogon);
            }
        }
        else {
            fprintf(cgiOut,
                    "Please select an album to view comments: <br />\n");

            fprintf(cgiOut, "<table border=\"1\">\n");
            fprintf(cgiOut, "\n");
            fprintf(cgiOut, "<thead>\n");
            fprintf(cgiOut, "  <tr>\n");
            fprintf(cgiOut, "    <td class=\"thead\">Album Title</td>\n");
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

            curr_id = allAlbums[count];
            while (curr_id != LAST_ID_IN_ARRAY) {
                char *albumTitle = getAlbumTitle(curr_id);

                fprintf(cgiOut, "  <tr>\n");
                fprintf(cgiOut, "    <td>");
                fprintf(cgiOut,
                        "<a href=\"./?page=albumcomment&amp;albumid=%d&amp;hash=%d\">%s</a>",
                        curr_id, _currUserLogon, albumTitle);
                fprintf(cgiOut, "    </td>\n");
                fprintf(cgiOut, "    <td>%d</td>\n",
                        getAlbumCommentsForAlbumCount(curr_id));
                fprintf(cgiOut, "  </tr>\n");

                count++;
                curr_id = allAlbums[count];

                free(albumTitle);
            }

            fprintf(cgiOut, "</tbody>\n");
            fprintf(cgiOut, "\n");
            fprintf(cgiOut, "</table>\n");
        }

        free(allAlbums);
    }
}

/*
 * Function: printAllAlbumCommentsByUser
 * Parameters: int
 * Returns: (void)
 *
 * This lists all the AlbumCommentsByUser in the database in a table that the user can view.
 * The method goes through the list and adds each to a table
 */
static void printAllAlbumCommentsByUser(int userid)
{
    int *allAlbumComments = NULL;
    int curr_id = 0;
    int count = 0;

    char *userName = getUserName(userid);
    fprintf(cgiOut,
            "<div class=\"head1\">Viewing Album Comments written by %s</div>",
            userName);
    free(userName);

    allAlbumComments = getAlbumCommentsByUser(userid);

    if (allAlbumComments == NULL) {
        fprintf(cgiOut,
                "<div class=\"head1\">Error retrieving all Album Comments</div>");
    }
    else {
        if (getAlbumCommentsByUserCount(userid) == 0) {
            fprintf(cgiOut, "No album comments\n");
        }
        else {
            fprintf(cgiOut, "<table border=\"1\" class=\"tcomment\">\n");
            fprintf(cgiOut, "\n");
            fprintf(cgiOut, "<tbody>\n");

            curr_id = allAlbumComments[count];
            while (curr_id != LAST_ID_IN_ARRAY) {
                int albumid = getAlbumCommentAlbum(curr_id);
                char *commentBody = getAlbumCommentBody(curr_id);
                char *title = getAlbumTitle(albumid);

                fprintf(cgiOut, "  <tr>\n");
                fprintf(cgiOut,
                        "    <td class=\"topper\">Comment written about ");
                fprintf(cgiOut,
                        "<a class=\"topper\" href=\"./?page=album&amp;albumid=%d&amp;hash=%d\">%s</a>",
                        albumid, _currUserLogon, title);
                fprintf(cgiOut, "    </td>\n");
                fprintf(cgiOut, "  </tr>\n");
                fprintf(cgiOut, "  <tr>\n");
                fprintf(cgiOut, "    <td>");
                fprintf(cgiOut, "%s", commentBody);
                fprintf(cgiOut, "</td>\n");
                fprintf(cgiOut, "  </tr>\n");

                count++;
                curr_id = allAlbumComments[count];

                free(commentBody);
                free(title);
            }

            fprintf(cgiOut, "</tbody>\n");
            fprintf(cgiOut, "\n");
            fprintf(cgiOut, "</table>\n");
        }

        free(allAlbumComments);
    }

    fprintf(cgiOut,
            "<hr /><a href=\"./?page=usercomment&amp;userid=%d&amp;hash=%d\">Back to User Comment page</a>\n",
            userid, _currUserLogon);
    fprintf(cgiOut,
            "<hr /><a href=\"./?page=user&amp;userid=%d&amp;hash=%d\">Back to User page</a>\n",
            userid, _currUserLogon);
}


/*
 * Function: printAllAlbumCommentsForAlbum
 * Parameters: int
 * Returns: (void)
 * 
 * function prints all the comments written about this album
 * This lists all the AlbumCommentsForAlbum in the database in a table that the user can view.
 * The function goes through the list and adds each to a table
 */
static void printAllAlbumCommentsForAlbum(int albumid)
{
    int *allAlbumComments = NULL;
    int curr_id = 0;
    int count = 0;

    char *title = getAlbumTitle(albumid);
    fprintf(cgiOut,
            "<div class=\"head1\">Viewing Album Comments written about %s</div>",
            title);
    free(title);

    allAlbumComments = getAlbumCommentsForAlbum(albumid);

    if (allAlbumComments == NULL) {
        fprintf(cgiOut,
                "<div class=\"head1\">Error retrieving all Album Comments</div>");
    }
    else {
        /*checks whether any comments have been written about this album */
        if (getAlbumCommentsForAlbumCount(albumid) == 0) {
            fprintf(cgiOut, "No album comments\n");
        }
        else {
            fprintf(cgiOut, "<table border=\"1\" class=\"tcomment\">\n");
            fprintf(cgiOut, "\n");
            fprintf(cgiOut, "<tbody>\n");

            curr_id = allAlbumComments[count];
            while (curr_id != LAST_ID_IN_ARRAY) {
                int ownerID = getAlbumCommentOwner(curr_id);
                char *name = getUserName(ownerID);
                char *body = getAlbumCommentBody(curr_id);

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
                curr_id = allAlbumComments[count];

                free(body);
                free(name);
            }

            fprintf(cgiOut, "</tbody>\n");
            fprintf(cgiOut, "\n");
            fprintf(cgiOut, "</table>\n");
        }

        free(allAlbumComments);
    }

    fprintf(cgiOut,
            "<hr /><a href=\"./?page=album&amp;albumid=%d&amp;hash=%d\">Back to Album page</a>\n",
            albumid, _currUserLogon);
}
