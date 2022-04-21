#include "WsWrapper.h"

//init WSA and connect
bool WsWrapper::Client::init(PCSTR Server, PCSTR Port, int protocol, OnClientConnectCallback clientConnect) {
	WSADATA wsa;
	SOCKET Socket_Listen = INVALID_SOCKET;
	int i_result;

	struct addrinfo* res = NULL;
	struct addrinfo* ptr = NULL;
	struct addrinfo hints;

	i_result = WSAStartup(MAKEWORD(2, 2), &wsa);

	if (i_result != 0) return false;

	ZeroMemory(&hints, sizeof(hints));
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = protocol;

	i_result = getaddrinfo(Server, Port, &hints, &res);
	if (i_result != 0) return false;

	for (ptr = res; ptr != NULL; ptr = ptr->ai_next) {
		Socket_Client = socket(ptr->ai_family, ptr->ai_socktype, ptr->ai_protocol);
		if (Socket_Client == INVALID_SOCKET) {
			freeaddrinfo(res);
			closesocket(Socket_Client);
			WSACleanup();
			return false;
		}
		i_result = connect(Socket_Client, ptr->ai_addr, (int)ptr->ai_addrlen);
		if (i_result == SOCKET_ERROR) {
			freeaddrinfo(res);
			closesocket(Socket_Client);
			WSACleanup();
			Socket_Client = INVALID_SOCKET;
			continue;
		}
		break;
	}

	if (Socket_Client == INVALID_SOCKET) {
		//freeaddrinfo(res);
		closesocket(Socket_Client);
		WSACleanup();
		return false;
	}

	freeaddrinfo(res);
	
	clientConnect();
	return true;
}


//send message
bool WsWrapper::Client::send_data(const char* sendChar) {
	bool b_result;

	if (send(Socket_Client, sendChar, (int)strlen(sendChar), 0) == SOCKET_ERROR) {
		closesocket(Socket_Client);
		WSACleanup();
		return false;
	}
	return true;
}


//listen and call callback-function
void WsWrapper::Client::listen(OnDataReceived dataReceived) {
	int i_result;
	char c_rec[DEFAULT_BUFF];

	//receive after connection established
	do {
		memset(c_rec, 0, DEFAULT_BUFF);
		i_result = recv(Socket_Client, c_rec, DEFAULT_BUFF, 0);
		if (i_result > 0) {
			dataReceived(c_rec);
		}
	} while (i_result > 0);
	//close socket if connection refused
	closesocket(Socket_Client);
	WSACleanup();
}

//close socket
void WsWrapper::Client::close() {
	if (shutdown(Socket_Client, SD_BOTH) == SOCKET_ERROR) {
		closesocket(Socket_Client);
		WSACleanup();
	}
}
