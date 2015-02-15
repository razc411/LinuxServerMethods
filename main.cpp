#include <iostream>
#include <EpollServer.h>
#include <Client.h>
#define OPTIONS "e:l:t:c:"

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

    while((c = getopt(argc, argv, OPTIONS)) != -1)
    {
        int port = atoi(optarg);

        if(c == 'e')
        {
            EpollServer eServer(port);
            eServer.monitor_connections("EDGE");
            break;
        }
        else if(c == 'l')
        {
            EpollServer eServer(port);
            eServer.monitor_connections("LEVEL");
            break;
        }
        else if(c == 't')
        {
            EpollServer eServer(port);
            eServer.monitor_connections("POLLING");
            break;
        }
        else if(c == 'c')
        {

            Client echo_client(port, "localhost");
            echo_client.connect_client();
            break;
        }
    }

    return 0;
}
