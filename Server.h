#define HEADER_SIZE 4
#include <errno.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <netdb.h>
#include <strings.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <signal.h>
#include "include/ThreadPool.h"

class Server
{
public:
    virtual ~Server() {}
    virtual void monitor_connections();

protected:
    int port;
    //ThreadPool th_Pool;

};
