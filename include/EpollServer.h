#ifndef EPOLLSERVER_H
#define EPOLLSERVER_H

#include <errno.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <netdb.h>
#include <strings.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <signal.h>
#include <sys/epoll.h>
#include "ThreadPool.h"

#define BUFFER_SIZE             2048
#define EPOLL_QUEUE_LEN         255
#define EDGE_SERVER             1
#define LEVEL_SERVER            2
#define LEVEL_SERVER_NO_THREAD  3

class EpollServer
{
    public:
        EpollServer(int s_port, int threads);
        ~EpollServer();
        void monitor_connections(int type);

    private:
        int fd_server, epoll_fd, port;
        ThreadPool * pool;
        struct epoll_event events[EPOLL_QUEUE_LEN], event;

        void s_exit(int);
        int create_listener();
        void callError(const char* message);
        void incoming_connection();
        void setup_server(int type);
};

#endif // EPOLLSERVER_H
