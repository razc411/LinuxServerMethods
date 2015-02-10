#include <iostream>
#include <EpollServer.h>
#define OPTIONS "e:l:t:"

using namespace std;
/**
*	Function: 	main(int, char **)
*	Author: 	Ramzi Chennafi
*	Date:		January 7 2015
*	Returns:	int, error code
*
*	Notes
*	Main entry loop for the program, checks for arguments and begins the program according
*	to the arguments.
*/
int main(int argc, char ** argv)
{
	char c;

	if((c = getopt(argc, argv, OPTIONS)) != -1)
    if(c == 'e')
    {
        int port = atoi(optarg);
        EpollServer eServer(port);
        eServer.monitor_connections();
    }

	return 0;
}
