#pragma once

#include <cstdio>
#include <string>

struct Properties;

struct BinarySerialiser
{
	FILE* file = nullptr;

	BinarySerialiser(const char* filename)
	{
		fopen_s(&file, filename, "wb");
	}

	~BinarySerialiser()
	{
		fclose(file);
	}

	void Serialise(Properties& props);
	void WriteString(const std::string str);
	void WriteWString(const std::wstring wstr);

	template <typename T>
	void Write(T* value)
	{
		fwrite(value, sizeof(T), 1, file);
	}
};
