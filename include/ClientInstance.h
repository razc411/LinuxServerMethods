#ifndef INSTANCE_H
#define INSTANCE_H

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
#include <thread>
#include <mutex>
#include <fstream>
#include <sys/epoll.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <vector>
#include <cstdlib>

class ClientInstance : public Thread
{
	public:
		ClientInstance(int port, std::string host, int d_size, std::ofstream log, std::mutex mtx);
		void run();
		void send_echo();
		long get_elapsed_time(struct timeval start);

	private:
		std::ofstream logfile;
		std::mutex m_log;
		int sd, sport, data_size;
		std::string hostname;
};

#endif // INSTANCE_H