#include <winsock2.h>
#include <windows.h>
#include <stdio.h>

void recieveMessage(SOCKET clientSocket) {

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