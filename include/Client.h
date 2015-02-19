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
#include <vector>
#include <unistd.h>
#include <iostream>
#include <cstring>

#define BUFFER_SIZE 2048

class Client
{
    public:
        Client(int, std::string, int, int); 
        ~Client() {};
        void connect_clients();
        void send_echo();

    private:

        std::string hostname;
        std::vector client_list;
};

#endif // CLIENT_H
