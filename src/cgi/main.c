#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "cgic.h"
#include "globals.h"
#include "../shared/defines.h"

static void printLinks(FILE *);
static void printHeader(FILE *);
static void printFooter(FILE *);
static void theRealWork(void);

int cgiMain() {
   
    _currUserLogon=E_NOUSER;

    /* Start outputting the header */
    printHeader(cgiOut);

    /* Do the real work */
    theRealWork();
    
    /* Start outputting the footer */
    printFooter(cgiOut);

    return 0;
}

static void theRealWork(void) {
    int result=0;

    char pageName[MAXSIZE_PAGENAME]={'\0'};

    int userHash=0;
   
    /* At this point we start the dynamic content, error page may be shown */

    /* We might need the database so load it up */
    result=loadDatabase();
    if(result != DB_LOAD_SUCCESS) {
	/* some problem with loading */
	fprintf(cgiOut, "<td>Some DB Load problem</td>\n</tr>\n<tr>\n");
	return;
    }
    
    result = cgiFormInteger("hash", &userHash, -1);
    if(result != cgiFormSuccess || userHash == -1) {
	/* No hash */

	/* check for login attempt */
	int dologin = 0;
	int result2 = cgiFormInteger("dologin", &dologin, FALSE);
	
	if(result2 != cgiFormSuccess || dologin == FALSE) {
	    /* No attempt */
	    /* Display login */
	    strcpy(pageName, "login");
	}
	else {
	    /* Hey someone is trying to login */
	    /* Get the userCode */
	    char *userCode=malloc(sizeof(char)*(MAXSIZE_USERCODE+1));
	    
	    result = cgiFormStringNoNewlines("usercode", userCode, MAXSIZE_USERCODE);
	    if(result != cgiFormSuccess) {
		/* Unsuccessful */
		/* Display login */
		fprintf(cgiOut, "<td>No such user</td></tr>\n<tr>\n");
		strcpy(pageName, "login");
	    }
	    else {
		/* Now check user */
		int tempHash=makeUserID(userCode);
		if(getUserExists(tempHash) == FALSE) {
		    /* No such user */
		    fprintf(cgiOut, "<td>No such user</td></tr>\n<tr>\n");
		    strcpy(pageName, "login");
		}
		else {
		    _currUserLogon=tempHash;
		    strcpy(pageName, "home");
		}
	    }
	}
    }
    else {
	/* Check & get user */
	/* Make sure logged in */
	if(getUserExists(userHash) == FALSE) {
	    /* UserHash invalid */
	    fprintf(cgiOut, "<td>Bad hash</td></tr>\n<tr>\n");
	    strcpy(pageName, "login");
	}
	else {
	    _currUserLogon=userHash;
	    
	    result = cgiFormStringNoNewlines("page", pageName, MAXSIZE_PAGENAME);
	    if(result != cgiFormSuccess) {
		strcpy(pageName, "home");
	    }
	}
    }

    /* If links are neccessary print them */
    if(_currUserLogon != E_NOUSER) {
	printLinks(cgiOut);
    }

    {
	/* Start outputting the body */

	fprintf(cgiOut, "<td id=\"bodycontent\">\n");
	fprintf(cgiOut, "<a name=\"navskip\"></a>\n");
	fprintf(cgiOut, "<!-- Start Body -->\n");


	if(strncmp(pageName, "login", MAXSIZE_PAGENAME) == 0) {
	    printLogin();
	}
	else if(strncmp(pageName, "news", MAXSIZE_PAGENAME) == 0) {
	    printNews();
	}
	else if(strncmp(pageName, "contact", MAXSIZE_PAGENAME) == 0) {
	    printContact();
	}
	else if(strncmp(pageName, "user", MAXSIZE_PAGENAME) == 0) {
	    printUser();
	}
	else if(strncmp(pageName, "album", MAXSIZE_PAGENAME) == 0) {
	    printAlbum();
	}
	else if(strncmp(pageName, "artist", MAXSIZE_PAGENAME) == 0) {
	    printArtist();
	}
	else if(strncmp(pageName, "usercomment", MAXSIZE_PAGENAME) == 0) {
	    printUserComment();
	}
	else if(strncmp(pageName, "albumcomment", MAXSIZE_PAGENAME) == 0) {
	    printAlbumComment();
	}
	else if(strncmp(pageName, "artistcomment", MAXSIZE_PAGENAME) == 0) {
	    printArtistComment();
	}
	else if(strncmp(pageName, "loan", MAXSIZE_PAGENAME) == 0) {
	    printLoan();
	}
	else {
	    /* Default */
	    printHome();
	}

	fprintf(cgiOut, "<!-- End Body -->\n");
	fprintf(cgiOut, "</td>\n");
    }
}

