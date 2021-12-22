#include "ClientLogger.h"

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
	std::wfstream file("log.txt", std::fstream::app);
	file << "ERROR: " << str << std::endl;
	file.close();
	LocalFree(str);
}

Logger& Logger::operator<<(std::wstring str)
{
	std::wfstream file("log.txt", std::fstream::app);
	file << str;
	file.close();
	return *this;
}

Logger& Logger::operator<<(std::string str)
{
	std::wfstream file("log.txt", std::fstream::app);
	file << str;
	file.close();
	return *this;
}

Logger& Logger::operator<<(const char* str)
{
	std::wfstream file("log.txt", std::fstream::app);
	file << str;
	file.close();
	return *this;
}