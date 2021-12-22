#pragma once
#include "..\..\shared_src\Socket.h"
#include <cstring>
#include <map>
#include <thread>
#include <mutex>
class Server
{
	char ip[16];
	unsigned short port;
	std::mutex map_mutex;
	bool threads_run = true;
	std::map<std::thread::id, std::thread> connections;
	std::thread listen;
	void ListenConnetions();
	void ProcessConnection(Socket _socket);
public:
	~Server();
	Server() = delete;
	Server(const char* ip, unsigned short port);
	
};
