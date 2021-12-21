#pragma once
#include "Socket.h"
#include <cstring>
class Server
{
	char ip[16];
	unsigned short port;
public:
	Server() = delete;
	Server(const char* ip, unsigned short port);
	void ListenConnetions();
};

