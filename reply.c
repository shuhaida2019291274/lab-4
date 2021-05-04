#include <unistd.h>
#include <stdio.h>
#include <netdb.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#define MAX 80
#define PORT 8080
#define STAD struct sockaddr

// Function designed for chat between client and server.
void func(int sock_desc)
{
	char buff[MAX];
	int n;
	// infinite loop for chat
	for (;;) {
		bzero(buff, MAX);

		// read the message from client and copy it in buffer
		read(sock_desc, buff, sizeof(buff));
		// print buffer which contains the client contents
		printf("Client said : %s\t For client : ", buff);
		bzero(buff, MAX);
		n = 0;
		// copy server message in the buffer
		while ((buff[n++] = getchar()) != '\n');

		// and send that buffer to client
		write(sock_desc, buff, sizeof(buff));

		// if the message contains "Exit" then server exit and chat ended.
		if (strncmp("exit", buff, 4) == 0) {
			printf("Server proceeds to exit...\n");
			break;
		}
	}
}

// Driver function
int main()
{
	int sock_desc, connfd, len;
	struct sockaddr_in servaddr, client;

	// socket create and verification
	sock_desc = socket(AF_INET, SOCK_STREAM, 0);
	if (sock_desc == -1) {
		printf("Failed to create socket...\n");
		exit(0);
	}
	else
		printf("Successfully creating the socket...\n");
	bzero(&servaddr, sizeof(servaddr));

	// assign IP, PORT
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = INADDR_ANY;
	servaddr.sin_port = htons(PORT);

	// Binding newly created socket to given IP and verification
	if ((bind(sock_desc, (STAD*)&servaddr, sizeof(servaddr))) != 0) {
		printf("Failed to bind socket...\n");
		exit(0);
	}
	else
		printf("Successfully binding the socket...\n");

	// Now server is ready to listen and verification
	if ((listen(sock_desc, 5)) != 0) {
		printf("Listen failed...\n");
		exit(0);
	}
	else
		printf("Server listening…\n");
	len = sizeof(client);

	// Accept the data packet from client and verification
	connfd = accept(sock_desc, (STAD*)&client, &len);
	if (connfd < 0) {
		printf("Failed to  accept server...\n");
		exit(0);
	}
	else
		printf("Client accepted by the server...\n");

	// Function for chatting between client and server
	func(connfd);

	// After chatting close the socket
	close(sock_desc);
}
