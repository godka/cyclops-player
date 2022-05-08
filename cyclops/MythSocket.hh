#pragma once
#include "curl/curl.h"
#include "SDL.h"

class MythSocket
{
public:
	MythSocket();
	MythSocket(const char* ip,u_short port);
	~MythSocket(void);
	int socket_InitalSocket();
	int socket_BindAddressPort(const char* ip,u_short port);
	int socket_SendStr(const char* str,int length);
	int socket_ReceiveData(char* recvBuf,int recvLength);
	int socket_CloseSocket();
	int socket_strcmp(char* buff,char*str,int length);
	int socket_ReceiveDataLn2(char* recvBuf,int recvLength,char* lnstr);
	//int ReadSocket(char* recvBuf,int length);
	static MythSocket *CreatedNew(const char* ip,u_short port);
	int wait_on_socket(curl_socket_t sockfd, int for_recv, long timeout_ms);
protected:
	char* downbuffer;
	int downlength;
	CURL *curl;
	CURLcode res;
	long sockextr;
	curl_socket_t sockfd;

};

