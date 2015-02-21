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
        void connect_clients(int port, std::string hostname, int data_size);
        void start_clients();

    private:
    	int clients;
        std::string hostname;
        std::vector<std::thread *> client_list;
};

#endif // CLIENT_H
