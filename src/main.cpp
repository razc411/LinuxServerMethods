#include "../include/EpollServer.h"
#include "../include/Client.h"

#define EDGE_SERVER             1
#define LEVEL_SERVER            2
#define LEVEL_SERVER_NO_THREAD  3
#define OPTIONS                 "eltcp:h:t:"
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
    int threads = 4, port = 5001, clients = 1, data_size = 1000;
    std::string hostname("localhost");

    while((c = getopt(argc, argv, OPTIONS)) != -1)
    {
        switch(c)
        {
            case 'p':
                port = atoi(optarg);
                break;

            case 't':
                threads = atoi(optarg);
                break;

            case 'h':
                hostname = optarg;
                break;

            case 'd':
                data_size = atoi(optarg);
                break;
            
            case 'c':
                clients = atoi(optarg);
                break;
        }

        if(c == 'e')
        {
            EpollServer eServer(port, threads);
            eServer.monitor_connections(EDGE_SERVER);
            break;
        }
        else if(c == 'l')
        {
            EpollServer eServer(port, threads);
            eServer.monitor_connections(LEVEL_SERVER);
            break;
        }
        else if(c == 't')
        {
            EpollServer eServer(port, threads);
            eServer.monitor_connections(LEVEL_SERVER_NO_THREAD);
            break;
        }
        else if(c == 'c')
        {
            Client echo_client(port, hostname, clients, data_size);
            echo_client.connect_clients(port, hostname, data_size);
            break; 
        }
    }

    return 0;
}
