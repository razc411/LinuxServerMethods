#include "../include/EpollServer.h"
#include "../include/Client.h"

#define EDGE_SERVER             1
#define LEVEL_SERVER            2
#define LEVEL_SERVER_NO_THREAD  3
#define CLIENT                  4
#define OPTIONS                 "ed:lwc:Cp:h:t:"

/**
*	Function: 	main(int, char **)
*	Author: 	Ramzi Chennafi
*	Date:		Feb 21 2015
*	Returns:	int, error code
*
*	Notes
*	Main entry loop for the program, checks for arguments and begins the program according
*	to the arguments. Can establish 3 different types of servers (EDGE_SERVER, LEVEL_SERVER, LEVEL_SERVER_NO_THREAD)
*   and a client. Below are the arguments, these are optional.
*       p <port>        - port for the server to be established on or the client to send to.     DEFAULT: 5001
*       t <num threads> - number of threads for the threaded servers to run on.                  DEFAULT: 4
*       h <hostname>    - host to send data to for the client.                                   DEFAULT: LOCALHOST
*       d <data size>   - the amount of data in bytes for each client to send.                   DEFAULT: 1000000
*       c <num clients> - the number of clients to create :                                      DEFAULT: 1
*       
*       1 of these arguments is required to run
*       e - runs the edge triggered threaded server.
*       w - runs the level triggered non threaded server.
*       l - runs the level triggered threaded server.
*       C - runs the client.
*/
int main(int argc, char ** argv)
{
    char c;
    int threads = 4, port = 5001, clients = 1, data_size = 1000000;
    int type = EDGE_SERVER;
    std::string hostname("localhost");
    std::ofstream server_log;
    server_log.open("server_log");

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
            case 'e':
                type = EDGE_SERVER;
                break;
            case 'l':
                type = LEVEL_SERVER;
                break;
            case 'w':
                type = LEVEL_SERVER_NO_THREAD;
                break;
            case 'C':
                type = CLIENT;
                break;
        }
    }

    EpollServer * eServer;

    switch(type)
    {
        case EDGE_SERVER:
            eServer = new EpollServer(port, threads, &server_log, type);
            eServer->monitor_connections(EDGE_SERVER);
            break;
        case LEVEL_SERVER:
            eServer = new EpollServer(port, threads, &server_log, type);
            eServer->monitor_connections(LEVEL_SERVER);
            break;
        case LEVEL_SERVER_NO_THREAD:
            eServer = new EpollServer(port, threads, &server_log, type);
            eServer->monitor_connections(LEVEL_SERVER_NO_THREAD);
            break;
        case CLIENT:
            Client echo_client(port, hostname, clients, data_size);
            break;
    }

    return 0;
}