static void printLinks(FILE *output) {

    fprintf(cgiOut, "<td class=\"linkholder\">\n");
    fprintf(cgiOut, "<!-- Start Links -->\n");
    fprintf(cgiOut, "<table class=\"links\">\n");
    fprintf(cgiOut, "  <tbody>\n");

    fprintf(cgiOut, "  <tr>\n    <td class=\"username\">You are logged on as&nbsp;<b>%s</b></td>\n  </tr>\n", getUserCode(_currUserLogon));
    fprintf(cgiOut, "  <tr>\n    <td class=\"spacer\">&nbsp;</td>\n  </tr>\n");

    fprintf(output, "  <tr>\n    <td class=\"link\"><a class=\"buttonref\" href=\"./?hash=%d\">Home</a></td>\n  </tr>\n", _currUserLogon);
    fprintf(output, "  <tr>\n    <td class=\"link\"><a class=\"buttonref\" href=\"./?page=news&amp;hash=%d\">News</a></td>\n  </tr>\n", _currUserLogon);
    fprintf(output, "  <tr>\n    <td class=\"link\"><a class=\"buttonref\" href=\"./?page=contact&amp;hash=%d\">Contact&nbsp;Us</a></td>\n  </tr>\n", _currUserLogon);
    fprintf(output, "  <tr>\n    <td class=\"link\"><a class=\"buttonref\" href=\"./?page=logout\">Logout</a></td>\n  </tr>\n");
    fprintf(output, "  <tr>\n    <td class=\"spacer\">&nbsp;</td>\n  </tr>\n");
    fprintf(output, "  <tr>\n    <td class=\"link\"><a class=\"buttonref\" href=\"./?page=user&amp;userid=%d&amp;hash=%d\">My&nbsp;Account</a></td>\n  </tr>\n", _currUserLogon, _currUserLogon);
    fprintf(output, "  <tr>\n    <td class=\"link\"><a class=\"buttonref\" href=\"./?page=loan&amp;func=view&amp;userid=%d&amp;hash=%d\">My&nbsp;Borrowing&nbsp;History</a></td>\n  </tr>\n", _currUserLogon, _currUserLogon);
    fprintf(output, "  <tr>\n    <td class=\"link\"><a class=\"buttonref\" href=\"./?page=usercomment&amp;userid=%d&amp;hash=%d\">My&nbsp;Comments</a></td>\n  </tr>\n", _currUserLogon, _currUserLogon);
    fprintf(output, "  <tr>\n    <td class=\"spacer\">&nbsp;</td>\n  </tr>\n");
    fprintf(output, "  <tr>\n    <td class=\"link\"><a class=\"buttonref\" href=\"./?page=artist&amp;hash=%d\">View&nbsp;All&nbsp;Artists</a></td>\n  </tr>\n", _currUserLogon);
    if(isUserLibrarian(_currUserLogon) == TRUE) {
	fprintf(output, "  <tr>\n    <td class=\"link\"><a class=\"buttonref\" href=\"./?page=artist&amp;func=add&amp;hash=%d\">Add&nbsp;New&nbsp;Artist</a></td>\n  </tr>\n", _currUserLogon);
	fprintf(output, "  <tr>\n    <td class=\"link\"><a class=\"buttonref\" href=\"./?page=artistcomment&amp;func=add&amp;hash=%d\">Comment&nbsp;about&nbsp;a&nbsp;Artist</a></td>\n  </tr>\n", _currUserLogon);
	fprintf(output, "  <tr>\n    <td class=\"spacer\">&nbsp;</td>\n  </tr>\n");
    }
    fprintf(output, "  <tr>\n    <td class=\"link\"><a class=\"buttonref\" href=\"./?page=album&amp;hash=%d\">View&nbsp;All&nbsp;Albums</a></td>\n  </tr>\n", _currUserLogon);
    if(isUserLibrarian(_currUserLogon) == TRUE) {
	fprintf(output, "  <tr>\n    <td class=\"link\"><a class=\"buttonref\" href=\"./?page=album&amp;func=add&amp;hash=%d\">Add&nbsp;New&nbsp;Album</a></td>\n  </tr>\n", _currUserLogon);
	fprintf(output, "  <tr>\n    <td class=\"link\"><a class=\"buttonref\" href=\"./?page=albumcomment&amp;func=add&amp;hash=%d\">Comment&nbsp;about&nbsp;a&nbsp;Album</a></td>\n  </tr>\n", _currUserLogon);
	fprintf(output, "  <tr>\n    <td class=\"spacer\">&nbsp;</td>\n  </tr>\n");
	fprintf(output, "  <tr>\n    <td class=\"link\"><a class=\"buttonref\" href=\"./?page=user&amp;hash=%d\">View&nbsp;All&nbsp;Users</a></td>\n  </tr>\n", _currUserLogon);
	fprintf(output, "  <tr>\n    <td class=\"link\"><a class=\"buttonref\" href=\"./?page=user&amp;func=add&amp;hash=%d\">Add&nbsp;New&nbsp;User</a></td>\n  </tr>\n", _currUserLogon);
	fprintf(output, "  <tr>\n    <td class=\"link\"><a class=\"buttonref\" href=\"./?page=usercomment&amp;func=add&amp;hash=%d\">Comment&nbsp;about&nbsp;a&nbsp;User</a></td>\n  </tr>\n", _currUserLogon);
    }

    fprintf(output, "  </tbody>\n");
    fprintf(output, "</table>\n");
    fprintf(output, "<!-- End Links -->\n");
    fprintf(output, "</td>\n");
}

