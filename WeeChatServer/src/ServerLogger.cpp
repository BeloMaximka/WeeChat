#include "ServerLogger.h"

Logger logger;

Logger::Logger()
{
	setlocale(0, "");
}

void Logger::format(int err_code)
{
	wchar_t* str;
	int code = WSAGetLastError();
	wchar_t code_str[16];
	_itow_s(code, code_str, 10);
	FormatMessageW(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
		NULL, WSAGetLastError(),
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPWSTR)&str, 0, NULL);
	std::wcout << "ERROR: " << str << std::endl;
	LocalFree(str);
}

Logger& Logger::operator<<(std::wstring str)
{
	std::wcout << str;
	return *this;
}

Logger& Logger::operator<<(std::string str)
{
	std::cout << str;
	return *this;
}

Logger& Logger::operator<<(const char* str)
{
	std::cout << str;
	return *this;
}
