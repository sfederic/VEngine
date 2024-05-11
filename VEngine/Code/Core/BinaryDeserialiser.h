#pragma once

#include <cstdio>
#include <string>

struct Properties;

class BinaryDeserialiser
{
public:
	FILE* file = nullptr;

	BinaryDeserialiser(const std::string filename)
	{
		fopen_s(&file, filename.c_str(), "rb");
	}

	~BinaryDeserialiser()
	{
		fclose(file);
	}

	void Deserialise(Properties& props);

	void ReadString(std::string* str);
	void ReadWString(std::wstring* wstr);

	template <typename T>
	void Read(T* value)
	{
		fread(value, sizeof(T), 1, file);
	}
};