static void printHeader(FILE *output) {
    cgiHeaderContentType("text/html");

    fprintf(output, "<?xml version=\"1.0\" encoding=\"ISO-8859-1\"?>\n");
    fprintf(output, "<!DOCTYPE html PUBLIC \"-//W3C//DTD XHTML 1.0 Strict//EN\"\n");
    fprintf(output, " \"http://www.w3.org/TR/xhtml1/DTD/xhtml1-strict.dtd\">\n");
    fprintf(output, "<html>\n");
    fprintf(output, "  <head>\n");
    fprintf(output, "    <meta http-equiv=\"Content-Type\" content=\"text/html; charset=iso-8859-1\" />\n");
    fprintf(output, "    <meta http-equiv=\"Content-Style-Type\" content=\"text/css\" />\n");
    fprintf(output, "    <meta name=\"keywords\" content=\"music, library\" />\n");
    fprintf(output, "    <meta name=\"description\" content=\"The Music Library\" />\n");
    fprintf(output, "    <title>Team 44 Music Library</title>\n");
    fprintf(output, "    <link rel=\"stylesheet\" type=\"text/css\" href=\"musiclib.css\" />\n");
    fprintf(output, "    <link rel=\"made\" href=\"mailto:c204g18@cosc.canterbury.ac.nz\" />\n");
    fprintf(output, "    <!--link rel=\"shortcut icon\" href=\"favicon.ico\" /-->\n");
    fprintf(output, "  </head>\n");
    fprintf(output, "  <body>\n");
    fprintf(output, "    <div id=\"hiddenlink\"><a href=\"#navskip\"><img src=\"1px.gif\" width=\"1\" height=\"10\" alt=\"Go to main content\" style=\"border: 0px;\" /></a></div>\n");
    fprintf(output, "    <table id=\"container\">\n");
    fprintf(output, "    <thead>\n");
    fprintf(output, "  <tr>\n");
    fprintf(output, "<td id=\"header\" colspan=\"2\">\n");
    fprintf(output, "<!-- Start Header -->\n");
    fprintf(output, "  <div id=\"maintitle\"><img src=\"logo.png\" alt=\"Team44 Music Library\" /></div>\n");
    fprintf(output, "<!-- End Header -->\n");
    fprintf(output, "</td>\n");
    fprintf(output, "  </tr>\n");
    fprintf(output, "  <tr id=\"topspacera\">\n");
    fprintf(output, "<td id=\"topspacerb\" colspan=\"2\">\n");
    fprintf(output, "<img src=\"1px.gif\" width=\"1\" height=\"4\" alt=\"\" />\n");
    fprintf(output, "</td>\n");
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

static void printFooter(FILE *output) {
    fprintf(output, "  </tr>\n");
    fprintf(output, "    </tbody>\n");
    fprintf(output, "</table>\n");
    fprintf(output, "</body>\n");
    fprintf(output, "</html>\n");
}
