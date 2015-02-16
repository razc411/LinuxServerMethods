#ifndef EPOLLSERVER_H
#define EPOLLSERVER_H
#define BUFLEN 255
#define EPOLL_QUEUE_LEN	255
#define HEADER_SIZE 4

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
#include <ThreadPool.h>
#include <assert.h>

class EpollServer
{
    public:
        EpollServer() : port(5000) {}
        EpollServer(int s_port);
        ~EpollServer();
        void monitor_connections(const char * type);
    protected:
        void s_exit(int);
        int create_listener();
        void callError(const char* message);
        void incoming_connection();
    private:
        int fd_server, epoll_fd, port;
        ThreadPool * pool;
        struct epoll_event events[EPOLL_QUEUE_LEN], event;
};

#endif // EPOLLSERVER_H
