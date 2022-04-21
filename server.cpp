#include "WsWrapper.h"

bool WsWrapper::__createserver(WsWrapper::Server* const& server_class, PCSTR port, int protocol, FPC_CALLBACK fpConnected, FPS_CALLBACK fpData) {
	WSADATA				wsaData;
	SOCKET				Socket_Listen = server_class->Socket_Listen;
	SOCKET				Socket_Client = server_class->Socket_Client;

	int					i_result;
	int					i_opt = TRUE;

	struct				addrinfo* res = NULL;
	struct              addrinfo hints;

	i_result = WSAStartup(MAKEWORD(2, 2), &wsaData);

	if (i_result != 0) return false;

	ZeroMemory(&hints, sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = protocol;
	hints.ai_flags = AI_PASSIVE;

	i_result = getaddrinfo(NULL, (PCSTR)port, &hints, &res);
	if (i_result != 0) return false;

	Socket_Listen = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
	if (Socket_Listen == INVALID_SOCKET) { freeaddrinfo(res); closesocket(Socket_Client); WSACleanup(); return false; }

	if (setsockopt(Socket_Listen, SOL_SOCKET, (SO_REUSEADDR), (char*)&i_opt, sizeof i_opt) < 0) {} //if error 


	i_result = bind(Socket_Listen, res->ai_addr, (int)res->ai_addrlen);
	if (i_result == SOCKET_ERROR) { freeaddrinfo(res); closesocket(Socket_Listen); WSACleanup(); return false; }

	freeaddrinfo(res);
	server_class->Socket_Listen = Socket_Listen;

	while (1) {
		i_result = listen(Socket_Listen, SOMAXCONN);
		if (i_result == SOCKET_ERROR)
			break;

		Socket_Client = accept(Socket_Listen, NULL, NULL);
		if (Socket_Client == INVALID_SOCKET)
			break;

		WsWrapper::s_server_res* s_params = new WsWrapper::s_server_res;
		s_params->addrRes = res;
		fpData != NULL ? s_params->fpResData = fpData : s_params->fpResData = NULL;
		s_params->SocketResponse = Socket_Client;

		CreateThread(0, 0, WsWrapper::__t__createServerRes, s_params, 0, 0);

		if (fpConnected != NULL)
			fpConnected(Socket_Client, res);
	}
	closesocket(Socket_Listen);
	WSACleanup();
	return true;
}

bool WsWrapper::Server::start_server(PCSTR port, int protocol, FPC_CALLBACK fpConnected, FPS_CALLBACK fpData, bool thread) {
	if (!thread) return WsWrapper::__createserver(this, port, protocol, fpConnected, fpData);

	s_server_params* server_data = new s_server_params;
	server_data->port = port;
	server_data->protocol = protocol;
	server_data->fpConnected = fpConnected;
	server_data->fpData = fpData;
	server_data->socket_l = Socket_Listen;
	server_data->socket_c = Socket_Client;
	server_data->s_class = this;

	CreateThread(0, 0, WsWrapper::__t__startserver, server_data, 0, 0);
	return true;
}

