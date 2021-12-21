#include "Server.h"

Server::Server(const char* ip, unsigned short port)
{
	WSADATA wsaData;
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != NO_ERROR)
	{
		logger.format(WSAGetLastError());
		WSACleanup();
		exit(10);
	}

	strcpy_s(this->ip, ip);
	this->port = port;
	listen = std::thread(&Server::ListenConnetions, this);
}

void Server::ListenConnetions()
{
	Socket base_socket;
	logger << "Hosting server by IP " << ip << " and port " << std::to_string(port) << "...\n";
	base_socket.Bind(ip, port);
	logger << "Binded.\n";
	while (threads_run)
	{
		Socket accepted = base_socket.MakeAccept();
		logger << "Got connection.\n";
		std::thread connection(&Server::ProcessConnection, this, std::move(accepted));
		connections[connection.get_id()] = std::move(connection);
	}
}

void Server::ProcessConnection(Socket _socket)
{
	std::wstring message;
	while (threads_run && _socket.Recv(message))
	{
		_socket.Send(message.c_str());
	}
	map_mutex.lock();
	auto id = std::this_thread::get_id();
	connections[id].detach();
	connections.erase(id);
	map_mutex.unlock();
}

Server::~Server()
{
	threads_run = false;
	//listen.detach();
	//while (connections.size());
}
