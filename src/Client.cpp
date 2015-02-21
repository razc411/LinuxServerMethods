#include "../include/Client.h"
/**
*	Function: 	monitor_connections
*	Author: 	Ramzi Chennafi
*	Date:		Febuary 10 2015
*	Returns:	void
*
*	Notes
*	Core monitoring function of the epoll server. Sets the server up and checks the epoll events for socket events
*   and responds accordingly. Manages listening, reading and writing.
*/
Client::Client(int port, std::string hostname, int num_clients, int data_size) : clients(num_clients)
{
	connect_clients(port, hostname, data_size);
}
/**
*	Function: 	monitor_connections
*	Author: 	Ramzi Chennafi
*	Date:		Febuary 10 2015
*	Returns:	void
*
*	Notes
*	Core monitoring function of the epoll server. Sets the server up and checks the epoll events for socket events
*   and responds accordingly. Manages listening, reading and writing.
*/
void Client::connect_clients(int port, std::string hostname, int data_size)
{
	std::ofstream log;
	std::mutex log_mutex;
	log.open("log.txt");
	std::thread client_list[clients];

	for(int i = 0; i < clients; ++i)
	{
		client_list[i] = std::thread(start_instance, port, hostname, data_size, &log, &log_mutex);
	}

	std::cout << clients << " clients created connected to " << hostname << "sending " << data_size << " bytes per second.\n";
}
