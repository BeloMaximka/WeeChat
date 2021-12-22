#pragma once
#include <string>
#include "WinSock2.h"
#include <ws2tcpip.h>
#pragma comment(lib, "Ws2_32.lib")

class Socket
{
	SOCKET _socket;
public:
	static const int RECV_BUFFER = 1024;
	Socket();
	Socket(SOCKET sock);
	~Socket();
	Socket(Socket&& obj);
	bool Send(const char* buff, size_t size);
	bool Send(const wchar_t* msg, int size = -1);
	bool Recv(char* buff, size_t size);
	bool Recv(std::wstring& str);
	bool Bind(const char* ip, u_short port);
	bool Accept();
	Socket MakeAccept();
	bool Connect(const char* ip, u_short port);
	bool CloseConnection();
};