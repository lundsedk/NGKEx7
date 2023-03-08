#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <netdb.h>
#include "iknlib.h"
#include <string>
#include <arpa/inet.h>

#define PORTN 9000

void error(const char *msg)
{
	perror(msg);
	exit(1);
}

int main(int argc, char *argv[])
{
	if (argc < 3)
	{
		error("\nERROR usage: get_measurement [server IP] [u/l]\n");
		exit(-1);
	}

	char receivedMess[256] = {0};
	int sockfd, readState; // all used?
	struct sockaddr_in serv_addr;
	socklen_t serv_addr_len;
	struct hostent *server;
	char commandChar[1] = {argv[2][0]};
	char readBuffer[256];



	sockfd = socket(AF_INET, SOCK_DGRAM, 0);
	if (sockfd < 0)
	{
		error("ERROR opening socket");
		exit(-1);
	}
	server = gethostbyname(argv[1]);
	if (server == NULL)
		error("ERROR no such host");

	//printf("\nServer at: %s, sending command: %c\n", argv[1], commandChar[0]);

	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(PORTN);
	serv_addr.sin_addr.s_addr = inet_addr("10.0.0.1");
	serv_addr_len = sizeof(serv_addr);

	if (sendto(sockfd, commandChar, 1, 0, (struct sockaddr *)&serv_addr, serv_addr_len) < 0)
	{
		error("\nError sending command ");
		close(sockfd);
		exit(-1);
	}

	readState = recvfrom(sockfd, readBuffer, 255, 0, (struct sockaddr*) &serv_addr, &serv_addr_len);
	if (readState < 0) { 
		error("\nRead error");
		close(sockfd);
		exit(-1);
	}
	printf("Server answered: %s", readBuffer);

	close(sockfd);
	return 0;
}
