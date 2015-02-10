#ifndef SELECTSERVER_H
#define SELECTSERVER_H

#include <../Server.h>


class SelectServer : public Server
{
    public:
        SelectServer(int s_port);
        ~SelectServer();
    protected:
    private:
};

#endif // SELECTSERVER_H
