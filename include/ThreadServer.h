#ifndef THREADSERVER_H
#define THREADSERVER_H

#include <../Server.h>


class ThreadServer : public Server
{
    public:
        ThreadServer(int s_port);
        virtual ~ThreadServer();
    protected:
    private:
};

#endif // THREADSERVER_H
