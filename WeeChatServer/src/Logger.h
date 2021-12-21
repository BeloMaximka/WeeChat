#pragma once
#include <string>
#include <iostream>
#include <Windows.h>
#include <codecvt>

class Logger
{
public:
	Logger();

	void format(int err_code);
	Logger& operator<<(std::wstring str);
	Logger& operator<<(std::string str);
	Logger& operator<<(const char* str);
};

extern Logger logger;