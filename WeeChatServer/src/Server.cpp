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
		connections[connection.get_id()].first.connection = std::move(connection);
	}
}

void Server::ProcessConnection(Socket _socket)
{
	auto id = std::this_thread::get_id();
	ClientInfo& infoClient = connections[id].second;
	connections[id].first.socket = &_socket;
	std::wstring message;
	while (threads_run && _socket.Recv(message))
	{
		if (message.substr(0, 8) == L"SET_DATA")
		{
			message = message.substr(8);
			infoClient.color = *(ULONG*)message.c_str();
			infoClient.name = message.substr(2);
			for (auto& elem : chatHistory)
				_socket.Send(elem.c_str(), elem.size());
			SendServerMessageFormatted(infoClient, L"%n has connected.");
			continue;
		}
		chatHistory.push_back(message);
		map_mutex.lock();
		for (auto& elem : connections)
			elem.second.first.socket->Send(message.c_str(), message.size());
		map_mutex.unlock();
	}
	SendServerMessageFormatted(infoClient, L"%n has disconnected.");
	map_mutex.lock();
	connections[id].first.connection.detach();
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

void Server::SendFormatted(ClientInfo info, std::wstring message)
{
	std::wstringstream msg;
	USHORT size = message.size() + info.name.size() + 5;
	msg << (wchar_t)*((wchar_t*)&size);
	msg << (wchar_t)*((wchar_t*)&info.color) << (wchar_t)*((wchar_t*)&info.color + 1);
	msg << info.name << L": " << message << L"\n";
	map_mutex.lock();
	for (auto& elem : connections)
		elem.second.first.socket->Send(msg.str().c_str(), msg.str().size());
	map_mutex.unlock();
}

void Server::SendServerMessageFormatted(ClientInfo info, std::wstring message)
{
	std::wstringstream msg;
	auto found = message.find(L"%n");
	message.replace(found, found + 1, info.name);
	USHORT size = message.size() + 3;
	msg << (wchar_t)*((wchar_t*)&size);
	msg << wchar_t(0) << wchar_t(0);
	msg << message << L"\n";
	map_mutex.lock();
	chatHistory.push_back(msg.str());
	for (auto& elem : connections)
		elem.second.first.socket->Send(msg.str().c_str(), msg.str().size());
	map_mutex.unlock();
}

Server::~Server()
{
	threads_run = false;
	//listen.detach();
	//while (connections.size());
}
