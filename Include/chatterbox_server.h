#include <winsock2.h>
#include <windows.h>
#include <process.h>

int MAX_CLIENTS = 10;
int clientCount = 0;

SOCKET clientList[10];

void clientListener(void *socketPtr) {

	SOCKET clientSocket = *(SOCKET*)socketPtr;
	char messageBuffer[1024];

	while (1) {
	
		int recievedBytes = recv(clientSocket, messageBuffer, sizeof(messageBuffer), 0);

		if (recievedBytes > 0) {

			for (int i = 0; i < clientCount; i++) {
			
				if (clientList[i] != clientSocket) {
				
					send(clientList[i], messageBuffer, recievedBytes, 0);
				
				}
			
			}
		
		}

		else {
		
			closesocket(clientSocket);
			return;
		
		}
	
	}

}

void relay(void *listenSocketPtr) {

	SOCKET listenSocket = *(SOCKET*)listenSocketPtr;

	while (1) {
	
		SOCKADDR_IN clientAddress;
		int addressLength = sizeof(clientAddress);

		SOCKET newClientSocket = accept(listenSocket, (struct socketAddress*)&clientAddress, &addressLength);
		
		if (newClientSocket != INVALID_SOCKET) {

			if (clientCount < MAX_CLIENTS) {
			
				clientList[clientCount] = newClientSocket;
				clientCount++;
				_beginthread(clientListener, 0, (void*)&newClientSocket);
				printf("SERVER - New client connected, client count: %d", clientCount);
			
			}

			else {
			
				closesocket(newClientSocket);
			
			}
		
		}

	}

}