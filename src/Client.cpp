#include "../include/Client.h"

void Client::connect_client()
{
    struct hostent	*hp;
	struct sockaddr_in server;

	if ((sd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
	{
		perror("Cannot create socket");
		exit(1);
	}
	bzero((char *)&server, sizeof(struct sockaddr_in));
	server.sin_family = AF_INET;
	server.sin_port = htons(port);
	if ((hp = gethostbyname(hostname.c_str())) == NULL)
	{
		fprintf(stderr, "Unknown server address\n");
		exit(1);
	}
	bcopy(hp->h_addr, (char *)&server.sin_addr, hp->h_length);

	if (connect(sd, (struct sockaddr *)&server, sizeof(server)) == -1)
	{
		fprintf(stderr, "Can't connect to server\n");
		perror("connect");
		exit(1);
	}
	printf("Connected:    Server Name: %s\n", hp->h_name);
    while(true){
        send_echo();
    }
}

void Client::send_echo()
{
    int n, bytes_to_read, msg_size;
    std::string input = "ECHO REPLY ECHO REPLY KEEP DOING IT.";
    char * bp;

    msg_size = input.length();

    char rbuf[msg_size];

    printf("Sending Echo...");
    send (sd, input.c_str(), msg_size, 0);

	printf("Receive:\n");
	bp = rbuf;
	bytes_to_read = msg_size;

	n = 0;
	while ((n = recv (sd, bp, bytes_to_read, 0)) < msg_size)
	{
		bp += n;
		bytes_to_read -= n;
	}

	printf ("%s\n", rbuf);
}
