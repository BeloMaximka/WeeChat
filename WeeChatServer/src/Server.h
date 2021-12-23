#pragma once
#include "..\..\shared_src\Socket.h"
#include <cstring>
#include <map>
#include <list>
#include <thread>
#include <mutex>
#include <iostream>
#include <sstream>

struct ConnectionInfo
{
	std::thread connection;
	Socket* socket;
};
struct ClientInfo
{
	std::wstring name;
	ULONG color;
};

class Server
{
	char ip[16];
	unsigned short port;

	std::mutex map_mutex;
	std::thread listen;

	std::map <std::thread::id, std::pair<ConnectionInfo, ClientInfo>> connections;
	std::list <std::wstring> chatHistory;
	
	void ListenConnetions();
	void ProcessConnection(Socket _socket);
	std::wstring WSAerror_to_wstring(int code);
	void SendFormatted(ClientInfo info, std::wstring message);
	void SendServerMessageFormatted(ClientInfo info, std::wstring message);
public:
	~Server();
	Server() = delete;
	Server(const char* ip, unsigned short port);
	
};