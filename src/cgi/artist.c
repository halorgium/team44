#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "cgic.h"
#include "globals.h"
#include "../shared/defines.h"

static void doAddArtist(void);
static void doViewArtist(void);

static int processAddForm(void);
static void printAddForm(void);

static void printAllArtists(void);
static void printSpecificArtist(int);

void printArtist(void) {
    int result=0;
    char func[MAXSIZE_PAGENAME]={'\0'};

    /* if func field is set */
    result = cgiFormStringNoNewlines("func", func, MAXSIZE_PAGENAME);
    if(result != cgiFormSuccess || func == NULL) {
	/* Some sort of failure */
	/* Default to viewArtists */
	strncpy(func, "view", MAXSIZE_PAGENAME);
    }

    if(strncmp(func, "add", MAXSIZE_PAGENAME) == 0) {
	/* Do add album etc */
	doAddArtist();
    }
    else {
	/* Default */
	/* Do view album etc */
	doViewArtist();
    }

}  

static void doAddArtist(void) {
    int result=0;
    Boolean isAdding=-1;

    /* Check privileges of current user */
    if(isUserLibrarian(_currUserLogon) == FALSE) {
	fprintf(cgiOut, "You are not privleged to add new Artists\n");
	return;
    }
    
    fprintf(cgiOut, "<div class=\"head1\">Adding New Artist</div>\n");

    /* if adding field is set */
    result=cgiFormIntegerBounded("adding", &isAdding, FALSE, TRUE, FALSE);
    if(result != cgiFormSuccess) {
	/* Some sort of failure */
	isAdding=FALSE;
    }

    if(isAdding) {
	/* The curr data is ready for processing */
	int newartistid=processAddForm();
	if(newartistid > 0) {
	    /* Artist added ok */
	    fprintf(cgiOut, "Adding successful<br />\n");
	    fprintf(cgiOut, "<a href=\"./?page=artist&amp;artistid=%d&amp;hash=%d\">[View Artist]</a><br />\n", newartistid, _currUserLogon);
	    fprintf(cgiOut, "<a href=\"./?page=artist&amp;func=add&amp;hash=%d\">[Add Another Artist]</a>", _currUserLogon);
	}
	else {
	    /* Artist adding error */
	    /* Link back to add page */
	    fprintf(cgiOut, "<a href=\"./?page=artist&amp;func=add&amp;hash=%d\">[Add Another Artist]</a>", _currUserLogon);
	}
    }
    else {
	/* Need to print form */
	printAddForm();
    }

}

static int processAddForm(void) {
    int result=0;
    int newArtistid=-1;
    char *artname=malloc(sizeof(char)*MAXSIZE_ARTISTNAME);

    result = cgiFormStringNoNewlines("artname", artname, MAXSIZE_ARTISTNAME);
    if(result != cgiFormSuccess || artname == NULL) {
	newArtistid =  E_INVALID_PARAM;
    }
    else {
	newArtistid=addArtist(artname);
    }

    if(newArtistid < 0) {
	switch(newArtistid) {
	case DB_NEXTID_ERROR:
	    fprintf(cgiOut, "Database failure: ID allocation failed<br />\n");
	    break;
	case DB_SAVE_FAILURE:
	    fprintf(cgiOut, "Database failure: Artist save incomplete<br />\n");
	    break;
	case E_INVALID_PARAM:
	    fprintf(cgiOut, "Artist Name is invalid<br />\n");
	    break;
	case ALREADY_ADDED:
	    fprintf(cgiOut, "Artist called &quot;%s&quot; has already been added<br />\n", artname);
	    break;
	case E_MALLOC_FAILED:
	    fprintf(cgiOut, "Memory Allocation Error<br />\n");
	    break;
	default:
	    fprintf(cgiOut, "Unknown error: Adding failed<br />\n");
	    return E_UNKNOWN;
	}
    }
    
    return newArtistid;
} 

