#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "cgic.h"
#include "globals.h"
#include "../shared/defines.h"

static void doAddAlbum(void);
static void doViewAlbum(void);

static int processAddForm(void);
static void printAddForm(void);

static void printAllAlbums(void);
static void printSpecificAlbum(int);

void printAlbum(void) {
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
	/* Do add album etc */
	doAddAlbum();
    }
    else {
	/* Default */
	/* Do view album etc */
	doViewAlbum();
    }
}  

static void doAddAlbum(void) {
    int result=0;
    Boolean isAdding=-1;

    /* Check privileges of current user */
    if(isUserLibrarian(_currUserLogon) == FALSE) {
	fprintf(cgiOut, "You are not privleged to add new Albums\n");
	return;
    }
    
    fprintf(cgiOut, "<div class=\"head1\">Adding New Album</div>\n");

    /* if adding field is set */
    result=cgiFormIntegerBounded("adding", &isAdding, FALSE, TRUE, FALSE);
    if(result != cgiFormSuccess) {
	/* Some sort of failure */
	isAdding=FALSE;
    }

    if(isAdding) {
	/* The curr data is ready for processing */
	int newalbumid=processAddForm();
	if(newalbumid > 0) {
	    /* Album added ok */
	    fprintf(cgiOut, "Adding successful<br />\n");
	    fprintf(cgiOut, "<a href=\"./?page=album&amp;albumid=%d&amp;hash=%d\">[View Album]</a><br />\n", newalbumid, _currUserLogon);
	    fprintf(cgiOut, "<a href=\"./?page=album&amp;func=add&amp;hash=%d\">[Add another Album]</a>", _currUserLogon);
	}
	else {
	    /* Album adding error */
	    /* Link back to add page */
	    fprintf(cgiOut, "<a href=\"./?page=album&amp;func=add&amp;hash=%d\">[Add Another Album]</a>", _currUserLogon);
	}
    }
    else {
	/* Need to print form */
	printAddForm();
    }

}

static int processAddForm(void) {
    int result=0;
    int newAlbumid=-1;
    char *albtitle=malloc(sizeof(char)*MAXSIZE_ALBUMTITLE);
    int artistid=-1;

    result = cgiFormStringNoNewlines("albtitle", albtitle, MAXSIZE_ARTISTNAME);
    if(result != cgiFormSuccess || albtitle == NULL) {
	newAlbumid = E_INVALID_PARAM;
    }
    else {
	result = cgiFormInteger("artistid", &artistid, -1);
	if(result != cgiFormSuccess || artistid == -1) {
	    newAlbumid = E_FORM;
	}
	else {
	    newAlbumid=addAlbum(albtitle, artistid);
	}
    }

    if(newAlbumid < 0) {
	switch(newAlbumid) {
	case DB_NEXTID_ERROR:
	    fprintf(cgiOut, "Database failure: ID allocation failed<br />\n");
	    break;
	case DB_SAVE_FAILURE:
	    fprintf(cgiOut, "Database failure: Album save incomplete<br />\n");
	    break;
	case E_NOARTIST:
	    fprintf(cgiOut, "Artist [%d] does not exist<br />\n", artistid);
	    break;
	case E_FORM:
	    fprintf(cgiOut, "Artist not selected<br />\n");
	    break;
	case E_INVALID_PARAM:
	    fprintf(cgiOut, "Album Name is invalid<br />\n");
	    break;
	case ALREADY_ADDED:
	    fprintf(cgiOut, "Album called &quot;%s&quot; written by &quot;%s&quot; has already been added<br />\n", albtitle, getArtistName(artistid));
	    break;
	case E_MALLOC_FAILED:
	    fprintf(cgiOut, "Memory Allocation Error<br />\n");
	    break;
	default:
	    fprintf(cgiOut, "Unknown error: Adding failed<br />\n");
	    return E_UNKNOWN;
	}
    }
    
    return newAlbumid;
} 
	
