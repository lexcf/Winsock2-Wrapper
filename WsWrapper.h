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
		bool init(PCSTR, PCSTR, int, OnClientConnectCallback);
		bool send_data(const char*);
		void listen(OnDataReceived);
		void close();
	};

}

#undef _CRT_SECURE_NO_WARNINGS