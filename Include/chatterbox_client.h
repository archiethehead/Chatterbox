#include <winsock2.h>
#include <windows.h>
#include <stdio.h>

void recieveMessage(void* socketPtr) {

	SOCKET clientSocket = *(SOCKET*)socketPtr;
	char messageBuffer[1024];

	while (1) {

		int recievedBytes = recv(clientSocket, messageBuffer, sizeof(messageBuffer), 0);
		if (recievedBytes > 0) {

			printf("\n%s\n", messageBuffer);
			printf(" > ");
			fflush(stdout);

		}

	}

}