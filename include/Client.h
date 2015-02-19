#ifndef CLIENT_H
#define CLIENT_H

#include <string>
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <strings.h>
#include <vector>
#include <unistd.h>
#include <cstring>

class Client
{
    public:
        Client(int, std::string, int, int); 
        ~Client() {};
        void connect_clients();
        void start_clients();

    private:

        std::string hostname;
        std::vector client_list;
};

#endif // CLIENT_H
