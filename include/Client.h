#ifndef CLIENT_H
#define CLIENT_H

#include <iostream>
#include <string>
#include <stdio.h>
#include <unistd.h>
#include <thread>
#include <mutex>
#include <fstream>
#include <vector>
#include <cstdlib>

#include "ClientInstance.h"

class Client
{
    public:
        Client(int, std::string, int, int); 
        ~Client() {}
        void connect_clients(int port, std::string hostname);
        void start_clients();

    private:
    	int clients;
    	int dsize;
        std::string hostname;
};

#endif // CLIENT_H
