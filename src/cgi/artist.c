/*
 *
 * artist.c
 * ==========
 * provides functions which deal with artists in the cgi
 * 
*/

/*================= Preprocessor statements===============================*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "cgic.h"
#include "globals.h"
#include "../shared/defines.h"
#include "../database/structs.h"

/*======================Function Declarations=============================*/

static void doAddArtist(void);
static void doViewArtist(void);

static Boolean processAddForm(int *, artistNode_t *);
static void printAddForm(Boolean, int *, artistNode_t *);

static void printAllArtists(void);
static void printSpecificArtist(int);

/*======================Function Definitions=============================*/

/*
 * Function: printArtist
 * Parameters: funcName stuct
 * Returns: (void)
 *
 * checks to see if the function name is for adding an artist.
 * if not it calls the doViewAlbum method
 */
void printArtist(funcName_t func)
{
    switch (func) {
    case FUNC_ADD:
        /* Do add artist etc */
        doAddArtist();
        break;
    default:
        /* Default */
        /* Do view artist etc */
        doViewArtist();
    }
}

/*
 * Function: printArtistComment
 * Parameters: funcName stuct
 * Returns: (void)
 * 
 * This function checks if the current user is a librarian or not 
 * so it knows whether the user has permission to add a new artist. 
 * If they don't have permission they will recieve a message stating 
 * they are not privleged to add new artists. The function will then 
 * check that the cgi form was a success and then will add the new artist 
 * by communicating with the database.
 */
