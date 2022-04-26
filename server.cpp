#include "WsWrapper.h"
#include <iostream>

bool WsWrapper::Server::init(PCSTR port, int protocol, OnServerClientAcceptCallback clientAccepted) {

	//init server
	WSAData wsa;
	struct sockaddr_in server;
	int i_result;
	sParams = new server_params;
	server.sin_family = AF_INET;
	server.sin_addr.s_addr = INADDR_ANY;
	server.sin_port = htons(std::stoi(port));

	i_result = WSAStartup(MAKEWORD(2, 2), &wsa);
	if (i_result != 0) {
		std::cout << "WSA error";
		return false;
	}

	Socket_Listen = socket(AF_INET, SOCK_STREAM, 0);
	if (Socket_Listen == INVALID_SOCKET) {
		WSACleanup();
		std::cout << "invalid socket";
		return false;
	}

	if (bind(Socket_Listen, (struct sockaddr*)&server, sizeof(server)) == SOCKET_ERROR) {
		WSACleanup();
		std::cout << "binding error";
		return false;
	}

	//write data to send to thread
	sParams->Socket_Listen = this->Socket_Listen; //socket
	sParams->clientAccepted = clientAccepted; //callback
	//start thread
	CreateThread(0, 0, (LPTHREAD_START_ROUTINE)t_start_listening, sParams, 0, 0);
	return true;

}

//listening thread
void WsWrapper::t_start_listening(LPVOID lpParams) {
	//get params
	struct server_params* sParams = (server_params*)lpParams;
	SOCKET Socket_Listen = sParams->Socket_Listen;
	SOCKET Socket_Client;
	struct sockaddr_in client;
	int i_result;
	OnServerClientAcceptCallback callback = sParams->clientAccepted;
	int c = sizeof(client);

	while (true) {
		i_result = listen(Socket_Listen, SOMAXCONN);
		if (i_result == SOCKET_ERROR) {
			std::cout << "listen error";
			break;
		}
		
		
		Socket_Client = accept(Socket_Listen, (sockaddr*)&client, &c);
		if (Socket_Client == INVALID_SOCKET) {
			std::cout << "accept error";
			break;
		}

		callback(Socket_Client, &client);
	}
}

bool WsWrapper::Server::send_data(SOCKET clientSocket, const char* sendChar) {
	if (send(clientSocket, sendChar, (int)strlen(sendChar), 0) == SOCKET_ERROR) {
		closesocket(clientSocket);
		WSACleanup();
		return false;
	}
	return true;
}

