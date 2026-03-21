#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include "chatterbox_client.h"
#include "chatterbox_server.h"
#include <winsock2.h>
#include <windows.h>
#include <process.h>

// structs
// pouinter wizardry
// test pass on many machine
// I <3 my GF

void main() {

	WSADATA wsaData;
	int success = WSAStartup(MAKEWORD(2, 2), &wsaData);

	if (success != 0) {
		
		perror("ERROR:");
		return -1;
	
	}

	SOCKET checkHost = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	SOCKADDR_IN	serverAddress;
	serverAddress.sin_family = AF_INET;
	inet_pton(AF_INET, "127.0.0.1", &serverAddress.sin_addr);
	serverAddress.sin_port = htons(8888);

	int check = connect(checkHost, (struct sockaddr_in*)&serverAddress, sizeof(serverAddress));

	if (check == SOCKET_ERROR) {
	
		SOCKET listenSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
		bind(listenSocket, (struct socketAddress*)&serverAddress, sizeof(serverAddress));
		listen(listenSocket, SOMAXCONN);

		_beginthread(relay, 0, (void*)&listenSocket);

		Sleep(100);
	
	}

	closesocket(checkHost);


	SOCKET clientSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	if (!clientSocket) {
		
		return -1;

	}

	connect(clientSocket, (struct sockaddr_in*)&serverAddress, sizeof(serverAddress));


	SOCKET* mallocSocket = malloc(sizeof(SOCKET));
	_beginthread(recieveMessage, 0, mallocSocket);

	while (1) {

		printf("\n > ");
		char messageBuffer[1024];
		fgets(messageBuffer, 1024, stdin);
		send(clientSocket, messageBuffer, strlen(messageBuffer), 0);

	}

	WSACleanup();

	return 0;

}