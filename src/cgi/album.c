/*
 *
 * album.c
 * ==========
 * provides functions which deal with albums in the cgi
 * 
*/

/*===================== Preprocessor statements===========================*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "cgic.h"
#include "globals.h"
#include "../shared/defines.h"
#include "../database/structs.h"

/*======================Function Declarations=============================*/

static void doAddAlbum(void);
static void doViewAlbum(void);

static Boolean processAddForm(int *, albumNode_t *);
static void printAddForm(Boolean, int *, albumNode_t *);

static void printAllAlbums(void);
static void printSpecificAlbum(int);

/*======================Function Definitions=============================*/

/*
 * Function: printAlbum
 * Parameters: funcName stuct
 * Returns: (void)
 *
 * checks to see if the function name is for adding an album.
 * if not it calls the doViewAlbum function
 */
void printAlbum(funcName_t func)
{
    switch (func) {
    case FUNC_ADD:
        /* Do add album etc */
        doAddAlbum();
        break;
    default:
        /* Default */
        /* Do view album etc */
        doViewAlbum();
    }
}

/*
 * Function: doAddAlbum
 * Parameters: (void)
 * Returns: (void)
 *
 * This function communicates with the database and adds albums. Checks privileges of current user
 * to see if they are a librarian. Because only librarians can add Albums. Then the ablum checks that there
 * are artists in the database which we can attribute the album with. Providing there are no errors the album
 * is then added to the database and a message is sent back to the user - Adding Successful.
 */
