#include "Server.h"

std::string getLocalIP()
{
    addrinfo* result = NULL;
    addrinfo hints;
    ZeroMemory(&hints, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;

    char name[256];
    gethostname(name, 256);
    getaddrinfo(name, "545454", &hints, &result);
    for (addrinfo* ptr = result; ptr != NULL; ptr = ptr->ai_next)
    {
        if (ptr->ai_next == NULL)
        {
            sockaddr_in* sockaddr_ipv4 = (sockaddr_in*)ptr->ai_addr;
            freeaddrinfo(result);
            char ip[32];
            inet_ntop(AF_INET, &sockaddr_ipv4->sin_addr, ip, 32);
            return ip;
        }
    }
    freeaddrinfo(result);
    return std::string();
}
int main()
{
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != NO_ERROR)
    {
        std::wcout << "WSAStartup error!\n";
        WSACleanup();
        exit(10);
    }

    Server serv(getLocalIP().c_str(), 545454);
	system("pause > NUL");
	return 0;
}