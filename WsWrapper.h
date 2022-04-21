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

	class Server
	{
	public:
		SOCKET Socket_Listen;
		SOCKET Socket_Client;

	public:
		bool start_server(PCSTR port, int protocol, FPC_CALLBACK fpConnected = 0, FPS_CALLBACK fpData = 0, bool thread = true);
		void send_data(SOCKET clientSocket, const char* sendChar);
		void close();
	};

	DWORD WINAPI			__t__listenThread(LPVOID lpParams);
	DWORD WINAPI			__t__startserver(LPVOID lpParams);
	DWORD WINAPI			__t__createServerRes(LPVOID lpParams);
	bool __createserver(WsWrapper::Server* const& server_class, PCSTR port, int protocol, FPC_CALLBACK fpConnected, FPS_CALLBACK fpData);

	struct s_server_params
	{
		PCSTR			port;
		int				protocol;
		FPC_CALLBACK	fpConnected;
		FPS_CALLBACK	fpData;
		SOCKET			socket_l;
		SOCKET			socket_c;
		WsWrapper::Server* s_class;
	};

	struct s_server_res {
		SOCKET SocketResponse;
		FPS_CALLBACK fpResData;
		struct addrinfo* addrRes;
	};
}

#undef _CRT_SECURE_NO_WARNINGS