static void doAddAlbum(void)
{
    Boolean needAddForm = TRUE;

    int result = 0;

    /* Temporary Struct to store form data */
    albumNode_t *formdata = NULL;
    /* Array to store errors */
    int *errors = NULL;
    Boolean isAdding = FALSE;

    /* Check privileges of current user */
    if (isUserLibrarian(_currUserLogon) == FALSE) {
        fprintf(cgiOut, "You are not privileged to add new Albums\n");
        return;
    }

    fprintf(cgiOut, "<div class=\"head1\">Adding New Album</div>\n");

    if (getArtistsCount() == 0) {
        fprintf(cgiOut,
                "To be able to add an album, the database must contain an artist<br />\n");
        fprintf(cgiOut,
                "<a href=\"./?page=artist&amp;func=add&amp;hash=%d\">[Add Artist]</a>\n",
                _currUserLogon);
        return;
    }

    /* if adding field is set */
    result = cgiFormIntegerBounded("adding", &isAdding, FALSE, TRUE, FALSE);
    if (result != cgiFormSuccess) {
        /* Some sort of failure */
        isAdding = FALSE;
    }

    /* Malloc space for form data */
    formdata = malloc(sizeof(albumNode_t));
    if (formdata == NULL) {
        isAdding = FALSE;
    }
    else {
        Boolean formOK = FALSE;

        formdata->title = NULL;
        formdata->artistID = -1;

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
            int newAlbumid = -1;

            /* All form data is good */
            /* Add album to database */
            newAlbumid = addAlbum(formdata->title, formdata->artistID);

            if (newAlbumid > 0) {
                /* Album added ok */
                needAddForm = FALSE;
                fprintf(cgiOut, "Adding successful<br />\n");
                fprintf(cgiOut,
                        "<a href=\"./?page=album&amp;albumid=%d&amp;hash=%d\">[View Album]</a><br />\n",
                        newAlbumid, _currUserLogon);
                fprintf(cgiOut,
                        "<a href=\"./?page=album&amp;func=add&amp;artistid=%d&amp;hash=%d\">[Add another Album by the same Artist]</a><br />\n",
                        getAlbumArtist(newAlbumid), _currUserLogon);
                fprintf(cgiOut,
                        "<a href=\"./?page=album&amp;func=add&amp;hash=%d\">[Add another Album by a different Artist]</a>\n",
                        _currUserLogon);
            }
            else {
                /* Album adding error */
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
        if (formdata->title != NULL) {
            free(formdata->title);
        }
        free(formdata);
    }
}

/*
 * Function: processAddForm
 * Parameters: int, struct
 * Returns: Boolean
 *
 * This is used to process the form that adds the new album. It communicates with the database and checks 
 * whether the album being added has already been added before.
 */
static Boolean processAddForm(int *errors, albumNode_t * formdata)
{
    int result = 0;
    int size = -1;

    /* Check arguments */
    if (errors == NULL || formdata == NULL) {
        return FALSE;
    }

    /* Get Album title */
    result = cgiFormStringSpaceNeeded("albtitle", &size);
    if (result != cgiFormSuccess) {
        errors[0] = E_FORM;
    }
    else if (size > MAXLEN_ALBUMTITLE + 1) {
        errors[0] = E_TOOBIG;
    }
    else {
        formdata->title = malloc(sizeof(char) * size);
        if (formdata->title == NULL) {
            errors[0] = E_MALLOC_FAILED;
        }
        else {
            result =
                cgiFormStringNoNewlines("albtitle", formdata->title, size);
            if (result != cgiFormSuccess) {
                errors[0] = E_FORM;
            }
            else if (checkString2(formdata->title) == FALSE) {
                errors[0] = E_INVALID_PARAM;
            }
            else {
                /*pointer to list of albums */
                int *allAlbums = getAlbums();
                int i;          /*counter */

                /*check for album 'already in' library */

                for (i = 0; allAlbums[i] != LAST_ID_IN_ARRAY; i++) {
                    char *albumTitle = getAlbumTitle(allAlbums[i]);
                    if (strcmp(albumTitle, formdata->title) == 0 &&
                        formdata->artistID == getAlbumArtist(allAlbums[i])) {
                        /*album added is 'the same' as another in database */
                        errors[0] = ALREADY_ADDED;
                    }
                    free(albumTitle);
                }
                free(allAlbums);
            }
        }
    }
    /* Get Artist ID */
    result = cgiFormInteger("artistid", &formdata->artistID, -1);
    if (result != cgiFormSuccess || formdata->artistID == -1) {
        errors[1] = E_FORM;
    }
    else if (getArtistExists(formdata->artistID) == FALSE) {
        /* No artistid */
        errors[1] = E_NOARTIST;
    }

    if (errors[0] != E_NOERROR || errors[1] != E_NOERROR) {
        return FALSE;
    }
    return TRUE;
}


/*
 * Function: printAddForm
 * Parameters: int, int*, struct
 * Returns: (void)
 *
 * This function checks for errors like Album Title is too big, Album Title is invalid or already exist
 * and then once this has been processed the form (html code) can be written to the cgi output stream 
 */
static void printAddForm(Boolean isAdding, int *errors,
                         albumNode_t * formdata)
{
    Boolean freshForm = FALSE;

    /* Check arguments */
    if (isAdding == FALSE || errors == NULL || formdata == NULL) {
        /* Print a fresh form */
        freshForm = TRUE;
    }
    else {
        /* Process errors */
        char *artistName = NULL;

        /* Album title */
        switch (errors[0]) {
        case E_NOERROR:
            break;
        case E_FORM:
            fprintf(cgiOut, "Album Title is empty<br />\n");
            break;
        case E_TOOBIG:
            fprintf(cgiOut, "Album Title is too big<br />\n");
            break;
        case E_INVALID_PARAM:
            fprintf(cgiOut, "Album Title is invalid<br />\n");
            break;
        case ALREADY_ADDED:
            artistName = getArtistName(formdata->artistID);
            fprintf(cgiOut,
                    "Album &quot;%s&quot; by %s already exists<br />\n",
                    formdata->title, artistName);
            free(artistName);
            break;
        case E_MALLOC_FAILED:
            fprintf(cgiOut, "Memory Allocation Error<br />\n");
            break;
        default:
            fprintf(cgiOut, "Unknown form error<br />\n");
            break;
        }

        /* Artist ID */
        switch (errors[1]) {
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
    }

    fprintf(cgiOut, "<form method=\"get\" action=\"./\">\n");
    fprintf(cgiOut, "<table>\n");
    fprintf(cgiOut, "<tbody>\n");
    fprintf(cgiOut, "  <tr class=\"hidden\">\n");
    fprintf(cgiOut, "    <td>\n");
    fprintf(cgiOut,
            "    <input type=\"hidden\" name=\"page\" value=\"album\" />\n");
    fprintf(cgiOut,
            "    <input type=\"hidden\" name=\"func\" value=\"add\" />\n");
    fprintf(cgiOut,
            "    <input type=\"hidden\" name=\"adding\" value=\"%d\" />\n",
            TRUE);
    fprintf(cgiOut,
            "    <input type=\"hidden\" name=\"hash\" value=\"%d\" />\n",
            _currUserLogon);
    fprintf(cgiOut, "    </td>\n");
    fprintf(cgiOut, "  </tr>\n");
    fprintf(cgiOut, "  <tr>\n");
    fprintf(cgiOut,
            "    <td class=\"describe%s\"><label for=\"albtitle\" title=\"Album Title\">Album <u>T</u>itle: </label></td>\n",
            ((isAdding == TRUE && errors != NULL
              && errors[0] != E_NOERROR) ? "2" : ""));
    fprintf(cgiOut, "  </tr>\n");
    fprintf(cgiOut, "  <tr>\n");
    fprintf(cgiOut,
            "    <td class=\"field\"><input type=\"text\" id=\"albtitle\" name=\"albtitle\" size=\"%d\" maxlength=\"%d\" value=\"%s\" accesskey=\"t\" /></td>\n",
            MAXLEN_ALBUMTITLE, MAXLEN_ALBUMTITLE,
            ((errors != NULL && errors[0] == E_NOERROR && formdata != NULL
              && formdata->title != NULL) ? formdata->title : ""));
    fprintf(cgiOut, "  </tr>\n");
    fprintf(cgiOut, "  <tr>\n");
    fprintf(cgiOut,
            "    <td class=\"describe%s\"><label for=\"artistid\" title=\"Artist Name\"><u>A</u>rtist Name: </label>&nbsp;<a class=\"small\" href=\"./?page=artist&amp;func=add&amp;hash=%d\">[Add Artist]</a></td>\n",
            ((isAdding == TRUE && errors != NULL
              && errors[1] != E_NOERROR) ? "2" : ""), _currUserLogon);
    fprintf(cgiOut, "  </tr>\n");
    fprintf(cgiOut, "  <tr>\n");
    fprintf(cgiOut, "    <td class=\"field\">\n");
    /* Print out the <select> for all artists */
    fprintf(cgiOut,
            "<select id=\"artistid\" name=\"artistid\" size=\"5\" accesskey=\"a\">\n");
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
            char *artistName = getArtistName(curr_id);

            fprintf(cgiOut, "  <option value=\"%d\"%s>%s</option>\n", curr_id,
                    ((formdata != NULL
                      && curr_id ==
                      formdata->artistID) ? " selected=\"selected\"" : ""),
                    artistName);
            count++;
            curr_id = allArtists[count];

            free(artistName);
        }
        free(allArtists);
    }

    fprintf(cgiOut, "</select>\n");
    fprintf(cgiOut, "    </td>\n");
    fprintf(cgiOut, "  </tr>\n");
    fprintf(cgiOut, "\n");
    fprintf(cgiOut, "  <tr>\n");
    fprintf(cgiOut,
            "    <td><input type=\"submit\" value=\"Add Album\" /></td>\n");
    fprintf(cgiOut, "  </tr>\n");
    fprintf(cgiOut, "</tbody>\n");
    fprintf(cgiOut, "</table>\n");
    fprintf(cgiOut, "</form>\n");
}

