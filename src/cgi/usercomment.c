/*
 *
 * usercomment.c
 * ==========
 * provides functions which deal with user comments in the cgi
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

static void doAddUserComment(void);
static void doViewUserComment(void);
static void doShowUserComment(void);

static Boolean processAddForm(int *, userCommentNode_t *);
static void printAddForm(Boolean, int *, userCommentNode_t *);

static void printAllUserComments(void);
static void printAllUserCommentsByUser(int);
static void printAllUserCommentsForUser(int);

/*======================Function Definitions=============================*/

/*
 * Function: printUserComment
 * Parameters: funcName stuct
 * Returns: (void)
 *
 * checks to see if the function name is for adding an userComments
 * and checks to see if the function name is for viewing userComments
 * if not it calls the doShowUser method
 */
void printUserComment(funcName_t func)
{
    switch (func) {
    case FUNC_ADD:
        /* Do add userComment etc */
        doAddUserComment();
        break;
    case FUNC_VIEW:
        /* Do view userComment etc */
        doViewUserComment();
        break;
    default:
        /* Default */
        /* Do show userComment etc */
        doShowUserComment();
    }
}

/*
 * Function: doAddUserComment
 * Parameters: (void)
 * Returns: (void)
 *
 * This function communicates with the database and adds usercomments. Then checks that there
 * are users in the database which we can attribute the comment with. Providing there are no errors the
 * user comment is then added to the database and a message is sent back to the user - Adding Successful.
 */
