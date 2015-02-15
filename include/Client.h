#ifndef CLIENT_H
#define CLIENT_H

#include <string>
#include <stdio.h>
#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <errno.h>
#include <stdlib.h>
#include <strings.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <iostream>

class Client
{
    public:
        Client(int c_port, std::string hostname);
        ~Client() {};
        void connect_client();
    protected:
        void send_echo();
    private:
        int port;
        int sd;
        std::string hostname;
};

#endif // CLIENT_H