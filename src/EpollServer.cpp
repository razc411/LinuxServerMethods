#include "EpollServer.h"

EpollServer::EpollServer()
{
    //ctor
}

EpollServer::~EpollServer()
{
    //dtor
}

EpollServer::monitor_connections()
{
    int i, arg;
    int num_fds, fd_new, epoll_fd;
    static struct epoll_event events[EPOLL_QUEUE_LEN], event;
    int port = SERVER_PORT;
    struct sockaddr_in addr, remote_addr;
    socklen_t addr_size = sizeof(struct sockaddr_in);

    fd_server = socket (AF_INET, SOCK_STREAM, 0);
    if (fd_server == -1)
        SystemFatal("socket");

    arg = 1;
    if (setsockopt (fd_server, SOL_SOCKET, SO_REUSEADDR, &arg, sizeof(arg)) == -1)
        SystemFatal("setsockopt");

    if (fcntl (fd_server, F_SETFL, O_NONBLOCK | fcntl (fd_server, F_GETFL, 0)) == -1)
        SystemFatal("fcntl");

    memset (&addr, 0, sizeof (struct sockaddr_in));
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    addr.sin_port = htons(port);
    if (bind (fd_server, (struct sockaddr*) &addr, sizeof(addr)) == -1)
        SystemFatal("bind");

    if (listen (fd_server, SOMAXCONN) == -1)
        SystemFatal("listen");

    epoll_fd = epoll_create(EPOLL_QUEUE_LEN);
    if (epoll_fd == -1)
        SystemFatal("epoll_create");

    event.events = EPOLLIN | EPOLLERR | EPOLLHUP | EPOLLET;
    event.data.fd = fd_server;
    if (epoll_ctl (epoll_fd, EPOLL_CTL_ADD, fd_server, &event) == -1)
        SystemFatal("epoll_ctl");

    while (TRUE)
    {
        //struct epoll_event events[MAX_EVENTS];
        num_fds = epoll_wait (epoll_fd, events, EPOLL_QUEUE_LEN, -1);
        if (num_fds < 0)
            SystemFatal ("Error in epoll_wait!");

        for (i = 0; i < num_fds; i++)
        {
            // Case 1: Error condition
            if (events[i].events & (EPOLLHUP | EPOLLERR))
            {
                fputs("epoll: EPOLLERR", stderr);
                close(events[i].data.fd);
                continue;
            }
            assert (events[i].events & EPOLLIN);

            // Case 2: Server is receiving a connection request
            if (events[i].data.fd == fd_server)
            {
                //socklen_t addr_size = sizeof(remote_addr);
                fd_new = accept (fd_server, (struct sockaddr*) &remote_addr, &addr_size);
                if (fd_new == -1)
                {
                    if (errno != EAGAIN && errno != EWOULDBLOCK)
                    {
                        perror("accept");
                    }
                    continue;
                }

                // Make the fd_new non-blocking
                if (fcntl (fd_new, F_SETFL, O_NONBLOCK | fcntl(fd_new, F_GETFL, 0)) == -1)
                    SystemFatal("fcntl");

                // Add the new socket descriptor to the epoll loop
                event.data.fd = fd_new;
                if (epoll_ctl (epoll_fd, EPOLL_CTL_ADD, fd_new, &event) == -1)
                    SystemFatal ("epoll_ctl");

                printf(" Remote Address:  %s\n", inet_ntoa(remote_addr.sin_addr));
                continue;
            }

            // Case 3: One of the sockets has read data
            if (!ClearSocket(events[i].data.fd))
            {
                // epoll will remove the fd from its set
                // automatically when the fd is closed
                close (events[i].data.fd);
            }
        }
    }
    close(fd_server);
    exit (EXIT_SUCCESS);
}

EpollServer::incomming_connection()
{
    if (events[i].data.fd == fd_server)
    {
        fd_new = accept (fd_server, (struct sockaddr*) &remote_addr, &addr_size);
        if (fd_new == -1)
        {
            if (errno != EAGAIN && errno != EWOULDBLOCK)
            {
                perror("accept");
            }
            continue;
        }

        if (fcntl (fd_new, F_SETFL, O_NONBLOCK | fcntl(fd_new, F_GETFL, 0)) == -1)
            SystemFatal("fcntl");

        event.data.fd = fd_new;
        if (epoll_ctl (epoll_fd, EPOLL_CTL_ADD, fd_new, &event) == -1)
            SystemFatal ("epoll_ctl");

        printf(" Remote Address:  %s\n", inet_ntoa(remote_addr.sin_addr));
}

EpollServer::incoming_data()
{

}

EpollServer::send_echo()
{

}

EpollServer::s_exit()
{

}
