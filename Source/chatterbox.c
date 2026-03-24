#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include "chatterbox_client.h"
#include "chatterbox_server.h"
#include <winsock2.h>
#include <ws2tcpip.h>
#include <windows.h>
#include <process.h>

// TODO:
// 
// structs
// test pass on many machine
// I <3 my GF

int main(int argc, char *argv[]) {

	int setName = strcmp(argv[1], "setname");

	if (setName == 0  && argc > 1) {
	
		int success = SetEnvironmentVariableW((LPCWSTR)"ChatterBoxName", (LPCWSTR)argv[2]);

		if (success == 0) {
			
			perror("ERROR: ");
			return -1;
		
		}

		printf("New name successfully set: %s", argv[2]);
		return 0;
	
	}

	WSADATA wsaData;
	int success = WSAStartup(MAKEWORD(2, 2), &wsaData);

	if (success != 0) {
		
		perror("ERROR:");
		return -1;
	
	}

	SOCKADDR_IN	serverAddress;
	serverAddress.sin_family = AF_INET;
	inet_pton(AF_INET, "127.0.0.1", &serverAddress.sin_addr);
	serverAddress.sin_port = htons(8888);


	SOCKET clientSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	if (!clientSocket) {
		
		return -1;

	}

	int check = connect(clientSocket, (struct sockaddr*)&serverAddress, sizeof(serverAddress));

	if (check != 0) {

		SOCKET listenSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
		bind(listenSocket, (struct sockaddr*)&serverAddress, sizeof(serverAddress));
		listen(listenSocket, SOMAXCONN);

		_beginthread(relay, 0, &listenSocket);

		connect(clientSocket, (struct sockaddr*)&serverAddress, sizeof(serverAddress));
			
	}

	_beginthread(recieveMessage, 0, &clientSocket);
	system("cls");

	while (1) {

		printf("\n\n > ");

		char messageBuffer[5120];
		fgets(messageBuffer, 5120, stdin);

		if ((int)strlen(messageBuffer) > 5000) {
		
			printf("\n\nERROR: Messages are limited to 5000 characters.");
			continue;
		
		}

		int exit = strcmp(messageBuffer, "Exit\n");

		if (exit == 0) {
			
			break;
		
		}

		else {
		
			send(clientSocket, messageBuffer, (int)strlen(messageBuffer), 0);
		
		}

	}

	WSACleanup();

	return 0;

}