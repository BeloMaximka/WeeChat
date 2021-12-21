#include "Socket.h"

Socket::Socket()
{
	_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (_socket == INVALID_SOCKET)
	{
		logger.format(WSAGetLastError());
		WSACleanup();
		exit(11);
	}
}

Socket::Socket(SOCKET sock)
{
	_socket = sock;
}

Socket::~Socket()
{
	closesocket(_socket);
}

Socket::Socket(Socket&& obj)
{
	closesocket(_socket);
	_socket = obj._socket;
	obj._socket = NULL;
}

bool Socket::Send(const char* buff, size_t size)
{
	if (send(_socket, buff, size, 0) == SOCKET_ERROR)
	{
		logger.format(WSAGetLastError());
		return false;
	}
	return true;
}

bool Socket::Recv(char* buff, size_t size)
{
	int bytesRecieved = recv(_socket, buff, size * sizeof(WCHAR), 0);
	if (bytesRecieved == SOCKET_ERROR)
	{
		logger.format(WSAGetLastError());
		return false;
	}
	return true;
}

bool Socket::Recv(char* buff)
{
	return Recv(buff, RECV_BUFFER);
}

bool Socket::Bind(const char* ip, u_short port)
{
	sockaddr_in addr;
	addr.sin_family = AF_INET;
	inet_pton(AF_INET, ip, &addr.sin_addr);
	addr.sin_port = htons(port);
	if (bind(_socket, (SOCKADDR*)&addr, sizeof(addr)) == SOCKET_ERROR ||
		listen(_socket, 1) == SOCKET_ERROR)
	{
		logger.format(WSAGetLastError());
		return false;
	}
	return true;
}

bool Socket::Accept()
{
	SOCKET acceptSocket = accept(_socket, NULL, NULL);
	while (acceptSocket == SOCKET_ERROR)
		acceptSocket = accept(_socket, NULL, NULL);
	closesocket(_socket);
	_socket = acceptSocket;
	return true;
}

Socket Socket::MakeAccept()
{
	SOCKET acceptSocket = accept(_socket, NULL, NULL);
	while (acceptSocket == SOCKET_ERROR)
		acceptSocket = accept(_socket, NULL, NULL);
	return Socket(acceptSocket);
}

bool Socket::Connect(const char* ip, int port)
{
	sockaddr_in addr;
	addr.sin_family = AF_INET;
	inet_pton(AF_INET, ip, &addr.sin_addr);
	addr.sin_port = htons(port);
	if (connect(_socket, (SOCKADDR*)&addr, sizeof(addr)) == SOCKET_ERROR)
	{
		logger.format(WSAGetLastError());
		return false;
	}
	return true;
}