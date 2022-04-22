#pragma once

typedef void(*OnClientConnectCallback)();
typedef void(*OnDataReceivedCallback)(char*);
typedef void(*OnServerClientAcceptCallback) (SOCKET clientSocket, struct sockaddr_in* clientAddr);

#define DEFAULT_BUFF 1024
#define MAX_CONN 64
