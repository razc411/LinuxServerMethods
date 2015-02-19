#include "../include/Client.h"

Client::Client(int port, std::string hostname, int clients, int dsize) : clients(num_clients)
{
	connect_clients(port, hostname, data_size);
}

void Client::start_clients()
{ 
    while(true)
    {
        for(int i = 0; i < clients; ++i)
        {
        	client_list[i].send_echo();
        }
    }
}

void Client::connect_clients(int port, std::string hostname, int data_size)
{
	for(int i = 0; i < clients; ++i)
	{
		Instance client = new Instance(port, hostname, data_size);
		client_list.push_back(client);
	}

	printf("%d clients created, connected to %s and ready to send a  %d bytes of data per second.", clients, hostname, data_size);
}
