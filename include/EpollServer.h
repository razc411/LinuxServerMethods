#ifndef EPOLLSERVER_H
#define EPOLLSERVER_H

#include <iostream>
#include <string>
#include <stdio.h>
#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <errno.h>
#include <unistd.h>
#include <cstring>
#include <fstream>
#include <sys/epoll.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <vector>
#include <fcntl.h>
#include <cstdlib>
#include "ThreadPool.h"

#define BUFFER_SIZE             2048
#define EPOLL_QUEUE_LEN         255
#define EDGE_SERVER             1
#define LEVEL_SERVER            2
#define LEVEL_SERVER_NO_THREAD  3

extern std::ofstream server_log;

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
