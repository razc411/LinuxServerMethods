/**
*	Program:	Server Comparison System
*	Author: 	Ramzi Chennafi
*	Date: 		Febuary 10 2015
*	Functions:
*		Client::Client(int port, std::string hostname, int num_clients, int data_size) : clients(num_clients), dsize(data_size)
*       void Client::connect_clients(int port, std::string hostname)
*
*	Description
*	This cpp file represents the client side of the comparison program. Establishes a number of clients and makes them send data.
*/
#include "../include/Client.h"
/**
*	Function: 	Client
*	Author: 	Ramzi Chennafi
*	Date:		Febuary 10 2015
*	Returns:	void
*
*	Notes
*	Constructor for the client class. Establishes the connected clients and sends the echos.
*		port - port for the client to send to
*		hostname - host of the server to send to
*		num_clients - number of clients to create
*		data_sie - amount of data for each client to send, in bytes
*/
Client::Client(int port, std::string hostname, int num_clients, int data_size) : clients(num_clients), dsize(data_size)
{
	connect_clients(port, hostname);
}
/**
*	Function: 	connect_clients(int port, std::string hostname)
*	Author: 	Ramzi Chennafi
*	Date:		Febuary 10 2015
*	Returns:	void
*
*	Notes
*	Connects the clients to the server, waits on user input and then sends the echos to the server. Logs all data 
*	for the transfers to client_log.
*		port - port on the server to send data to
*		hostname - host to connect the clients to
*/
void Client::connect_clients(int port, std::string hostname)
{
	std::ofstream clientlog;
	clientlog.open("client_log");
	int client_list[clients];

	for(int i = 0; i < clients; i++)
	{
		client_list[i] = start_instance(port, hostname);
	}

	std::cout << clients << " clients created connected to " << hostname << "sending " << dsize << " bytes per second.\n";
	clientlog << clients << " clients created connected to " << hostname << "sending " << dsize << " bytes per second." << std::endl;
	std::cout << "Hit any key to begin transfer." << std::endl;
	std::cin.get();

	while(true)
	{
		for(int i = 0; i < clients; i++)
		{
			send_echo(client_list[i], dsize, &clientlog);
		}

	}
}