static void printAddForm(void) {
    int result=-1;
    
    /* Check for artistid */
    int artistid=-1;
    result = cgiFormInteger("artistid", &artistid, -1);
    if(result != cgiFormSuccess || artistid == -1) {
	/* No artistid */
	artistid=-1;
    }
    
    if(artistid != -1 && getArtistExists(artistid) == FALSE) {
	/* No artistid */
	fprintf(cgiOut, "Artist [%d] does not exist in the database\n", artistid);
	artistid=-1;
    }

    if(getArtistsCount() == 0) {
	fprintf(cgiOut, "To be able to add an album, the database must contain an artist<br />\n");
	fprintf(cgiOut, "<a href=\"./?page=artist&amp;func=add&amp;hash=%d\">[Add Artist]</a>\n", _currUserLogon);
	return;
    }

    fprintf(cgiOut, "<form method=\"get\" action=\"./\">\n");
    fprintf(cgiOut, "<table>\n");
    fprintf(cgiOut, "<tbody>\n");
    fprintf(cgiOut, "  <tr class=\"hidden\">\n");
    fprintf(cgiOut, "    <td>\n");
    fprintf(cgiOut, "    <input type=\"hidden\" name=\"page\" value=\"album\" />\n");
    fprintf(cgiOut, "    <input type=\"hidden\" name=\"func\" value=\"add\" />\n");
    if(artistid != -1) {
	fprintf(cgiOut, "    <input type=\"hidden\" name=\"artistid\" value=\"%d\" />\n", artistid);
    }
    fprintf(cgiOut, "    <input type=\"hidden\" name=\"adding\" value=\"%d\" />\n", TRUE);
    fprintf(cgiOut, "    <input type=\"hidden\" name=\"hash\" value=\"%d\" />\n", _currUserLogon);
    fprintf(cgiOut, "    </td>\n");
    fprintf(cgiOut, "  </tr>\n");
    fprintf(cgiOut, "  <tr>\n");
    fprintf(cgiOut, "    <td class=\"describe\"><label for=\"albtitle\" title=\"Album Title\">Album Title: </label></td>\n");
    fprintf(cgiOut, "  </tr>\n");
    fprintf(cgiOut, "  <tr>\n");
    fprintf(cgiOut, "    <td class=\"field\"><input type=\"text\" id=\"albtitle\" name=\"albtitle\" size=\"%d\"/></td>\n", MAXSIZE_ALBUMTITLE);
    fprintf(cgiOut, "  </tr>\n");
    fprintf(cgiOut, "  <tr>\n");
    fprintf(cgiOut, "    <td class=\"describe\"><label for=\"artistid\" title=\"Artist Name\">Artist Name: </label>&nbsp;<a class=\"small\" href=\"./?page=artist&amp;func=add&amp;hash=%d\">[Add Artist]</a></td>\n", _currUserLogon);
    fprintf(cgiOut, "  </tr>\n");
    fprintf(cgiOut, "  <tr>\n");
    if(artistid == -1) {
	int *allArtists=getArtists();
	int count=0;
	int curr_id=0;
	
	fprintf(cgiOut, "    <td class=\"field\">\n");
	/* Print out the <select> for all artists */
	fprintf(cgiOut, "<select id=\"artistid\" name=\"artistid\" size=\"5\">\n");
	
	curr_id=allArtists[count];
	while (curr_id != LAST_ID_IN_ARRAY) {
	    fprintf(cgiOut, "  <option value=\"%d\">%s</option>\n", curr_id, getArtistName(curr_id));
	    count++;
	    curr_id=allArtists[count];
	}
	
	fprintf(cgiOut, "</select>\n");
	fprintf(cgiOut, "</td>\n");
    }
    else {
	fprintf(cgiOut, "    <td class=\"field\">%s</td>\n", getArtistName(artistid));
    }
    fprintf(cgiOut, "  </tr>\n");
    fprintf(cgiOut, "\n");
    fprintf(cgiOut, "  <tr>\n");
    fprintf(cgiOut, "    <td><input type=\"submit\" value=\"Add Album\" /></td>\n");
    fprintf(cgiOut, "  </tr>\n");
    fprintf(cgiOut, "</tbody>\n");
    fprintf(cgiOut, "</table>\n");
    fprintf(cgiOut, "</form>\n");
}

static void doViewAlbum(void) {
    int result=0;
    int albumid=-1;

    /* if albumid field is set */
    result=cgiFormInteger("albumid", &albumid, -1);
    if(result != cgiFormSuccess) {
	/* Some sort of failure */
	albumid=-1;
    }
    else {
	/* Check albumid exists */
	if(getAlbumExists(albumid) == FALSE) {
	    fprintf(cgiOut, "Album [%d] does not exist in the database", albumid);
	    return;
	}
    }

    if(albumid == -1) {
	/* print all albums */
	printAllAlbums();
    }
    else {
	/* print specific album */
	printSpecificAlbum(albumid);
    }
}

