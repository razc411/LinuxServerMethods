#ifndef INSTANCE_H
#define INSTANCE_H

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
#include <thread>

#define BUFFER_SIZE 2048

class Instance extends Thread
{
public:
		Instance::Instance(int sport, std::string hostname, int d_size) 
			: port(sport), hostname(host), data_size(d_size) {};
		void run();
		void send_echo();

	private:
		int sport;
		std::string hostname;
		int data_size;
};

#endif // INSTANCE_H