/*
 * Function: doViewAlbum
 * Parameters: (void)
 * Returns: (void)
 *
 * This function lets the user view the details of a specified album
 */
static void doViewAlbum(void)
{
    int result = 0;
    int albumid = -1;

    /* if albumid field is set */
    result = cgiFormInteger("albumid", &albumid, -1);
    if (result != cgiFormSuccess) {
        /* Some sort of failure */
        albumid = -1;
    }
    else {
        /* Check albumid exists */
        if (getAlbumExists(albumid) == FALSE) {
            fprintf(cgiOut, "Album [%d] does not exist in the database",
                    albumid);
            return;
        }
    }

    if (albumid == -1) {
        /* print all albums */
        printAllAlbums();
    }
    else {
        /* print specific album */
        printSpecificAlbum(albumid);
    }
}

/*
 * Function: printAllAlbums
 * Parameters: (void)
 * Returns: (void)
 *
 * This lists all the albums in the database in a table that the user can view.
 * The method goes through the list of albums and adds each to the table
 */
static void printAllAlbums(void)
{
    int *allAlbums = NULL;
    int curr_id = 0;
    int count = 0;

    fprintf(cgiOut, "<div class=\"head1\">Viewing All Albums</div>");

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
            fprintf(cgiOut, "<table border=\"1\">\n");
            fprintf(cgiOut, "\n");
            fprintf(cgiOut, "<thead>\n");
            fprintf(cgiOut, "  <tr>\n");
            fprintf(cgiOut, "    <td class=\"thead\">Album Title</td>\n");
            fprintf(cgiOut, "    <td class=\"thead\">Artist Name</td>\n");
            fprintf(cgiOut,
                    "    <td class=\"thead\" colspan=\"2\">Status</td>\n");
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

            curr_id = allAlbums[count];
            while (curr_id != LAST_ID_IN_ARRAY) {
                char *albumTitle = getAlbumTitle(curr_id);
                int albumArtist = getAlbumArtist(curr_id);
                char *artistName = getArtistName(albumArtist);
                int loanid = getAlbumCurrentLoan(curr_id);

                fprintf(cgiOut, "  <tr>\n");
                fprintf(cgiOut, "    <td>");
                fprintf(cgiOut,
                        "<a href=\"./?page=album&amp;albumid=%d&amp;hash=%d\">%s</a>",
                        curr_id, _currUserLogon, albumTitle);
                fprintf(cgiOut, "</td>\n");
                fprintf(cgiOut,
                        "    <td><a href=\"./?page=artist&amp;artistid=%d&amp;hash=%d\">%s</a></td>\n",
                        albumArtist, _currUserLogon, artistName);

                if (loanid != E_NOLOAN) {
                    /* Album is on loan */
                    fprintf(cgiOut, "    <td>On Loan</td>\n");

                    if (getLoanUser(loanid) == _currUserLogon) {
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
                                loanid);
                        fprintf(cgiOut,
                                "    <input type=\"hidden\" name=\"hash\" value=\"%d\" />\n",
                                _currUserLogon);
                        fprintf(cgiOut,
                                "    <input type=\"submit\" value=\"Return Album\" />\n");
                        fprintf(cgiOut, "  </div>\n");
                        fprintf(cgiOut, "</form>\n");
                        fprintf(cgiOut, "    </td>\n");
                    }
                    else {
                        fprintf(cgiOut, "    <td>Unavailable</td>\n");
                    }
                }
                else {
                    /* Album is in library */
                    fprintf(cgiOut, "    <td>In Library</td>\n");

                    fprintf(cgiOut, "    <td>\n");
                    fprintf(cgiOut,
                            "<form method=\"get\" action=\"./\" style=\"display: inline;\">\n");
                    fprintf(cgiOut, "  <div>\n");
                    fprintf(cgiOut,
                            "    <input type=\"hidden\" name=\"page\" value=\"loan\" />\n");
                    fprintf(cgiOut,
                            "    <input type=\"hidden\" name=\"func\" value=\"borrow\" />\n");
                    fprintf(cgiOut,
                            "    <input type=\"hidden\" name=\"albid\" value=\"%d\" />\n",
                            curr_id);
                    fprintf(cgiOut,
                            "    <input type=\"hidden\" name=\"hash\" value=\"%d\" />\n",
                            _currUserLogon);
                    fprintf(cgiOut,
                            "    <input type=\"submit\" value=\"Borrow Album\" />\n");
                    fprintf(cgiOut, "  </div>\n");
                    fprintf(cgiOut, "</form>\n");
                    fprintf(cgiOut, "    </td>\n");
                }

                fprintf(cgiOut, "  </tr>\n");

                count++;
                curr_id = allAlbums[count];

                free(albumTitle);
                free(artistName);
            }

            fprintf(cgiOut, "</tbody>\n");
            fprintf(cgiOut, "\n");
            fprintf(cgiOut, "</table>\n");
        }

        free(allAlbums);
    }
}

