#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "cgic.h"
#include "globals.h"
#include "../shared/structs.h"
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
    if(newalbumid != -1) {
      /* Album added ok */
      fprintf(cgiOut, "Adding successful\n");
      fprintf(cgiOut, "<a href=\"./?page=album&amp;albumid=%d&amp;user=%s\">[View Album]</a>", newalbumid, userCode);
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

  fprintf(cgiOut, "<form method=\"post\" action=\"./\">\n");
  fprintf(cgiOut, "<table>\n");
  fprintf(cgiOut, "<tbody>\n");
  fprintf(cgiOut, "  <tr class=\"hidden\">\n");
  fprintf(cgiOut, "    <td>\n");
  fprintf(cgiOut, "    <input type=\"hidden\" name=\"page\" value=\"album\" />\n");
  fprintf(cgiOut, "    <input type=\"hidden\" name=\"func\" value=\"add\" />\n");
  fprintf(cgiOut, "    <input type=\"hidden\" name=\"adding\" value=\"%d\" />\n", TRUE);
  fprintf(cgiOut, "    <input type=\"hidden\" name=\"user\" value=\"%s\" />\n", userCode);
  fprintf(cgiOut, "    </td>\n");
  fprintf(cgiOut, "  </tr>\n");
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
    Boolean albumexists=TRUE;

    /****************************/
    if(albumid == 23) {
      albumexists=FALSE;
    }
    /****************************/

    if(!albumexists) {
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

void printAllAlbums(void) {
    int *allAlbums=NULL;
    int curr_id=0;
    int count=0;

    fprintf(cgiOut, "<div class=\"head1\">Viewing All Albums</div>");

    allAlbums=getAllAlbums();

    if(allAlbums == NULL) {
      fprintf(cgiOut, "<div class=\"head1\">Error retrieving all Albums</div>");
    }
    else {
      fprintf(cgiOut, "<table border=\"1\">\n");
      fprintf(cgiOut, "\n");
      fprintf(cgiOut, "<thead>\n");
      fprintf(cgiOut, "  <tr>\n");
      fprintf(cgiOut, "    <td class=\"thead\">Album Title</td>\n");
      fprintf(cgiOut, "    <td class=\"thead\">Artist Name</td>\n");
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
	  fprintf(cgiOut, "<a href=\"./?page=album&amp;albumid=%d&amp;user=%s\">%s</a>", curr_id, userCode, getAlbumTitle(curr_id));
	  fprintf(cgiOut, "    </td>\n");
	  fprintf(cgiOut, "    <td>%s</td>\n", getArtistName(getAlbumArtist(curr_id)));
	  fprintf(cgiOut, "  </tr>\n");

	  count++;
	  curr_id=allAlbums[count];
        }

	fprintf(cgiOut, "</tbody>\n");
	fprintf(cgiOut, "\n");
	fprintf(cgiOut, "</table>\n");

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
    fprintf(cgiOut, "    <td class=\"field\">%s&nbsp;<a class=\"small\" href=\"./?page=artist&amp;artistid=%d&amp;user=%s\">[View Artist]</a></td>\n", getArtistName(artistid), artistid, userCode);
    fprintf(cgiOut, "  </tr>\n");
    fprintf(cgiOut, "</tbody>\n");
    fprintf(cgiOut, "</table>\n");

    fprintf(cgiOut, "<hr />\n");

    /* is album on loan */
    {
      Boolean albumOnLoan=FALSE;

      /*********************/
      if(albumid == 4) {
	albumOnLoan=TRUE;
      }
      /*********************/

      if(albumOnLoan) {
	char *loanUserCode="admin";

	fprintf(cgiOut, "Album is on loan to <b>%s</b>\n", loanUserCode);

	if(strncmp(loanUserCode, userCode, MAXSIZE_USERCODE) == 0) {
 	  fprintf(cgiOut, "<form method=\"post\" action=\"./\">\n");
	  fprintf(cgiOut, "  <p>\n");
	  fprintf(cgiOut, "    <input type=\"hidden\" name=\"page\" value=\"loan\" />\n");
	  fprintf(cgiOut, "    <input type=\"hidden\" name=\"func\" value=\"delete\" />\n");
	  fprintf(cgiOut, "    <input type=\"hidden\" name=\"albumid\" value=\"%d\" />\n", albumid);
	  fprintf(cgiOut, "    <input type=\"hidden\" name=\"user\" value=\"%s\" />\n", userCode);
	  fprintf(cgiOut, "    <input type=\"submit\" value=\"Return Album\" />\n");
	  fprintf(cgiOut, "  </p>\n");
	  fprintf(cgiOut, "</form>\n");
	}
      }
      else {
	fprintf(cgiOut, "Album is not on loan\n");

	fprintf(cgiOut, "<form method=\"post\" action=\"./\">\n");
	fprintf(cgiOut, "  <p>\n");
	fprintf(cgiOut, "    <input type=\"hidden\" name=\"page\" value=\"loan\" />\n");
	fprintf(cgiOut, "    <input type=\"hidden\" name=\"func\" value=\"add\" />\n");
	fprintf(cgiOut, "    <input type=\"hidden\" name=\"albumid\" value=\"%d\" />\n", albumid);
	fprintf(cgiOut, "    <input type=\"hidden\" name=\"user\" value=\"%s\" />\n", userCode);
	fprintf(cgiOut, "    <input type=\"submit\" value=\"Borrow Album\" />\n");
	fprintf(cgiOut, "  </p>\n");
	fprintf(cgiOut, "</form>\n");
      }
    }
}
