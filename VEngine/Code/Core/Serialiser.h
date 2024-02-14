#pragma once

#include <string>
#include <unordered_map>
#include <typeindex>
#include <fstream>
#include <functional>

struct Properties;
struct Property;

enum class OpenMode
{
	Out = std::ios_base::out,
	In = std::ios_base::in,
	Binary = std::ios_base::binary
};

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

struct BinaryDeserialiser
{
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

struct Serialiser
{
private:
	std::wofstream ofs;

	//Serialiser uses a stringstream as a temp buffer in memory before writing out to file with std::ofstream.
	//This way if there's a crash while serialising, partial save files won't be written out.
	std::wstringstream ss;

	const std::string filename;
	const OpenMode mode;

	std::unordered_map<std::type_index, std::function<void(Property& prop, std::wstring& name)>> typeToWriteFuncMap;

public:
	Serialiser(const std::string filename_, const OpenMode mode_);
	~Serialiser();
	void Serialise(Properties& props);

	template <typename T>
	void WriteLine(T arg)
	{
		ss << arg << L"\n";
	}
};

struct Deserialiser
{
	std::wifstream is;

	//@Todo: there's a bad bug here with std::wifstream where if the textual type of of the property doesn't match
	//(e.g. a bool value is '205') then the whole program will loop infinitely. Maybe find a way to catch this.
	std::unordered_map<std::type_index, std::function<void(Property& prop)>> typeToReadFuncMap;

	Deserialiser(const std::string filename, const OpenMode mode);
	~Deserialiser();
	void Deserialise(Properties& props);

	template <typename T>
	void ReadLine(T& arg)
	{
		is >> arg;
	}
};
