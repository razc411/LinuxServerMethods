/**
*	Program:	Server Comparison System
*	Author: 	Ramzi Chennafi
*	Date: 		Febuary 10 2015
*	Functions:
*		EpollServer::EpollServer(int s_port, int threads, std::ofstream * log) : port(s_port), server_log(log)
*       EpollServer::~EpollServer()
*       void EpollServer::monitor_connections(int type)
*       int EpollServer::create_listener()
*       void EpollServer::setup_server(int type)
*       void EpollServer::incoming_connection()
*       void incoming_data(int fd, std::ofstream * server_log)
*       int send_echo(int fd)
*       void EpollServer::s_exit(int code)
*       void EpollServer::callError(const char* error)
*
*	Description
*	This cpp file represents the server side of the comparison program. It allows for running a server in edge triggered 
*   epoll threaded mode, level triggered threaded epoll mode, and non threaded level triggered epoll mode. Chooses the server
*   based on the type passed to the constructor.
*/
#include "../include/EpollServer.h"
/**
*	Function: 	EpollServer(int s_port, int threads, std::ofstream * log) : port(s_port), server_log(log)
*	Author: 	Ramzi Chennafi
*	Date:		Febuary 10 2015
*	Returns:	void
*
*	Notes
*	Constructor for the EpollServer. Creates the threadpool and the listener socket for the server.
*       s_port - port for the server to run on
*       threads - number of threads to run in the pool
*       log - pointer to the logfile for writing
*       type - type of server to run, can be LEVEL_SERVER_NO_THREAD, EDGE_SERVER or LEVEL_SERVER
*/
std::mutex mtx;
int total_data_read = 0;

EpollServer::EpollServer(int s_port, int threads, std::ofstream * log, int type) : port(s_port), server_log(log)
{
    fd_server = create_listener();
    event = {0};
    if(type != LEVEL_SERVER_NO_THREAD)
    {
        pool = new ThreadPool(threads);
    }
}
/**
*	Function: 	EpollServer::~EpollServer()
*	Author: 	Ramzi Chennafi
*	Date:		Febuary 10 2015
*	Returns:	void
*
*	Notes
*	Destructor for the EpollServer.
*/
EpollServer::~EpollServer()
{
    delete [] pool;
}
/**
*	Function: 	void incoming_data(int fd, std::ofstream * server_log)
*	Author: 	Ramzi Chennafi
*	Date:		Febuary 10 2015
*	Returns:	void
*
*	Notes
*	Function for recieving and sending echos. Prints data to the server log.
*       fd - socket to send the data on
*       server_log - pointer to the server log file
*/
void incoming_data(int fd, std::ofstream * server_log)
{
    int	n, total_data = 0, bytes_read = 0;
	char buf[BUFFER_SIZE];
    sockaddr_in remote_addr;
    socklen_t address_len = sizeof(struct sockaddr_in);

    memset(buf, 0, BUFFER_SIZE);

    while ((n = recv (fd, &buf[bytes_read], BUFFER_SIZE - bytes_read, 0)) > 0)
    {
        bytes_read += n;
        total_data += n;

        if(bytes_read == BUFFER_SIZE)
        {
            send (fd, buf, bytes_read, 0);
            bytes_read = 0;
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

    if(total_data == 0)
    {
        return;
    }

    getpeername(fd, (sockaddr*)&remote_addr, &address_len);
    mtx.lock();
    total_data_read += total_data;
    *server_log << "Thread " << pthread_self() << ":" << inet_ntoa(remote_addr.sin_addr) << ": " << total_data << "," << total_data_read << std::endl;
    mtx.unlock();
}
/**
*   Function:   void EpollServer::setup_server(int type)
*   Author:     Ramzi Chennafi
*   Date:       Febuary 10 2015
*   Returns:    void
*
*   Notes
*   Sets the server up.
*       type - type of server to create, LEVEL_SERVER_NO_THREADS, EDGE_SERVER or LEVEL_SERVER.
*/
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
*	Function: 	void EpollServer::monitor_connections(int type)
*	Author: 	Ramzi Chennafi
*	Date:		Febuary 10 2015
*	Returns:	void
*
*	Notes
*	Main server loop. Monitors for incoming connections and incoming data.
*       type - type of server to create, LEVEL_SERVER_NO_THREADS, EDGE_SERVER or LEVEL_SERVER.
*/
void EpollServer::monitor_connections(int type)
{
    setup_server(type);

    while (1)
    {
        int num_fds = epoll_wait (epoll_fd, events, EPOLL_QUEUE_LEN, -1);

        for (int i = 0; i < num_fds; i++)
        {
            if (events[i].events & (EPOLLHUP | EPOLLERR))
            {
                fprintf(stderr, "Connection Closed\n");
                close(events[i].data.fd);
                continue;
            }

            if(events[i].events & EPOLLIN){
                if (events[i].data.fd == fd_server)
                {
                    incoming_connection();
                    continue;
                }
            }

            int temp = events[i].data.fd;

            if(type < LEVEL_SERVER_NO_THREAD)
            {
                pool->enqueue(incoming_data, temp, server_log);
            }

            else
            {
                incoming_data(temp, server_log);
            }
        
        }
    }
    
    close(fd_server);
    exit(0);
}
/**
*	Function: 	EpollServer::create_listener
*	Author: 	Ramzi Chennafi
*	Date:		Febuary 10 2015
*	Returns:	void
*
*	Notes
*	Creates the listener socket for the server.
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

    if (fcntl (fd_server, F_SETFL, O_NONBLOCK | fcntl (fd_server, F_GETFL, 0)) == -1)
    {
        callError("Failure at fcntl: listener.");
    }

    return fd_server;
}
/**
*	Function: 	void EpollServer::incoming_connection()
*	Author: 	Ramzi Chennafi
*	Date:		Febuary 10 2015
*	Returns:	void
*
*	Notes
*	Called when an incoming connection is detected. Writes connection data to the server_log.
*/
void EpollServer::incoming_connection()
{
    int fd_new;
    struct sockaddr_in remote_addr;
    socklen_t addr_size = sizeof(struct sockaddr_in);

    while((fd_new = accept4(fd_server, (struct sockaddr*)&remote_addr, &addr_size, SOCK_NONBLOCK)) != -1)
    {
        event.data.fd = fd_new;

        if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, fd_new, &event) == -1)
        {
            callError("epoll_ctl new incoming");
        }
    }

    *server_log << "New client at IP " <<  inet_ntoa(remote_addr.sin_addr) << " added." << std::endl;
}
/**
*	Function: 	void EpollServer::s_exit(int code)
*	Author: 	Ramzi Chennafi
*	Date:		Febuary 10 2015
*	Returns:	void
*
*	Notes
*	Exit function for the server, closes the listening socket.
*       int code - code for the error.
*/
void EpollServer::s_exit(int code)
{
    close(fd_server);
    exit(1);
}
/**
*	Function: 	void EpollServer::callError(const char* error)
*	Author: 	Ramzi Chennafi
*	Date:		Febuary 10 2015
*	Returns:	void
*
*	Notes
*	Prints the error message for the sent code and calls server_exit.
*       error - the error message to print.
*/
void EpollServer::callError(const char* error)
{
    perror(error);
    s_exit(1);
}

