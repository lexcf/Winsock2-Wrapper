#pragma once

typedef void(*OnClientConnectCallback)();
typedef void(*OnDataReceived)(char*);

#define DEFAULT_BUFF 1024
