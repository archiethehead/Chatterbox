#include <winsock2.h>
#include <ws2tcpip.h>
#include <windows.h>
#include <process.h>

#define MAX_CLIENTS 10

int clientCount = 0;
SOCKET clientList[MAX_CLIENTS];

CRITICAL_SECTION clientCriticalSection;

void clientListener(SOCKET *socketPtr) {

	SOCKET clientSocket = *socketPtr;
	char messageBuffer[5120];

	while (1) {
	
		int recievedBytes = recv(clientSocket, messageBuffer, sizeof(messageBuffer), 0);

		if (recievedBytes > 0) {

			EnterCriticalSection(&clientCriticalSection);

			for (int i = 0; i < clientCount; i++) {
			
				if (clientList[i] != clientSocket) {
				
					send(clientList[i], messageBuffer, recievedBytes, 0);
				
				}
			
			}

			LeaveCriticalSection(&clientCriticalSection);
		
		}

		else {
		
			closesocket(clientSocket);
			return;
		
		}
	
	}

}

void relay(SOCKET* listenSocketPtr) {

	InitializeCriticalSection(&clientCriticalSection);
	SOCKET listenSocket = *listenSocketPtr;

	while (1) {
	
		SOCKADDR_IN clientAddress;

		clientAddress.sin_family = AF_INET;
		inet_pton(AF_INET, "127.0.0.1", &clientAddress.sin_addr);
		clientAddress.sin_port = htons(8888);

		int addressLength = sizeof(clientAddress);

		SOCKET newClientSocket = accept(listenSocket, (struct sockaddr*)&clientAddress, &addressLength);
		
		if (newClientSocket != INVALID_SOCKET) {

			if (clientCount < MAX_CLIENTS) {
				
				EnterCriticalSection(&clientCriticalSection);

				clientList[clientCount] = newClientSocket;
				clientCount += 1;
				printf("\n\nSERVER - New client connected! Client Count: %d\n\n > ", clientCount);

				_beginthread(clientListener, 0, &newClientSocket);

				LeaveCriticalSection(&clientCriticalSection);
			
			}

			else {
			
				closesocket(newClientSocket);
				DeleteCriticalSection(&clientCriticalSection);
			
			}
		
		}

	}

}