static void doAddArtist(void)
{
    Boolean needAddForm = TRUE;

    int result = 0;

    /* Temporary Struct to store form data */
    artistNode_t *formdata = NULL;
    /* Array to store errors */
    int *errors = NULL;
    Boolean isAdding = FALSE;

    /* Check privileges of current user */
    if (isUserLibrarian(_currUserLogon) == FALSE) {
        fprintf(cgiOut, "You are not privileged to add new Artists\n");
        return;
    }

    fprintf(cgiOut, "<div class=\"head1\">Adding New Artist</div>\n");

    /* if adding field is set */
    result = cgiFormIntegerBounded("adding", &isAdding, FALSE, TRUE, FALSE);
    if (result != cgiFormSuccess) {
        /* Some sort of failure */
        isAdding = FALSE;
    }

    /* Malloc space for form data */
    formdata = malloc(sizeof(artistNode_t));
    if (formdata == NULL) {
        isAdding = FALSE;
    }
    else {
        Boolean formOK = FALSE;

        formdata->name = NULL;

        /* Malloc space for error code of each field */
        errors = malloc(sizeof(int) * 1);
        if (errors == NULL) {
            free(formdata);
            isAdding = FALSE;
        }
        else {
            /* Set errors to E_NOERROR */
            int i = 0;
            for (i = 0; i < 1; i++) {
                errors[i] = E_NOERROR;
            }
        }

        /* The curr data is ready for processing */
        formOK = processAddForm(errors, formdata);

        if (isAdding == TRUE && formOK == TRUE) {
            int newArtistid = -1;

            /* All form data is good */
            /* Add artist to database */
            newArtistid = addArtist(formdata->name);

            if (newArtistid > 0) {
                /* User added ok */
                needAddForm = FALSE;
                fprintf(cgiOut, "Adding successful<br />\n");
                fprintf(cgiOut,
                        "<a href=\"./?page=artist&amp;artistid=%d&amp;hash=%d\">[View Artist]</a><br />\n",
                        newArtistid, _currUserLogon);
                fprintf(cgiOut,
                        "<a href=\"./?page=album&amp;func=add&amp;artistid=%d&amp;hash=%d\">[Add Album by this Artist]</a><br />\n",
                        newArtistid, _currUserLogon);
                fprintf(cgiOut,
                        "<a href=\"./?page=artist&amp;func=add&amp;hash=%d\">[Add Another Artist]</a>",
                        _currUserLogon);
            }
            else {
                /* Artist adding error */
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
        if (formdata->name != NULL) {
            free(formdata->name);
        }
        free(formdata);
    }
}

/*
 * Function: processAddForm
 * Parameters: (void)
 * Returns: int
 * 
 * This function is used to process the data that was entered in the adding 
 * form. The function allocates memory and if it fails memory allocation
 * error will be printed to the user. It also checks there were no errors 
 * in new artists id and if there are errors the appropriate message will 
 * be shown to the user
 */
static Boolean processAddForm(int *errors, artistNode_t * formdata)
{
    int result = 0;
    int size = -1;

    /* Check arguments */
    if (errors == NULL || formdata == NULL) {
        return FALSE;
    }

    /* Get artist name */
    result = cgiFormStringSpaceNeeded("artname", &size);
    if (result != cgiFormSuccess) {
        errors[0] = E_FORM;
    }
    else if (size > MAXLEN_ARTISTNAME + 1) {
        errors[0] = E_TOOBIG;
    }
    else {
        formdata->name = malloc(sizeof(char) * size);
        if (formdata->name == NULL) {
            errors[0] = E_MALLOC_FAILED;
        }
        else {
            result = cgiFormStringNoNewlines("artname", formdata->name, size);
            if (result != cgiFormSuccess) {
                errors[0] = E_FORM;
            }
            else if (checkString2(formdata->name) == FALSE) {
                errors[0] = E_INVALID_PARAM;
            }
            else {
                /* check whether artist already exists */
                {
                    /*pointer to list of artists */
                    int *allArtists = getArtists();
                    int i;      /*counter */

                    for (i = 0; allArtists[i] != LAST_ID_IN_ARRAY; i++) {
                        char *artistName = getArtistName(allArtists[i]);
                        if (strcmp(artistName, formdata->name) == 0) {
                            /*artist added is 'the same' as another in database */
                            errors[0] = ALREADY_ADDED;
                        }
                        free(artistName);
                    }
                    free(allArtists);
                }
            }
        }
    }

    if (errors[0] != E_NOERROR) {
        return FALSE;
    }
    return TRUE;
}

/*
 * Function: printAddForm
 * Parameters: (void)
 * Returns: (void)
 *
 * This function checks for errors like its too big, is invalid or already exist
 * and then once this has been processed the form(html code) can be written to the cgi output stream 
 */
static void printAddForm(Boolean isAdding, int *errors,
                         artistNode_t * formdata)
{
    Boolean freshForm = FALSE;

    /* Check arguments */
    if (isAdding == FALSE || errors == NULL || formdata == NULL) {
        /* Print a fresh form */
        freshForm = TRUE;
    }
    else {
        /* Process errors */

        /* Artist Name */
        switch (errors[0]) {
        case E_NOERROR:
            break;
        case E_FORM:
            fprintf(cgiOut, "Artist Name is empty<br />\n");
            break;
        case E_TOOBIG:
            fprintf(cgiOut, "Artist Name is too big<br />\n");
            break;
        case E_INVALID_PARAM:
            fprintf(cgiOut, "Artist Name is invalid<br />\n");
            break;
        case ALREADY_ADDED:
            fprintf(cgiOut,
                    "Artist Name &quot;%s&quot; already exists<br />\n",
                    formdata->name);
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
    fprintf(cgiOut, "  <tr class=\"hidden\">\n");
    fprintf(cgiOut, "    <td>\n");
    fprintf(cgiOut,
            "    <input type=\"hidden\" name=\"page\" value=\"artist\" />\n");
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
            "    <td class=\"describe%s\"><label for=\"artname\" title=\"Artist Name\"><u>A</u>rtist Name: </label></td>\n",
            ((isAdding == TRUE && errors != NULL
              && errors[0] != E_NOERROR) ? "2" : ""));
    fprintf(cgiOut, "  </tr>\n");
    fprintf(cgiOut, "  <tr>\n");
    fprintf(cgiOut,
            "    <td class=\"field\"><input type=\"text\" id=\"artname\" name=\"artname\" size=\"%d\" maxlength=\"%d\" value=\"%s\" accesskey=\"a\" /></td>\n",
            MAXLEN_ARTISTNAME, MAXLEN_ARTISTNAME,
            ((errors != NULL && errors[0] == E_NOERROR && formdata != NULL
              && formdata->name != NULL) ? formdata->name : ""));
    fprintf(cgiOut, "  </tr>\n");
    fprintf(cgiOut, "\n");
    fprintf(cgiOut, "  <tr>\n");
    fprintf(cgiOut,
            "    <td><input type=\"submit\" value=\"Add Artist\" /></td>\n");
    fprintf(cgiOut, "  </tr>\n");
    fprintf(cgiOut, "</tbody>\n");
    fprintf(cgiOut, "</table>\n");
    fprintf(cgiOut, "</form>\n");
}

/*
 * Function: doViewArtist
 * Parameters: (void)
 * Returns: (void)
 *
 * This function lets the user view the details of a specified Artist
 */
static void doViewArtist(void)
{
    int result = 0;
    int artistid = -1;

    /* if artistid field is set */
    result = cgiFormInteger("artistid", &artistid, -1);
    if (result != cgiFormSuccess) {
        /* Some sort of failure */
        artistid = -1;
    }
    else {
        /* Check artistid exists */
        if (getArtistExists(artistid) == FALSE) {
            fprintf(cgiOut,
                    "Artist [%d] does not exist in the database<br />\n",
                    artistid);
            fprintf(cgiOut,
                    "<a href=\"./?page=artist&amp;hash=%d\">[View All Artists]</a><br />\n",
                    _currUserLogon);
            return;
        }
    }

    if (artistid == -1) {
        /* print all artists */
        printAllArtists();
    }
    else {
        /* print specific artist */
        printSpecificArtist(artistid);
    }
}

/*
 * Function: printAllArtists
 * Parameters: (void)
 * Returns: (void)
 *
 * displays all the artists in a table The function goes through the list of albums 
 * and adds each to the table
 */
static void printAllArtists(void)
{
    int *allArtists = NULL;
    int curr_id = 0;
    int count = 0;

    fprintf(cgiOut, "<div class=\"head1\">Viewing All Artists</div>");

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
            fprintf(cgiOut, "<table border=\"1\">\n");
            fprintf(cgiOut, "\n");
            fprintf(cgiOut, "<thead>\n");
            fprintf(cgiOut, "  <tr>\n");
            fprintf(cgiOut, "    <td class=\"thead\">Artist Name</td>\n");
            fprintf(cgiOut, "    <td class=\"thead\">Album Count</td>\n");
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
                char *artistName = getArtistName(curr_id);

                fprintf(cgiOut, "  <tr>\n");
                fprintf(cgiOut, "    <td>");
                fprintf(cgiOut,
                        "<a href=\"./?page=artist&amp;artistid=%d&amp;hash=%d\">%s</a>",
                        curr_id, _currUserLogon, artistName);
                fprintf(cgiOut, "    </td>\n");
                fprintf(cgiOut, "    <td>%d</td>\n",
                        getArtistAlbumsCount(curr_id));
                fprintf(cgiOut, "  </tr>\n");

                count++;
                curr_id = allArtists[count];

                free(artistName);
            }

            fprintf(cgiOut, "</tbody>\n");
            fprintf(cgiOut, "\n");
            fprintf(cgiOut, "</table>\n");
        }

        free(allArtists);
    }
}

