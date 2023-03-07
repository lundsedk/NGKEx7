#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <netdb.h> 
#include "iknlib.h"
#include <string>

#define PORTN		9000

void error(const char *msg)
{
	perror(msg);
	exit(1);
}

int main(int argc, char *argv[])
{
	if (argc < 3){
	    error("\nERROR usage: get_measurement [server IP] [u/l]\n");
		exit(-1);
	}
	printf("Starting client...\n");

	char receivedMess[256] = {0};

	int sockfd, n;					//all used?
	struct sockaddr_in serv_addr;
	socklen_t serv_addr_len;
	struct hostent *server;
	char commandChar[1] = {argv[2][0]};
	char buffer[256];

	sockfd = socket(AF_INET, SOCK_DGRAM, 0);
	if (sockfd < 0) {
	    error("ERROR opening socket");
		exit(-1);
	}
	server = gethostbyname(argv[1]);
	if (server == NULL) 
	    error("ERROR no such host");

	printf("\nServer at: %s, sending command: %c\n",argv[1], commandChar[0]);



	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(PORTN);
	bcopy((char *)server->h_addr, (char *)&serv_addr.sin_addr.s_addr, server->h_length);
	if (connect(sockfd,(struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0) 
		error("ERROR connecting");

	//does the above bind correct, change something for UDP?


//cliSockDes = sockfd
	//temp, remove...

  if (sendto(sockfd, commandChar, strlen(commandChar), 0, (struct sockaddr*)&serv_addr, serv_addr_len) < 0) {
    error("Error sending command\n");
    close(sockfd);
    exit(-1);
  }

//not done below, needs to receive into buffer array, then print it
//something like:
/*
		serAddrLen = sizeof(serAddr);
		readStatus = recvfrom(cliSockDes, buff, 1024, 0, (struct sockaddr*)&serAddr, &serAddrLen);
		if (readStatus < 0) {
		perror("reading error...\n");
		close(cliSockDes);
		exit(-1);
		}

		cout.write(buff, readStatus);
		cout << endl;

*/

    printf("Closing client...\n\n");
	close(sockfd);
	return 0;
}






