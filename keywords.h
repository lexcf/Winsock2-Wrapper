#pragma once

typedef void(*OnClientConnectCallback)();
typedef void(*OnDataReceivedCallback)(char*);

#define DEFAULT_BUFF 1024
