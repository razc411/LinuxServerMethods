class Server
{
public:
    virtual ~Server() {}

    virtual void monitor_connections();
    virtual int incoming_data();
    virtual int send_echo();
    virtual int s_exit();

    void set_port(int port)
    {
        this.port = port;
    }

    void set_pool(ThreadPool th_Pool)
    {
        this.th_Pool = th_Pool
    }
private:
    int port;
    ThreadPool th_Pool;
};
