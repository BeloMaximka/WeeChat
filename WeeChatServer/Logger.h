#pragma once
#include <string>
#include <iostream>
#include <Windows.h>

class Logger
{
public:
	void format(int err_code);
	Logger& operator<<(std::wstring str);
} log;

