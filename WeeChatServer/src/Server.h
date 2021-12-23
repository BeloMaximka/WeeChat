#pragma once
#include "..\..\shared_src\Socket.h"
#include <cstring>
#include <map>
#include <list>
#include <thread>
#include <mutex>
#include <iostream>

class Server
{
	char ip[16];
	unsigned short port;

	std::mutex map_mutex;
	bool threads_run = true;
	std::thread listen;

	std::map <std::thread::id, std::pair<std::thread, Socket*>> connections;
	std::list <std::wstring> chatHistory;
	
	void ListenConnetions();
	void ProcessConnection(Socket _socket);
	std::wstring WSAerror_to_wstring(int code);
public:
	~Server();
	Server() = delete;
	Server(const char* ip, unsigned short port);
	
};