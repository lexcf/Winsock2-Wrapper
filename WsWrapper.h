#pragma once

#define WIN32_LEAN_AND_MEAN

#include				<iostream>
#include				<string>
#include				<Windows.h>
#include				<winsock2.h>
#include				<WS2tcpip.h>
#include				<stdlib.h>
#include				<stdio.h>
#include				<vector>
#include				"keywords.h"

#pragma comment(lib, "Ws2_32.lib")

#ifndef _CRT_SECURE_NO_WARNINGS
	#define _CRT_SECURE_NO_WARNINGS
#endif

namespace WsWrapper {

	class Client {
	public:
		SOCKET Socket_Client;
		bool init(PCSTR Server, PCSTR port, int protocol, OnClientConnectCallback clientConnect);
		bool send_data(const char* sendChar);
		void listen(OnDataReceivedCallback dataReceived);
		void close();
	};

}

#undef _CRT_SECURE_NO_WARNINGS
