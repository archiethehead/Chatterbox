#include <winsock2.h>
#include <windows.h>
#include <process.h>

#define MAX_CLIENTS 10

int clientCount = 0;
SOCKET clientList[MAX_CLIENTS];

void client_handler(void *socketPtr) {

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