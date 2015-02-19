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
		Instance(int port, std::string hostname, int data_size);
		void run();

	private:
		int sd;
		int data_size;

		void send_echo();
		int exit_instance();
};

#endif // INSTANCE_H