static void printAddForm(void) {
    fprintf(cgiOut, "<form method=\"get\" action=\"./\">\n");
    fprintf(cgiOut, "<table>\n");
    fprintf(cgiOut, "<tbody>\n");
    fprintf(cgiOut, "  <tr class=\"hidden\">\n");
    fprintf(cgiOut, "    <td>\n");
    fprintf(cgiOut, "    <input type=\"hidden\" name=\"page\" value=\"artist\" />\n");
    fprintf(cgiOut, "    <input type=\"hidden\" name=\"func\" value=\"add\" />\n");
    fprintf(cgiOut, "    <input type=\"hidden\" name=\"adding\" value=\"%d\" />\n", TRUE);
    fprintf(cgiOut, "    <input type=\"hidden\" name=\"hash\" value=\"%d\" />\n", _currUserLogon);
    fprintf(cgiOut, "    </td>\n");
    fprintf(cgiOut, "  </tr>\n");
    fprintf(cgiOut, "  <tr>\n");
    fprintf(cgiOut, "    <td class=\"describe\"><label for=\"artname\" title=\"Artist Name\">Artist Name: </label></td>\n");
    fprintf(cgiOut, "  </tr>\n");
    fprintf(cgiOut, "  <tr>\n");
    fprintf(cgiOut, "    <td class=\"field\"><input type=\"text\" id=\"artname\" name=\"artname\" size=\"%d\" /></td>\n", MAXSIZE_ARTISTNAME);
    fprintf(cgiOut, "  </tr>\n");
    fprintf(cgiOut, "\n");
    fprintf(cgiOut, "  <tr>\n");
    fprintf(cgiOut, "    <td><input type=\"submit\" value=\"Add Artist\" /></td>\n");
    fprintf(cgiOut, "  </tr>\n");
    fprintf(cgiOut, "</tbody>\n");
    fprintf(cgiOut, "</table>\n");
    fprintf(cgiOut, "</form>\n");
}

static void doViewArtist(void) {
    int result=0;
    int artistid=-1;

    /* if artistid field is set */
    result=cgiFormInteger("artistid", &artistid, -1);
    if(result != cgiFormSuccess) {
	/* Some sort of failure */
	artistid=-1;
    }
    else {
	/* Check artistid exists */
	if(getArtistExists(artistid) == FALSE) {
	    fprintf(cgiOut, "Artist [%d] does not exist in the database<br />\n", artistid);
	    fprintf(cgiOut, "<a href=\"./?page=artist&amp;hash=%d\">[View All Artists]</a><br />\n", _currUserLogon);
	    return;
	}
    }

    if(artistid == -1) {
	/* print all artists */
	printAllArtists();
    }
    else {
	/* print specific artist */
	printSpecificArtist(artistid);
    }
}

static void printAllArtists(void) {
    int *allArtists=NULL;
    int curr_id=0;
    int count=0;

    fprintf(cgiOut, "<div class=\"head1\">Viewing All Artists</div>");

    allArtists=getArtists();

    if(allArtists == NULL) {
	fprintf(cgiOut, "<div class=\"head1\">Error retrieving all Artists</div>");
    }
    else {
	if(getArtistsCount() == 0) {
	    fprintf(cgiOut, "No artists<br />\n");
            fprintf(cgiOut, "<a href=\"./?page=artist&amp;func=add&amp;hash=%d\">[Add new Artist]</a>\n", _currUserLogon);
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
	    fprintf(cgiOut, "    <td class=\"tfoot\" colspan=\"2\">&nbsp;</td>\n");
	    fprintf(cgiOut, "  </tr>\n");
	    fprintf(cgiOut, "</tfoot>\n");
	    fprintf(cgiOut, "\n");
	    fprintf(cgiOut, "<tbody>\n");
	    
	    curr_id=allArtists[count];
	    while (curr_id != LAST_ID_IN_ARRAY) {
	      char *artistName=getArtistName(curr_id);

		fprintf(cgiOut, "  <tr>\n");
		fprintf(cgiOut, "    <td>");
		fprintf(cgiOut, "<a href=\"./?page=artist&amp;artistid=%d&amp;hash=%d\">%s</a>", curr_id, _currUserLogon, artistName);
		fprintf(cgiOut, "    </td>\n");
		fprintf(cgiOut, "    <td>%d</td>\n", getArtistAlbumsCount(curr_id));
		fprintf(cgiOut, "  </tr>\n");
		
		count++;
		curr_id=allArtists[count];

		free(artistName);
	    }
	    
	    fprintf(cgiOut, "</tbody>\n");
	    fprintf(cgiOut, "\n");
	    fprintf(cgiOut, "</table>\n");
	}
	
        free(allArtists);
    }
}