static void printAllAlbums(void) {
    int *allAlbums=NULL;
    int curr_id=0;
    int count=0;

    fprintf(cgiOut, "<div class=\"head1\">Viewing All Albums</div>");

    allAlbums=getAlbums();

    if(allAlbums == NULL) {
	fprintf(cgiOut, "<div class=\"head1\">Error retrieving all Albums</div>");
    }
    else {
	if(getAlbumsCount() == 0) {
	    fprintf(cgiOut, "No albums<br />\n");
	    fprintf(cgiOut, "<a href=\"./?page=album&amp;func=add&amp;hash=%d\">[Add new Album]</a>\n", _currUserLogon);
	}
	else {
	    fprintf(cgiOut, "<table border=\"1\">\n");
	    fprintf(cgiOut, "\n");
	    fprintf(cgiOut, "<thead>\n");
	    fprintf(cgiOut, "  <tr>\n");
	    fprintf(cgiOut, "    <td class=\"thead\">Album Title</td>\n");
	    fprintf(cgiOut, "    <td class=\"thead\">Artist Name</td>\n");
	    fprintf(cgiOut, "    <td class=\"thead\">Status</td>\n");
	    fprintf(cgiOut, "  </tr>\n");
	    fprintf(cgiOut, "</thead>\n");
	    fprintf(cgiOut, "\n");
	    fprintf(cgiOut, "<tfoot>\n");
	    fprintf(cgiOut, "  <tr>\n");
	    fprintf(cgiOut, "    <td class=\"tfoot\" colspan=\"3\">&nbsp;</td>\n");
	    fprintf(cgiOut, "  </tr>\n");
	    fprintf(cgiOut, "</tfoot>\n");
	    fprintf(cgiOut, "\n");
	    fprintf(cgiOut, "<tbody>\n");
	    
	    curr_id=allAlbums[count];
	    while (curr_id != LAST_ID_IN_ARRAY) {
	      char *albumTitle=getAlbumTitle(curr_id);
	      int albumArtist=getAlbumArtist(curr_id);
	      char *artistName=getArtistName(albumArtist);
	      
		fprintf(cgiOut, "  <tr>\n");
		fprintf(cgiOut, "    <td>");
		fprintf(cgiOut, "<a href=\"./?page=album&amp;albumid=%d&amp;hash=%d\">%s</a>", curr_id, _currUserLogon, albumTitle);
		fprintf(cgiOut, "    </td>\n");
		fprintf(cgiOut, "    <td><a href=\"./?page=artist&amp;artistid=%d&amp;hash=%d\">%s</a></td>\n", albumArtist, _currUserLogon, artistName);
		
		if(getAlbumCurrentLoan(curr_id) != E_NOLOAN) {
		    fprintf(cgiOut, "    <td>On Loan</td>\n");
		}
		else {
		    fprintf(cgiOut, "    <td>In Library</td>\n");
		}
		fprintf(cgiOut, "  </tr>\n");
		
		count++;
		curr_id=allAlbums[count];

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

static void printSpecificAlbum(int albumid) {
    int artistid=getAlbumArtist(albumid);

    fprintf(cgiOut, "<div class=\"head1\">View Album [%d]</div>", albumid);

    fprintf(cgiOut, "<table>\n");
    fprintf(cgiOut, "<tbody>\n");
    fprintf(cgiOut, "  <tr>\n");
    fprintf(cgiOut, "    <td class=\"describe\">Album Title: </td>\n");
    fprintf(cgiOut, "  </tr>\n");
    fprintf(cgiOut, "  <tr>\n");
    fprintf(cgiOut, "    <td class=\"field\">%s</td>\n", getAlbumTitle(albumid));
    fprintf(cgiOut, "  </tr>\n");
    fprintf(cgiOut, "  <tr>\n");
    fprintf(cgiOut, "    <td class=\"describe\">Artist Name: </td>\n");
    fprintf(cgiOut, "  </tr>\n");
    fprintf(cgiOut, "  <tr>\n");
    fprintf(cgiOut, "    <td class=\"field\">%s&nbsp;<a class=\"small\" href=\"./?page=artist&amp;artistid=%d&amp;hash=%d\">[View Artist]</a></td>\n", getArtistName(artistid), artistid, _currUserLogon);
    fprintf(cgiOut, "  </tr>\n");
    fprintf(cgiOut, "</tbody>\n");
    fprintf(cgiOut, "</table>\n");

    fprintf(cgiOut, "<hr />\n");

    {
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

    if(isUserLibrarian(_currUserLogon) == TRUE) {
	fprintf(cgiOut, "<hr />\n");
	fprintf(cgiOut, "<a href=\"./?page=loan&amp;func=view&amp;albumid=%d&amp;hash=%d\">View albums borrowing history</a>\n", albumid, _currUserLogon);
    }

    fprintf(cgiOut, "<hr />\n");

    fprintf(cgiOut, "<a href=\"./?page=albumcomment&amp;albumid=%d&amp;hash=%d\">View Comments written about Album (%d)</a>\n", albumid, _currUserLogon, getAlbumCommentsForAlbumCount(albumid));
    fprintf(cgiOut, "<br /><a href=\"./?page=albumcomment&amp;func=add&amp;albumid=%d&amp;hash=%d\">Add Comment about this Album</a>\n", albumid, _currUserLogon);
}
