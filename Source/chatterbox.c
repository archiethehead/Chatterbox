#include "chatterbox_client.h"
#include "chatterbox_server.h"
#include <winsock2.h>
#include <windows.h>
#include <process.h>

CRITICAL_SECTION clientCriticalSection;

void main() {
	
	InitializeCriticalSection(&clientCriticalSection);

	SOCKET clientSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	if (!clientSocket) {
		
		return -1;

	}

	_beginthread(recieveMessage, 0, (void*)clientSocket);

	while (1) {

		char messageBuffer[1024];
		fgets(messageBuffer, 1024, stdin);
		send(clientSocket, messageBuffer, strlen(messageBuffer), 0);

	}


	return 0;

}