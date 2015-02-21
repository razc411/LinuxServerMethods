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
#include <mutex>
#include <fstream>
#include <sys/epoll.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <vector>
#include <cstdlib>

void start_instance(int port, std::string host, int d_size, std::ofstream * log, std::mutex * mtx);
void send_echo(int sd, int data_size, std::mutex * m_log, std::ofstream * log);
long get_elapsed_time(struct timeval start);

#endif // INSTANCE_H