static void doAddUserComment(void)
{
    Boolean needAddForm = TRUE;

    int result = 0;

    /* Temporary Struct to store form data */
    userCommentNode_t *formdata = NULL;
    /* Array to store errors */
    int *errors = NULL;
    Boolean isAdding = FALSE;

    /* Check privileges of current user */
    if (isUserLibrarian(_currUserLogon) == FALSE) {
        fprintf(cgiOut, "You are not privileged to comment on Users\n");
        return;
    }

    fprintf(cgiOut, "<div class=\"head1\">Adding New User Comment</div>\n");

    if (getUsersCount() == 0) {
        fprintf(cgiOut, "No users<br />\n");
        if (isUserLibrarian(_currUserLogon) == TRUE) {
            fprintf(cgiOut,
                    "<a href=\"./?page=user&amp;func=add&amp;hash=%d\">[Add User]</a>\n",
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
    formdata = malloc(sizeof(userCommentNode_t));
    if (formdata == NULL) {
        isAdding = FALSE;
    }
    else {
        Boolean formOK = FALSE;

        formdata->userID = -1;
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
            int newUserCommentid = -1;

            /* All form data is good */
            /* Add user comment to database */
            newUserCommentid =
                addUserComment(formdata->userID, _currUserLogon,
                               formdata->comment);

            if (newUserCommentid > 0) {
                /* User Comment added ok */
                int userid = getUserCommentUser(newUserCommentid);
                char *username = getUserName(userid);

                needAddForm = FALSE;

                fprintf(cgiOut, "Adding successful<br />\n");
                fprintf(cgiOut,
                        "<a href=\"./?page=user&amp;userid=%d&amp;hash=%d\">[View Info about &quot;%s&quot;]</a><br />\n",
                        userid, _currUserLogon, username);
                fprintf(cgiOut,
                        "<a href=\"./?page=usercomment&amp;func=view&amp;userid=%d&amp;hash=%d\">[View All Comments about &quot;%s&quot;]</a><br />\n",
                        userid, _currUserLogon, username);
                fprintf(cgiOut,
                        " <a href=\"./?page=usercomment&amp;func=add&amp;userid=%d&amp;hash=%d\">[Write another Comment about &quot;%s&quot;]</a><br />\n",
                        userid, _currUserLogon, username);
                fprintf(cgiOut,
                        " <a href=\"./?page=usercomment&amp;func=add&amp;hash=%d\">[Write a Comment about another User]</a>\n",
                        _currUserLogon);

                free(username);
            }
            else {
                /* User Comment adding error */
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
 * Parameters: int *, userCommentNode_t *
 * Returns: Boolean
 *
 * This is used to process the form that adds the new usercomment..
 */
static Boolean processAddForm(int *errors, userCommentNode_t * formdata)
{
    int result = 0;
    int size = -1;

    /* Check arguments */
    if (errors == NULL || formdata == NULL) {
        return FALSE;
    }

    /* Get User ID */
    result = cgiFormInteger("albid", &formdata->userID, -1);
    if (result != cgiFormSuccess || formdata->userID == -1) {
        errors[0] = E_FORM;
    }
    else if (getUserExists(formdata->userID) == FALSE) {
        /* No userid */
        errors[0] = E_NOUSER;
    }
    /* Get Comment body */
    result = cgiFormStringSpaceNeeded("combody", &size);
    if (result != cgiFormSuccess) {
        errors[1] = E_FORM;
    }
    else if (size > MAXLEN_USERCOMMENT + 1) {
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
 * Parameters: boolean, int *, userCommentNode_t *
 * Returns: (void)
 *
 * This function checks for errors like its too big, is invalid or already exists
 * and then once this has been processed the form(html code) can be written to the cgi output stream 
 */
static void printAddForm(Boolean isAdding, int *errors,
                         userCommentNode_t * formdata)
{
    Boolean freshForm = FALSE;

    /* Check arguments */
    if (isAdding == FALSE || errors == NULL || formdata == NULL) {
        /* Print a fresh form */
        freshForm = TRUE;
    }
    else {
        /* Process errors */

        /* User ID */
        switch (errors[0]) {
        case E_NOERROR:
            break;
        case E_FORM:
            fprintf(cgiOut, "User not selected<br />\n");
            break;
        case E_NOUSER:
            fprintf(cgiOut, "User [%d] does not exist<br />\n",
                    formdata->userID);
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
            "    <input type=\"hidden\" name=\"page\" value=\"usercomment\" />\n");
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
            "    <td class=\"describe%s\"><label title=\"User Name\">User <u>N</u>ame: </label></td>\n",
            ((isAdding == TRUE && errors != NULL
              && errors[0] != E_NOERROR) ? "2" : ""));
    fprintf(cgiOut, "  </tr>\n");


    fprintf(cgiOut, "  <tr>\n");
    fprintf(cgiOut, "    <td class=\"field\">\n");
    /* Print out the <select> for all users */
    fprintf(cgiOut,
            "<select id=\"albid\" name=\"albid\" size=\"5\" accesskey=\"n\">\n");
    fprintf(cgiOut, "  <option value=\"-1\"%s>&nbsp;</option>\n",
            ((formdata == NULL
              || getUserExists(formdata->userID) ==
              FALSE) ? " selected=\"selected\"" : ""));
    {
        int *allUsers = getUsers();
        int count = 0;
        int curr_id = 0;

        curr_id = allUsers[count];
        while (curr_id != LAST_ID_IN_ARRAY) {
            char *title = getUserName(curr_id);
            fprintf(cgiOut, "  <option value=\"%d\"%s>%s</option>\n", curr_id,
                    ((formdata != NULL
                      && curr_id ==
                      formdata->userID) ? " selected=\"selected\"" : ""),
                    title);
            count++;
            curr_id = allUsers[count];
            free(title);
        }

        free(allUsers);
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

    if (formdata != NULL && getUserExists(formdata->userID) == TRUE) {
        fprintf(cgiOut,
                "<hr /><a href=\"./?page=user&amp;userid=%d&amp;hash=%d\">To User Info page</a>\n",
                formdata->userID, _currUserLogon);
    }
}

/*
 * Function: doViewUserComment
 * Parameters: (void)
 * Returns: (void)
 *
 * This function lets the user view the details of a specified UserComment
 */
static void doViewUserComment(void)
{
    int result = 0;
    int userid = 0;
    int owner = 0;

    /* Check for owner field */
    result = cgiFormInteger("owner", &owner, -1);
    if (result != cgiFormSuccess || owner == -1) {
        /* Some sort of failure */
        /* check for userid field */
        result = cgiFormInteger("userid", &userid, -1);
        if (result != cgiFormSuccess || userid == -1) {
            /* No userid specified */
            /* Print out listing of users */
            fprintf(cgiOut,
                    "<div class=\"head1\">Viewing Users Comments </div>\n");
            printAllUserComments();
            return;
        }
        else {
            /* Check User exists */
            if (getUserExists(userid) == FALSE) {
                fprintf(cgiOut,
                        "<div class=\"head1\">Viewing Users Comments </div>\n");
                fprintf(cgiOut, "User [%d] does not exist<br />\n", userid);
                printAllUserComments();
                return;
            }

            /* Check privileges */
            if (userid == _currUserLogon
                || isUserLibrarian(_currUserLogon) == TRUE) {
                /* Will use userid */
                printAllUserCommentsForUser(userid);
            }
            else {
                fprintf(cgiOut,
                        "You can not view the User Comments written by User [%d]\n",
                        owner);
            }
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
            printAllUserCommentsByUser(owner);
        }
        else {
            fprintf(cgiOut,
                    "You can not view the User Comments written by User [%d]\n",
                    owner);
        }
    }
}

/*
 * Function: printAllUserComments
 * Parameters: (void)
 * Returns: (void)
 *
 * This function lets the user view the comments written by or about Users
 */
static void printAllUserComments(void)
{
    int *allUsers = NULL;
    int curr_id = 0;
    int count = 0;

    allUsers = getUsers();

    if (allUsers == NULL) {
        fprintf(cgiOut,
                "<div class=\"head1\">Error retrieving all Users</div>");
    }
    else {
        if (getUsersCount() == 0) {
            fprintf(cgiOut, "No users<br />\n");
            if (isUserLibrarian(_currUserLogon) == TRUE) {
                fprintf(cgiOut,
                        "<a href=\"./?page=user&amp;func=add&amp;hash=%d\">[Add new User]</a>\n",
                        _currUserLogon);
            }
        }
        else {
            fprintf(cgiOut,
                    "Please select an user to view comments: <br />\n");

            fprintf(cgiOut, "<table border=\"1\">\n");
            fprintf(cgiOut, "\n");
            fprintf(cgiOut, "<thead>\n");
            fprintf(cgiOut, "  <tr>\n");
            fprintf(cgiOut, "    <td class=\"thead\">User Name</td>\n");
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

            curr_id = allUsers[count];
            while (curr_id != LAST_ID_IN_ARRAY) {
                char *username = getUserName(curr_id);

                fprintf(cgiOut, "  <tr>\n");
                fprintf(cgiOut, "    <td>");
                fprintf(cgiOut,
                        "<a href=\"./?page=usercomment&amp;userid=%d&amp;hash=%d\">%s</a>",
                        curr_id, _currUserLogon, username);
                fprintf(cgiOut, "    </td>\n");
                fprintf(cgiOut, "    <td>%d</td>\n",
                        getUserCommentsForUserCount(curr_id) +
                        getUserCommentsByUserCount(curr_id) +
                        getAlbumCommentsByUserCount(curr_id) +
                        getArtistCommentsByUserCount(curr_id));
                fprintf(cgiOut, "  </tr>\n");

                count++;
                curr_id = allUsers[count];

                free(username);
            }

            fprintf(cgiOut, "</tbody>\n");
            fprintf(cgiOut, "\n");
            fprintf(cgiOut, "</table>\n");
        }

        free(allUsers);
    }
}

/*
 * Function: printAllUserCommentsByUser
 * Parameters: int
 * Returns: (void)
 *
 * This lists all the UserCommentsByUser in the database in a table that the user can view.
 * The method goes through the list and adds each to a table
 */
static void printAllUserCommentsByUser(int userid)
{
    int *allUserComments = NULL;
    int curr_id = 0;
    int count = 0;

    char *userName = getUserName(userid);
    fprintf(cgiOut,
            "<div class=\"head1\">Viewing User Comments written by %s</div>",
            userName);
    free(userName);

    allUserComments = getUserCommentsByUser(userid);

    if (allUserComments == NULL) {
        fprintf(cgiOut,
                "<div class=\"head1\">Error retrieving all User Comments</div>");
    }
    else {
        if (getUserCommentsByUserCount(userid) == 0) {
            fprintf(cgiOut, "No user comments\n");
        }
        else {
            fprintf(cgiOut, "<table border=\"1\" class=\"tcomment\">\n");
            fprintf(cgiOut, "\n");
            fprintf(cgiOut, "<tbody>\n");

            curr_id = allUserComments[count];
            while (curr_id != LAST_ID_IN_ARRAY) {
                int userid = getUserCommentUser(curr_id);
                char *commentBody = getUserCommentBody(curr_id);
                char *title = getUserName(userid);

                fprintf(cgiOut, "  <tr>\n");
                fprintf(cgiOut,
                        "    <td class=\"topper\">Comment written about ");
                fprintf(cgiOut,
                        "<a class=\"topper\" href=\"./?page=user&amp;userid=%d&amp;hash=%d\">%s</a>",
                        userid, _currUserLogon, title);
                fprintf(cgiOut, "    </td>\n");
                fprintf(cgiOut, "  </tr>\n");
                fprintf(cgiOut, "  <tr>\n");
                fprintf(cgiOut, "    <td>");
                fprintf(cgiOut, "%s", commentBody);
                fprintf(cgiOut, "</td>\n");
                fprintf(cgiOut, "  </tr>\n");

                count++;
                curr_id = allUserComments[count];

                free(commentBody);
                free(title);
            }

            fprintf(cgiOut, "</tbody>\n");
            fprintf(cgiOut, "\n");
            fprintf(cgiOut, "</table>\n");
        }

        free(allUserComments);
    }

    fprintf(cgiOut,
            "<hr /><a href=\"./?page=usercomment&amp;userid=%d&amp;hash=%d\">Back to User Comment page</a>\n",
            userid, _currUserLogon);
    fprintf(cgiOut,
            "<hr /><a href=\"./?page=user&amp;userid=%d&amp;hash=%d\">Back to User page</a>\n",
            userid, _currUserLogon);
}

/*
 * Function: printAllUserCommentsForUser
 * Parameters: int
 * Returns: (void)
 * 
 * function prints all the comments written about this user
 * This lists all the UserCommentsForUser in the database in a table that the user can view.
 * The function goes through the list and adds each to a table
 */
static void printAllUserCommentsForUser(int userid)
{
    int *allUserComments = NULL;
    int curr_id = 0;
    int count = 0;

    char *title = getUserName(userid);
    fprintf(cgiOut,
            "<div class=\"head1\">Viewing User Comments written about %s</div>",
            title);
    free(title);

    allUserComments = getUserCommentsForUser(userid);

    if (allUserComments == NULL) {
        fprintf(cgiOut,
                "<div class=\"head1\">Error retrieving all User Comments</div>");
    }
    else {
        /*checks whether any comments have been written about this user */
        if (getUserCommentsForUserCount(userid) == 0) {
            fprintf(cgiOut, "No user comments\n");
        }
        else {
            fprintf(cgiOut, "<table border=\"1\" class=\"tcomment\">\n");
            fprintf(cgiOut, "\n");
            fprintf(cgiOut, "<tbody>\n");

            curr_id = allUserComments[count];
            while (curr_id != LAST_ID_IN_ARRAY) {
                int ownerID = getUserCommentOwner(curr_id);
                char *name = getUserName(ownerID);
                char *body = getUserCommentBody(curr_id);

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
                curr_id = allUserComments[count];

                free(body);
                free(name);
            }

            fprintf(cgiOut, "</tbody>\n");
            fprintf(cgiOut, "\n");
            fprintf(cgiOut, "</table>\n");
        }

        free(allUserComments);
    }

    fprintf(cgiOut,
            "<hr /><a href=\"./?page=user&amp;userid=%d&amp;hash=%d\">Back to User page</a>\n",
            userid, _currUserLogon);
}

/*
 * Function: doShowUserComment
 * Parameters: (void)
 * Returns: (void)
 * 
 * This function first checks the user exsists then it finds what privileges it has.
 * Once this is done it will show the comments that have been said about this user.
 */
static void doShowUserComment(void)
{
    int result = 0;
    int userid = 0;

    /* check for userid field */
    result = cgiFormInteger("userid", &userid, -1);
    if (result != cgiFormSuccess || userid == -1) {
        /* Some sort of failure */
        /* Nothing specified */
        fprintf(cgiOut, "<div class=\"head1\">Viewing User Comments</div>\n");
        printAllUserComments();
        return;
    }
    else {
        /* Check User exists */
        if (getUserExists(userid) == FALSE) {
            fprintf(cgiOut, "User [%d] does exist<br />\n", userid);
            fprintf(cgiOut,
                    "<a href=\"./?page=user&amp;hash=%d\">[View All Users]</a>\n",
                    _currUserLogon);
            return;
        }

        /* Check privileges */
        if (userid != _currUserLogon
            && isUserLibrarian(_currUserLogon) == FALSE) {
            fprintf(cgiOut,
                    "You can not view info about Comments by/about User [%d]\n",
                    userid);
            return;
        }
    }

    {
        /* Will use userid */
        char *userCode = getUserCode(userid);
        char *userName = getUserName(userid);

        fprintf(cgiOut,
                "<div class=\"head1\">Comments written by and written about %s</div>\n",
                userName);

        fprintf(cgiOut,
                "<p><a href=\"./?page=usercomment&amp;func=view&amp;userid=%d&amp;hash=%d\">View Comments about <b>%s</b> (%d)</a></p>\n\n",
                userid, _currUserLogon, userCode,
                getUserCommentsForUserCount(userid));

        if (isUserLibrarian(userid) == TRUE) {
            fprintf(cgiOut,
                    "<p><a href=\"./?page=usercomment&amp;func=view&amp;owner=%d&amp;hash=%d\">View Comments <b>%s</b> has written about Users (%d)</a></p>\n\n",
                    userid, _currUserLogon, userCode,
                    getUserCommentsByUserCount(userid));
        }

        fprintf(cgiOut,
                "<p><a href=\"./?page=albumcomment&amp;func=view&amp;owner=%d&amp;hash=%d\">View Comments <b>%s</b> has written about Albums (%d)</a></p>\n\n",
                userid, _currUserLogon, userCode,
                getAlbumCommentsByUserCount(userid));

        fprintf(cgiOut,
                "<p><a href=\"./?page=artistcomment&amp;func=view&amp;owner=%d&amp;hash=%d\">View Comments <b>%s</b> has written about Artists (%d)</a></p>\n\n",
                userid, _currUserLogon, userCode,
                getArtistCommentsByUserCount(userid));

        fprintf(cgiOut,
                "<hr /><a href=\"./?page=user&amp;userid=%d&amp;hash=%d\">Back to User page</a>\n",
                userid, _currUserLogon);

        free(userCode);
        free(userName);
    }
}
