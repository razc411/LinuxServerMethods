#ifndef EPOLLSERVER_H
#define EPOLLSERVER_H
#define BUFLEN 255
#define EPOLL_QUEUE_LEN	256
#include <../Server.h>

class EpollServer : public Server
{
    public:
        EpollServer(int s_port);
        ~EpollServer();
        void monitor_connections();
    protected:
        int incoming_data(int fd);
        void s_exit(int);
        int create_listener();
        void callError(const char* message);
        void incoming_connection(struct sockaddr_in* remote_addr);
    private:
        int fd_server, epoll_fd;
        static struct epoll_event events[EPOLL_QUEUE_LEN], event;
};

#endif // EPOLLSERVER_H
