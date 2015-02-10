/**
*	Program:	Server Comparison System
*	Author: 	Ramzi Chennafi
*	Date: 		Febuary 10 2015
*	Functions:
*		EpollServer()
*       ~EpollServer()
*       void monitor_connections()
*       int create_listener()
*       void incoming_connection(int fd_server, struct sockaddr* remote_addr, struct * epoll_event event)
*       int incoming_data(int fd)
*       int send_echo(int fd)
*       int s_exit()
*       void callError(string error)
*
*	Description
*	This program allows the user to do a comparison between threads and processes using prime number
*	calculations. Allows for specification of the number of threads/processes and the maximum number to calculate
*	primes up to.
*
*	Takes the switches...
*		-p - specifies to do calculation in process mode
*		-t - specifies to do calculation in thread mode
*		-m arg - specifies the maximum value to calculate prime to, takes an argument from 2 to infinity.
*		-n arg - specifies the amount of threads/processes to be made, takes an argument from 1 to infinity.
*		-v - causes each prime number to be printed to terminal, causes an extreme increase in processing time and
*			may break the program if factoring a number greater than 10 000 000.
*		-i arg - specifies the iterations to run the testing for. Defaults to 1 iteration.
*		-f arg - specifies the filename for the log.
*
*/
#include "EpollServer.h"
/**
*	Function: 	monitor_connections
*	Author: 	Ramzi Chennafi
*	Date:		Febuary 10 2015
*	Returns:	void
*
*	Notes
*	Core monitoring function of the epoll server. Sets the server up and checks the epoll events for socket events
*   and responds accordingly. Manages listening, reading and writing.
*/
EpollServer::EpollServer(int s_port) : Server(s_port)
{
    fd_server = create_listener();
}
/**
*	Function: 	monitor_connections
*	Author: 	Ramzi Chennafi
*	Date:		Febuary 10 2015
*	Returns:	void
*
*	Notes
*	Core monitoring function of the epoll server. Sets the server up and checks the epoll events for socket events
*   and responds accordingly. Manages listening, reading and writing.
*/
EpollServer::~EpollServer()
{
    //dtor
}
/**
*	Function: 	monitor_connections
*	Author: 	Ramzi Chennafi
*	Date:		Febuary 10 2015
*	Returns:	void
*
*	Notes
*	Core monitoring function of the epoll server. Sets the server up and checks the epoll events for socket events
*   and responds accordingly. Manages listening, reading and writing.
*/
void EpollServer::monitor_connections()
{
    int num_fds, epoll_fd;
    struct sockaddr_in remote_addr;

    if((epoll_fd = epoll_create(EPOLL_QUEUE_LEN)) == -1)
    {
        callError("Failure at epoll fd create.");
    }

    event.events = EPOLLIN | EPOLLERR | EPOLLHUP | EPOLLET;
    event.data.fd = fd_server;
    if (epoll_ctl (epoll_fd, EPOLL_CTL_ADD, fd_server, &event) == -1)
    {
        callError("Failure at epoll_ctl epoll fd.");
    }

    while (1)
    {
        num_fds = epoll_wait (epoll_fd, events, EPOLL_QUEUE_LEN, -1);
        if (num_fds < 0)
        {
            continue;
        }

        for (int i = 0; i < num_fds; i++)
        {
            if (events[i].events & (EPOLLHUP | EPOLLERR))
            {
                fprintf(stderr, "epoll: EPOLLERR\n");
                close(events[i].data.fd);
                continue;
            }

            if (events[i].data.fd == fd_server)
            {
                incoming_connection(&remote_addr);
                continue;
            }

            if (!incoming_data(events[i].data.fd))
            {
                printf("Closing fd %d.\n", events[i].data.fd);
                close(events[i].data.fd);
            }
        }
    }
    close(fd_server);
    exit(0);
}
/**
*	Function: 	monitor_connections
*	Author: 	Ramzi Chennafi
*	Date:		Febuary 10 2015
*	Returns:	void
*
*	Notes
*	Core monitoring function of the epoll server. Sets the server up and checks the epoll events for socket events
*   and responds accordingly. Manages listening, reading and writing.
*/
int EpollServer::create_listener()
{
    struct sockaddr_in addr;
    int arg = 1;

    if((fd_server = socket (AF_INET, SOCK_STREAM, 0)) = -1)
    {
        callError("Failure at creating listener.");
    }

    if (setsockopt (fd_server, SOL_SOCKET, SO_REUSEADDR, &arg, sizeof(arg)) == -1)
    {
        callError("Failure at setsockopt: listener.");
    }

    if (fcntl (fd_server, F_SETFL, O_NONBLOCK | fcntl (fd_server, F_GETFL, 0)) == -1)
    {
        callError("Failure at fcntl: listener.");
    }

    memset (&addr, 0, sizeof (struct sockaddr_in));
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    addr.sin_port = htons(port);
    if (bind (fd_server, (struct sockaddr*) &addr, sizeof(addr)) == -1)
    {
        callError("Failure at bind");
    }

    if (listen (fd_server, SOMAXCONN) == -1)
    {
        callError("Failure at listen.");
    }

    return fd_server;
}
/**
*	Function: 	monitor_connections
*	Author: 	Ramzi Chennafi
*	Date:		Febuary 10 2015
*	Returns:	void
*
*	Notes
*	Core monitoring function of the epoll server. Sets the server up and checks the epoll events for socket events
*   and responds accordingly. Manages listening, reading and writing.
*/
void EpollServer::incoming_connection(struct sockaddr_in* remote_addr)
{
    int fd_new;
    socklen_t addr_size = sizeof(struct sockaddr_in);

    if ((fd_new = accept (fd_server, (struct sockaddr*)remote_addr, &addr_size)) == -1)
    {
        if (errno != EAGAIN && errno != EWOULDBLOCK)
        {
            perror("accept incoming");
        }
    }

    if (fcntl (fd_new, F_SETFL, O_NONBLOCK | fcntl(fd_new, F_GETFL, 0)) == -1)
    {
        callError("Failure at fcntl.");
    }
    event.data.fd = fd_new;

    if (epoll_ctl (epoll_fd, EPOLL_CTL_ADD, fd_new, &event) == -1)
    {
        perror("epoll_ctl new incoming");
    }

    printf("Remote Address:  %s\n", inet_ntoa(remote_addr->sin_addr));
}
/**
*	Function: 	monitor_connections
*	Author: 	Ramzi Chennafi
*	Date:		Febuary 10 2015
*	Returns:	void
*
*	Notes
*	Core monitoring function of the epoll server. Sets the server up and checks the epoll events for socket events
*   and responds accordingly. Manages listening, reading and writing.
*/
int EpollServer::incoming_data(int fd)
{
    int	n, bytes_to_read, bytes_incoming;
	char *bp, buf[BUFLEN];

	while (1)
	{
		bp = buf;
		bytes_to_read = BUFLEN;
		while ((n = recv (fd, bp, bytes_to_read, 0)) < HEADER_SIZE)
		{
			bp += n;
			bytes_to_read -= n;
		}

        bytes_incoming = (int)*buf;
        memset(buf, BUFLEN, 0);
        bp = buf;

		while ((n = recv (fd, bp, bytes_to_read, 0)) < bytes_incoming)
		{
			bp += n;
			bytes_to_read -= n;
		}

		send (fd, buf, BUFLEN, 0);
		close (fd);
		return 1;
	}
	close(fd);
	return(0);
}
/**
*	Function: 	monitor_connections
*	Author: 	Ramzi Chennafi
*	Date:		Febuary 10 2015
*	Returns:	void
*
*	Notes
*	Core monitoring function of the epoll server. Sets the server up and checks the epoll events for socket events
*   and responds accordingly. Manages listening, reading and writing.
*/
void EpollServer::s_exit(int code)
{
    close(fd_server);
    exit(1);
}
/**
*	Function: 	monitor_connections
*	Author: 	Ramzi Chennafi
*	Date:		Febuary 10 2015
*	Returns:	void
*
*	Notes
*	Core monitoring function of the epoll server. Sets the server up and checks the epoll events for socket events
*   and responds accordingly. Manages listening, reading and writing.
*/
void EpollServer::callError(const char* error)
{
    perror(error);
    s_exit(1);
}
