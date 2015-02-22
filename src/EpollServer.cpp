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
#include "../include/EpollServer.h"
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
EpollServer::EpollServer(int s_port, int threads) : port(s_port)
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
    delete [] pool;
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
void incoming_data(int fd)
{
    int	n, bytes_read = 0;
	char *bp, buf[BUFFER_SIZE];

    memset(buf, 0, BUFFER_SIZE);
    bp = buf;

    while ((n = recv (fd, bp, BUFFER_SIZE - bytes_read, 0)) > 0)
    {
        bp += n;
        bytes_read += n;

        if(BUFFER_SIZE == bytes_read)
        {
            send (fd, buf, bytes_read, 0);
            bytes_read = 0;
            memset(buf, 0, BUFFER_SIZE);
            bp = buf;
        }
    }
    if(n == -1 && (errno == EAGAIN || errno == EWOULDBLOCK))
    {
        send (fd, buf, bytes_read, 0);
    }
    else if (n == 0)
    {
        close(fd);
    }
}

void EpollServer::setup_server(int type)
{
    if((epoll_fd = epoll_create(EPOLL_QUEUE_LEN)) == -1)
    {
        callError("Failure at epoll fd create.");
    }

    if(type == EDGE_SERVER)
    {
        event.events = EPOLLIN | EPOLLERR | EPOLLHUP | EPOLLET;
    }
    else
    {
        event.events = EPOLLIN | EPOLLERR | EPOLLHUP;
    }

    event.data.fd = fd_server;
    if (epoll_ctl (epoll_fd, EPOLL_CTL_ADD, fd_server, &event) == -1)
    {
        callError("Failure at epoll_ctl epoll fd.");
    }
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
void EpollServer::monitor_connections(int type)
{
   // ThreadPool pool(4);
    setup_server(type);

    while (1)
    {
        int num_fds = epoll_wait (epoll_fd, events, EPOLL_QUEUE_LEN, -1);
        if (num_fds < 0)
        {
            continue;
        }

        for (int i = 0; i < num_fds; i++)
        {
            if (events[i].events & (EPOLLHUP | EPOLLERR))
            {
                fprintf(stderr, "Connection Closed");
                close(events[i].data.fd);
                continue;
            }

            if (events[i].data.fd == fd_server)
            {
                incoming_connection();
                continue;
            }

            int temp = events[i].data.fd;

            // if(type < LEVEL_SERVER_NO_THREAD)
            // {
            //     pool.enqueue(incoming_data, temp);
            //     continue;
            // }
            // else
            // {
                incoming_data(temp);
            //}
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

    if((fd_server = socket (AF_INET, SOCK_STREAM, 0)) == -1)
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
void EpollServer::incoming_connection()
{
    int fd_new;
    struct sockaddr_in remote_addr;
    socklen_t addr_size = sizeof(struct sockaddr_in);

    if ((fd_new = accept(fd_server, (struct sockaddr*)&remote_addr, &addr_size)) == -1)
    {
        if (errno != EAGAIN && errno != EWOULDBLOCK)
        {
            perror("accept incoming");
        }
    }

    if (fcntl(fd_new, F_SETFL, O_NONBLOCK | fcntl(fd_new, F_GETFL, 0)) == -1)
    {
        callError("Failure at fcntl.");
    }
    event.data.fd = fd_new;

    if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, fd_new, &event) == -1)
    {
        printf("%d", errno);
        callError("epoll_ctl new incoming");
    }

    printf("Remote Address:  %s\n", inet_ntoa(remote_addr.sin_addr));
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

