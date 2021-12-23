#include "Server.h"

int main()
{
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != NO_ERROR)
    {
        std::wcout << "WSAStartup error!\n";
        WSACleanup();
        exit(10);
    }

    Server serv("0.0.0.0", 545454);
	system("pause > NUL");
	return 0;
}