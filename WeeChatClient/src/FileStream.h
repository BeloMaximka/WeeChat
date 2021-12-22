#pragma once
#include <string>
#include <fstream>
class FileStream
{
	std::wstring path;
	std::fstream file;
public:
	FileStream() = delete;
	FileStream(std::wstring path);
	template <class input>
	FileStream& operator<<(input str);
};

FileStream::FileStream(std::wstring path) : path(path) {}

template<class input>
inline FileStream& FileStream::operator<<(input str)
{
	file.open(path, std::fstream::app);
	file << str;
	file.close;
}
