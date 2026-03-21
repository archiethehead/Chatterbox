#include <winsock2.h>
#include <windows.h>
#include <process.h>

int MAX_CLIENTS = 10;
int clientCount = 0;

CRITICAL_SECTION clientCriticalSection;

SOCKET clientList[10];

void clientListener(void *socketPtr) {

	SOCKET clientSocket = *(SOCKET*)socketPtr;
	char messageBuffer[1024];

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

void relay(void *listenSocketPtr) {

	InitializeCriticalSection(&clientCriticalSection);
	SOCKET listenSocket = *(SOCKET*)listenSocketPtr;

	while (1) {
	
		SOCKADDR_IN clientAddress;
		int addressLength = sizeof(clientAddress);

		SOCKET newClientSocket = accept(listenSocket, (struct socketAddress*)&clientAddress, &addressLength);
		
		if (newClientSocket != INVALID_SOCKET) {

			if (clientCount < MAX_CLIENTS) {
				
				EnterCriticalSection(&clientCriticalSection);

				clientList[clientCount] = newClientSocket;
				clientCount++;
				_beginthread(clientListener, 0, (void*)&newClientSocket);
				printf("SERVER - New client connected, client count: %d", clientCount);

				LeaveCriticalSection(&clientCriticalSection);
			
			}

			else {
			
				closesocket(newClientSocket);
				DeleteCriticalSection(&clientCriticalSection);
			
			}
		
		}

	}

}