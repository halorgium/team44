#include "../../shared/defines.h"
#include "lib.h"
#include "load.h"
#include "net.h"

/* main loads database then waits in a loop for calls from the cgi
   and calls functiond here then sends the info back.
   main only handles one call from the cgi at a time (tasks will
   have to be queued)
*/
int main(int arg, char **args) {
    loadDatabase();

    startNetworkServer();

    return 1;
}
