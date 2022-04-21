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

bool WsWrapper::Client::send_data(const char* sendChar) {
	bool b_result;

	if (send(Socket_Client, sendChar, (int)strlen(sendChar), 0) == SOCKET_ERROR) {
		closesocket(Socket_Client);
		WSACleanup();
		return false;
	}
	return true;
}

void WsWrapper::Client::listen(OnDataReceived dataReceived) {
	int i_result;
	TCHAR c_rec[DEFAULT_BUFF];
}