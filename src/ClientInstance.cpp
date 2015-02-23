/**
*	Program:	Server Comparison System
*	Author: 	Ramzi Chennafi
*	Date: 		Febuary 10 2015
*	Functions:
*		int start_instance(int port, std::string host)
*       void send_echo(int sd, int data_size, std::ofstream * clientlog)
*
*	Description
*	Contains functions to send data out
*/
#include "../include/ClientInstance.h"
/**
*	Function: 	start_instance(int port, std::string host)
*	Author: 	Ramzi Chennafi
*	Date:		Febuary 10 2015
*	Returns:	int - the socket created for the client
*	
*	Notes
*	Starts up a client instance. Connects to the server.
*		port - port to send data to
*		host - hostname of server
*/
int start_instance(int port, std::string host)
{
	int sd;
	struct hostent	*hp;
	struct sockaddr_in server;
	
	if ((sd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
	{
		perror("Cannot create socket");
		exit(1);
	}

	bzero((char *)&server, sizeof(struct sockaddr_in));
	server.sin_family = AF_INET;
	server.sin_port = htons(port);
	if ((hp = gethostbyname(host.c_str())) == NULL)
	{
		fprintf(stderr, "Unknown server address\n");
		exit(1);
	}
	bcopy(hp->h_addr, (char *)&server.sin_addr, hp->h_length);

	if (connect(sd, (struct sockaddr *)&server, sizeof(server)) == -1)
	{
		fprintf(stderr, "Can't connect to server\n");
		perror("connect");
		exit(1);
	}

	printf("Connected:    Server Name: %s\n", hp->h_name);
	
	return sd;
}
/**
*	Function: 	send_echo(int sd, int data_sie, ofstream * clientlog)
*	Author: 	Ramzi Chennafi
*	Date:		Febuary 10 2015
*	Returns:	void
*
*	Notes
*	Sends data to the server and recieves an echo from the server. Also logs all data to client_log.
*		sd - socket of the client
*		data_size - amount of data to send to the server
*		clientlog - pointer to the clientlog variable for data logging
*/	
void send_echo(int sd, int data_size, std::ofstream * clientlog)
{
    char * bp;
    struct timeval start_time;
    long milliseconds;

    int n = 0, bytes_to_read, total_bytes = 0;
    char * buffer = (char*)malloc(data_size);

    gettimeofday(&start_time, 0);
    memset(buffer, 1, data_size);

    send (sd, buffer, data_size, 0);

    memset(buffer, 0, data_size);

	bp = buffer;
	bytes_to_read = data_size;

	while ((n = recv (sd, bp, bytes_to_read, 0)) < data_size )
	{
		total_bytes += n;
		bp += n;
	}
    if (n == 0)
    {
        close(sd);
    }

	milliseconds = get_elapsed_time(start_time);

	*clientlog << data_size << "," << milliseconds << std::endl;
}

/**
*	Function: 	get_elapsed_time()
*	Author: 	Ramzi Chennafi
*	Date:		January 7 2015
*	Returns:	long, milliseconds elapsed
*
*	Notes
*	Gets the difference between the current start time and the current time 
*	then converts it to milliseconds and returns it.
*/
long get_elapsed_time(struct timeval start)
{
	struct timeval end;
	long seconds, useconds, mtime;

	gettimeofday(&end, 0);

	seconds  = end.tv_sec  - start.tv_sec;
    useconds = end.tv_usec - start.tv_usec;

    mtime = ((seconds) * 1000 + useconds/1000.0) + 0.5;

    return mtime;
}