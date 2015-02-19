#include ../include/Instance.h

void Instance::run()
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
}

	
void Instance::send_echo()
{
 	int n = 0, bytes_to_read, total_bytes = 0;
    char * buffer = (char*)malloc(data_size);
    char * bp;

    memset(buffer, 1, data_size);

    send (sd, buffer, data_size, 0);

    memset(buffer, 0, data_size);

	bp = buffer;
	bytes_to_read = data_size;

	while ((n = recv (sd, bp, bytes_to_read, 0)) > 0)
	{
		total_bytes += n;

		if(n == -1 && (errno == EAGAIN || errno == EWOULDBLOCK))
	    {
	        break;
	    }
	    else if (n == 0)
	    {
	    	printf("Connection closed.");
	        close(fd);
	    }
	    else
	    {
	        perror("Receive error");
	        close(fd);
	    }
	}

	if(total_bytes != data_size)
	{
		printf("Only recieved an echo of %d bytes.", total_bytes);
	}

	sleep(1);
}