#include "chatterbox_client.h"
#include "chatterbox_server.h"
#include <winsock2.h>
#include <windows.h>
#include <process.h>

void main() {

	
	SOCKET clientSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	if (!clientSocket) {
		
		return -1;

	}

	_beginthread(recieveMessage, 0, (void*)clientSocket);


	return 0;

}