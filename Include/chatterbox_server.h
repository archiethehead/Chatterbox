#include <winsock2.h>
#include <windows.h>

#define MAX_CLIENTS 5

int clientCount = 0;
SOCKET clients[MAX_CLIENTS];