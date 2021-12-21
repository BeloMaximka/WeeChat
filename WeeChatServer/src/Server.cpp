#include "Server.h"

Server::Server(const char* ip, unsigned short port)
{
	strcpy_s(this->ip, ip);
	this->port = port;
}

void Server::ListenConnetions()
{
	Socket base_socket;
	base_socket.Bind(ip, port);
	while (true)
	{

	}
}