/*
 * Function: printSpecificArtist
 * Parameters: int
 * Returns: (void)
 *
 * Users can specify the artist they wish to view. This function allows them to do this. 
 * The artist details are then printed out for the user to view.
 */
static void printSpecificArtist(int artistid)
{
    fprintf(cgiOut, "<div class=\"head1\">View Artist [%d]</div>", artistid);

    {
        /* print out artist information */
        char *artistName = getArtistName(artistid);

        fprintf(cgiOut, "<table>\n");
        fprintf(cgiOut, "<tbody>\n");
        fprintf(cgiOut, "  <tr>\n");
        fprintf(cgiOut, "    <td class=\"describe\">Artist Name: </td>\n");
        fprintf(cgiOut, "  </tr>\n");
        fprintf(cgiOut, "  <tr>\n");
        fprintf(cgiOut, "    <td class=\"field\">%s</td>\n", artistName);
        fprintf(cgiOut, "  </tr>\n");
        fprintf(cgiOut, "  <tr>\n");
        fprintf(cgiOut, "    <td class=\"describe\">Album Count: </td>\n");
        fprintf(cgiOut, "  </tr>\n");
        fprintf(cgiOut, "  <tr>\n");
        fprintf(cgiOut, "    <td class=\"field\">%d</td>\n",
                getArtistAlbumsCount(artistid));
        fprintf(cgiOut, "  </tr>\n");
        fprintf(cgiOut, "</tbody>\n");
        fprintf(cgiOut, "</table>\n");
        fprintf(cgiOut, "<hr />\n");

        free(artistName);
    }

    {
        /* list all albums by this artist */
        int *allAlbums = NULL;
        int curr_id = 0;
        int count = 0;

        fprintf(cgiOut, "<div class=\"head1\">Albums written</div>\n");

        allAlbums = getArtistAlbums(artistid);

        if (allAlbums == NULL) {
            fprintf(cgiOut,
                    "<div class=\"head1\">Error retrieving all Albums</div>");
        }
        else {
            if (getArtistAlbumsCount(artistid) == 0) {
                fprintf(cgiOut, "No albums\n");
            }
            else {
                fprintf(cgiOut, "<table border=\"1\">\n");
                fprintf(cgiOut, "\n");
                fprintf(cgiOut, "<thead>\n");
                fprintf(cgiOut, "  <tr>\n");
                fprintf(cgiOut, "    <td class=\"thead\">Album Title</td>\n");
                fprintf(cgiOut,
                        "    <td class=\"thead\" colspan=\"2\">Status</td>\n");
                fprintf(cgiOut, "  </tr>\n");
                fprintf(cgiOut, "</thead>\n");
                fprintf(cgiOut, "\n");
                fprintf(cgiOut, "<tfoot>\n");
                fprintf(cgiOut, "  <tr>\n");
                fprintf(cgiOut,
                        "    <td class=\"tfoot\" colspan=\"3\">&nbsp;</td>\n");
                fprintf(cgiOut, "  </tr>\n");
                fprintf(cgiOut, "</tfoot>\n");
                fprintf(cgiOut, "\n");
                fprintf(cgiOut, "<tbody>\n");

                curr_id = allAlbums[count];
                while (curr_id != LAST_ID_IN_ARRAY) {
                    char *albumTitle = getAlbumTitle(curr_id);
                    int loanid = getAlbumCurrentLoan(curr_id);

                    fprintf(cgiOut, "  <tr>\n");
                    fprintf(cgiOut, "    <td>");
                    fprintf(cgiOut,
                            "<a href=\"./?page=album&amp;albumid=%d&amp;hash=%d\">%s</a>",
                            curr_id, _currUserLogon, albumTitle);
                    fprintf(cgiOut, "    </td>\n");
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
                }

                fprintf(cgiOut, "</tbody>\n");
                fprintf(cgiOut, "\n");
                fprintf(cgiOut, "</table>\n");
            }
            free(allAlbums);
        }
    }
    if (isUserLibrarian(_currUserLogon) == TRUE) {
        fprintf(cgiOut,
                "<br /><a href=\"./?page=album&amp;func=add&amp;artistid=%d&amp;hash=%d\">Add Album by this Artist</a>\n",
                artistid, _currUserLogon);
    }

    fprintf(cgiOut, "<hr />\n");

    fprintf(cgiOut,
            "<a href=\"./?page=artistcomment&amp;artistid=%d&amp;hash=%d\">View Comments written about Artist (%d)</a>\n",
            artistid, _currUserLogon,
            getArtistCommentsForArtistCount(artistid));
    fprintf(cgiOut,
            "<br /><a href=\"./?page=artistcomment&amp;func=add&amp;artistid=%d&amp;hash=%d\">Add Comment about this Artist</a>\n",
            artistid, _currUserLogon);
}