static void printSpecificArtist(int artistid) {
    fprintf(cgiOut, "<div class=\"head1\">View Artist [%d]</div>", artistid);

    fprintf(cgiOut, "<table>\n");
    fprintf(cgiOut, "<tbody>\n");
    fprintf(cgiOut, "  <tr>\n");
    fprintf(cgiOut, "    <td class=\"describe\">Artist Name: </td>\n");
    fprintf(cgiOut, "  </tr>\n");
    fprintf(cgiOut, "  <tr>\n");
    fprintf(cgiOut, "    <td class=\"field\">%s</td>\n", getArtistName(artistid));
    fprintf(cgiOut, "  </tr>\n");
    fprintf(cgiOut, "  <tr>\n");
    fprintf(cgiOut, "    <td class=\"describe\">Album Count: </td>\n");
    fprintf(cgiOut, "  </tr>\n");
    fprintf(cgiOut, "  <tr>\n");
    fprintf(cgiOut, "    <td class=\"field\">%d</td>\n", getArtistAlbumsCount(artistid));
    fprintf(cgiOut, "  </tr>\n");
    fprintf(cgiOut, "</tbody>\n");
    fprintf(cgiOut, "</table>\n");
    fprintf(cgiOut, "<hr />\n");

    {
	/* list all albums by this artist */
	int *allAlbums=NULL;
	int curr_id=0;
	int count=0;

	fprintf(cgiOut, "<div class=\"head1\">Albums written</div>\n");

	allAlbums=getArtistAlbums(artistid);

	if(allAlbums == NULL) {
	    fprintf(cgiOut, "<div class=\"head1\">Error retrieving all Albums</div>");
	}
	else {
	    if(getArtistAlbumsCount(artistid) == 0) {
		fprintf(cgiOut, "No albums\n");
	    }
	    else {
		fprintf(cgiOut, "<table border=\"1\">\n");
		fprintf(cgiOut, "\n");
		fprintf(cgiOut, "<thead>\n");
		fprintf(cgiOut, "  <tr>\n");
		fprintf(cgiOut, "    <td class=\"thead\">Album Title</td>\n");
		fprintf(cgiOut, "    <td class=\"thead\">Status</td>\n");
		fprintf(cgiOut, "  </tr>\n");
		fprintf(cgiOut, "</thead>\n");
		fprintf(cgiOut, "\n");
		fprintf(cgiOut, "<tfoot>\n");
		fprintf(cgiOut, "  <tr>\n");
		fprintf(cgiOut, "    <td class=\"tfoot\" colspan=\"2\">&nbsp;</td>\n");
		fprintf(cgiOut, "  </tr>\n");
		fprintf(cgiOut, "</tfoot>\n");
		fprintf(cgiOut, "\n");
		fprintf(cgiOut, "<tbody>\n");

		curr_id=allAlbums[count];
		while (curr_id != LAST_ID_IN_ARRAY) {
		    fprintf(cgiOut, "  <tr>\n");
		    fprintf(cgiOut, "    <td>");
		    fprintf(cgiOut, "<a href=\"./?page=album&amp;albumid=%d&amp;hash=%d\">%s</a>", curr_id, _currUserLogon, getAlbumTitle(curr_id));
		    fprintf(cgiOut, "    </td>\n");
		    if(getAlbumCurrentLoan(curr_id) != E_NOLOAN) {
			fprintf(cgiOut, "    <td>On Loan</td>\n");
		    }
		    else {
			fprintf(cgiOut, "    <td>In Library</td>\n");
		    }
		    fprintf(cgiOut, "  </tr>\n");
		    
		    count++;
		    curr_id=allAlbums[count];
		}
		
		fprintf(cgiOut, "</tbody>\n");
		fprintf(cgiOut, "\n");
		fprintf(cgiOut, "</table>\n");
	    }
	    free(allAlbums);
	}
    }

    fprintf(cgiOut, "<br /><a href=\"./?page=album&amp;func=add&amp;artistid=%d&amp;hash=%d\">Add Album by this Artist</a>\n", artistid, _currUserLogon);
    
    fprintf(cgiOut, "<hr />\n");
    
    fprintf(cgiOut, "<a href=\"./?page=artistcomment&amp;artistid=%d&amp;hash=%d\">View Comments written about Artist (%d)</a>\n", artistid, _currUserLogon, getArtistCommentsForArtistCount(artistid));
    fprintf(cgiOut, "<br /><a href=\"./?page=artistcomment&amp;func=add&amp;artistid=%d&amp;hash=%d\">Add Comment about this Artist</a>\n", artistid, _currUserLogon);

}
