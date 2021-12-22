#include "Server.h"

Server::Server(const char* ip, unsigned short port)
{
	WSADATA wsaData;
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != NO_ERROR)
	{
		std::wcout << "ERROR: " << WSAerror_to_wstring(WSAGetLastError()) << std::endl;
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
	std::wcout << "Hosting server by IP " << ip << " and port " << std::to_wstring(port) << "...\n";
	base_socket.Bind(ip, port);
	std::wcout << "Binded.\n";
	while (threads_run)
	{
		Socket accepted = base_socket.MakeAccept();
		std::wcout << "Got connection.\n";
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

std::wstring Server::WSAerror_to_wstring(int code)
{
	WCHAR* str;
	WCHAR code_str[16];
	_itow_s(code, code_str, 10);
	FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
		NULL, WSAGetLastError(),
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(WCHAR*)&str, 0, NULL);
	std::wstring result(str);
	LocalFree(str);
	return result;
}

Server::~Server()
{
	threads_run = false;
	//listen.detach();
	//while (connections.size());
}
