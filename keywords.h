#pragma once

typedef void(*OnClientConnectCallback)();
typedef void(*OnDataReceivedCallback)(char*);
typedef void(*FPC_CALLBACK) (SOCKET clientSocket, addrinfo* addressInfo);
typedef void(*FPS_CALLBACK) (SOCKET clientSocket, addrinfo* addressInfo, char* data);

#define DEFAULT_BUFF 1024
