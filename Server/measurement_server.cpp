
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include "iknlib.h"

#define PORTN		9000

void error(const char *msg)
{
	perror(msg);
	exit(1);
}



int main(int argc, char *argv[]) {
	// Local vars
	sockaddr_in serverAddress;
	sockaddr_in clientAddress;
	socklen_t clientAddressLength;
	int connectedSocket;
	int inSocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	char readChar[0];
	char writeBuffer[256];
	int readState;

	printf("Starting server...\n");


	if(inSocket<0){ error("ERROR opening socket");}						//check initialization of global var

	// Set up binding of socket
	serverAddress.sin_family = AF_INET; //IPv4
	serverAddress.sin_addr.s_addr = INADDR_ANY; //accept any message
	serverAddress.sin_port = htons(PORTN); //converts portnumber to network byte order

	// Bind
	if (bind(inSocket, (struct sockaddr *) &serverAddress, sizeof(serverAddress)) < 0) 
		{error("ERROR on binding");}



	printf("Listening for connection...\n");

	listen(inSocket,2); //max 2 pending connections. ready to accept incoming connections
	clientAddressLength = sizeof(clientAddress);
		//where are we setting client-address? cannot really send without it...



				
  	while(1) {

		readState = recvfrom(inSocket, readChar, 1, 0, (struct sockaddr*) &clientAddress, &clientAddressLength);
		if (readState < 0) { 
			error("\nRead error");
			close(inSocket);
			exit(-1);
		}

		switch (readChar[0])
		{
		case 'u':
		case 'U':
			printf("\nReceived %c - sending uptime", readChar[0]);
			fflush(stdout);
			break;
		case 'l':
		case 'L':
			printf("\nReceived %c - sending loadavg", readChar[0]);
			fflush(stdout);
			break;
		
		default:
			printf("\nReceived %c - command not recognized", readChar[0]);
			fflush(stdout);

					//test send, debugging - now test receive...
						strncpy(writeBuffer, (char*) "test1", 6);		//working
						if (sendto(inSocket, writeBuffer, 255, 0, (struct sockaddr *)&clientAddress, clientAddressLength) < 0)
						{
							error("\nError sending command ");
							close(inSocket);
							exit(-1);
						}

			break;
		}

	}
	close(inSocket);




	return 0; 
}