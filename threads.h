#include "WsWrapper.h"

DWORD WINAPI WsWrapper::__t__createServerRes(LPVOID lpParams) {
    int i_result;
    char c_rec[DEFAULT_BUFF];
    WsWrapper::s_server_res* Socket_Response = (WsWrapper::s_server_res*)lpParams;
    do {
        i_result = recv(Socket_Response->SocketResponse, c_rec, DEFAULT_BUFF, 0);
		if (i_result > 0) {
			if (i_result < DEFAULT_BUFF - 1)
				c_rec[i_result] = '\0';

			if(Socket_Response->fpResData != NULL)
				Socket_Response->fpResData( Socket_Response->SocketResponse, Socket_Response->addrRes, c_rec);
		}
    } while(i_result > 0);
    return 0;
}
