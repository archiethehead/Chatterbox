#include <winsock2.h>
#include <windows.h>
#include <stdio.h>

void recieveMessage(SOCKET* newSocket) {

	SOCKET clientSocket = *newSocket;

	char messageBuffer[5120];

	while (1) {

		int recievedBytes = recv(clientSocket, messageBuffer, sizeof(messageBuffer), 0);

		if (recievedBytes < 5120) {
		
			messageBuffer[recievedBytes] = 0x0;
		
		}

		if (recievedBytes > 0) {

			printf("\n\n%s\n\n > ", messageBuffer);
			fflush(stdout);

		}

	}

}