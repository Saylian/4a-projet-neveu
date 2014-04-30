#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <string.h>

#define MAX_SIZE 50

int main()

{
	int sock_descriptor, conn_desc;

	struct sockaddr_in serv_addr, client_addr;

	char buff[MAX_SIZE];
	sock_descriptor = socket(AF_INET, SOCK_STREAM, 0);
	
	if (sock_descriptor < 0)
	{
		printf("Creation socket echouer");
	}
	bzero((char *)&serv_addr, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = INADDR_ANY;
	
	serv_addr.sin_port = htons(1235);
	
	if (bind(sock_descriptor, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
	{
		printf("Bind Echec");
	}

	listen(sock_descriptor,5);
	
	printf("En attente de connection ...");
	int size = sizeof(client_addr);
	
	conn_desc = accept(sock_descriptor, (struct sockaddr *)&client_addr, &size);
	if (conn_desc == -1)
	{
		printf("Echec de connection");
	}
	else
	{
		printf("Connected \n");
	}
	
	if ( read(conn_desc, buff, sizeof(buff) -1) > 0)
	{
		printf("Received %s", buff);
	}
	else
	{
		printf("Echec de reception \n");
	}
	close(conn_desc);
	close(sock_descriptor);
	return 0;
}