/*
 * Function: printSpecificAlbum
 * Parameters: int
 * Returns: (void)
 *
 * Users can specify the album they wish to view. This function allows them to do this. 
 * The albums details are then printed out for the user to view.
 */
static void printSpecificAlbum(int albumid)
{
    fprintf(cgiOut, "<div class=\"head1\">View Album [%d]</div>", albumid);

    {
        /* Print out information about the album */
        int artistid = getAlbumArtist(albumid);
        char *albumTitle = getAlbumTitle(albumid);
        char *artistName = getArtistName(artistid);

        fprintf(cgiOut, "<table>\n");
        fprintf(cgiOut, "<tbody>\n");
        fprintf(cgiOut, "  <tr>\n");
        fprintf(cgiOut, "    <td class=\"describe\">Album Title: </td>\n");
        fprintf(cgiOut, "  </tr>\n");
        fprintf(cgiOut, "  <tr>\n");
        fprintf(cgiOut, "    <td class=\"field\">%s</td>\n", albumTitle);
        fprintf(cgiOut, "  </tr>\n");
        fprintf(cgiOut, "  <tr>\n");
        fprintf(cgiOut, "    <td class=\"describe\">Artist Name: </td>\n");
        fprintf(cgiOut, "  </tr>\n");
        fprintf(cgiOut, "  <tr>\n");
        fprintf(cgiOut,
                "    <td class=\"field\">%s&nbsp;<a class=\"small\" href=\"./?page=artist&amp;artistid=%d&amp;hash=%d\">[View Artist]</a></td>\n",
                artistName, artistid, _currUserLogon);
        fprintf(cgiOut, "  </tr>\n");
        fprintf(cgiOut, "</tbody>\n");
        fprintf(cgiOut, "</table>\n");

        fprintf(cgiOut, "<hr />\n");

        free(albumTitle);
        free(artistName);
    }

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
                    char *userName = getUserName(tempUserID);

                    fprintf(cgiOut, "to <b>");
                    userLink("", tempUserID, userName, cgiOut);
                    fprintf(cgiOut, "</b><br />\n");
                    fprintf(cgiOut, "[Taken at ");
                    printTime(getLoanTimeIn(currLoan), cgiOut);
                    fprintf(cgiOut, "]\n");

                    free(userName);
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

    if (isUserLibrarian(_currUserLogon) == TRUE) {
        fprintf(cgiOut, "<hr />\n");
        fprintf(cgiOut,
                "<a href=\"./?page=loan&amp;func=view&amp;albumid=%d&amp;hash=%d\">View albums borrowing history</a>\n",
                albumid, _currUserLogon);
    }

    fprintf(cgiOut, "<hr />\n");

    fprintf(cgiOut,
            "<a href=\"./?page=albumcomment&amp;albumid=%d&amp;hash=%d\">View Comments written about Album (%d)</a>\n",
            albumid, _currUserLogon, getAlbumCommentsForAlbumCount(albumid));
    fprintf(cgiOut,
            "<br /><a href=\"./?page=albumcomment&amp;func=add&amp;albumid=%d&amp;hash=%d\">Add Comment about this Album</a>\n",
            albumid, _currUserLogon);
}
