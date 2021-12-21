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
	Socket() = default;
	Socket(SOCKET sock);
	~Socket();
	Socket(Socket&& obj);
	bool Send(const char* buff, size_t size);
	bool Recv(char* buff, size_t size);
	bool Bind(const char* ip, int port);
	bool Accept();
	Socket MakeAccept();
	bool Connect(const char* ip, int port);
};