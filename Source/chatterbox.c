
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#define BLUE(string) "\x1b[34m"string "\x1b[0m" 

#include "chatterbox_client.h"
#include "chatterbox_server.h"
#include <winsock2.h>
#include <ws2tcpip.h>
#include <windows.h>
#include <process.h>

// TODO:
// 
// structs
// UDP broadcasting
// test pass on many machine

int main(int argc, char *argv[]) {

	if (argc > 1) {

		int setName = strcmp(argv[1], "setname");
		
		if (setName == 0 && argc > 2) {


			int success = SetEnvironmentVariableA("ChatterBoxName", argv[2]);

			if (success == 0) {

				perror("ERROR: ");
				return -1;

			}

			printf("\nNew name successfully set: %s\n", argv[2]);
			return 0;

		}

		int help = strcmp(argv[1], "help");

		if (help == 0) {
			
			printf("\nWelcome to Chatterbox!\n\n\
Because Chatterbox is added to your system path as per the installer specifications, Chatterbox is very\n\
easy to use. Simply open your system terminal, and type Chatterbox to open the messaging on your local\n\
network. Messages stay between you, and whoever else on your local network is logged on.\n\n\
Commands:\nsetname - type the identity you wish to use as the argument for this.\n\
colour - type the name of the colour you wish to use for your name. If it is supported, it will save\n\
help - outputs instructions on how to use Chatterbox.\n");
			return 0;
		
		}

		else {

			printf("\n'%s' is not a recognised Chatterbox command - try 'Chatterbox help'\n", argv[1]);
			return 0;

		}

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