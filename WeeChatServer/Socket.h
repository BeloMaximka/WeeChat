#pragma once
#include <string>
#include "WinSock2.h"
#include <ws2tcpip.h>
#pragma comment(lib, "Ws2_32.lib")

#include "Logger.h"
class Socket
{
	static const int RECV_BUFFER = 1024;
	SOCKET _socket;
public:
	Socket();
	~Socket();
	Socket(Socket&& obj);
	bool Send(const char* buff, size_t size);
	bool Recv(char* buff, size_t size);
	bool Host(const char* ip, int port);
	bool Connect(const char* ip, int port);
};