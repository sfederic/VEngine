#pragma once

#include <fstream>
#include <string>
#include <sstream>
#include <unordered_map>
#include <typeindex>
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
	std::ofstream os;

	BinarySerialiser(const std::string filename)
	{
		os.open(filename, std::ios::binary | std::ios::out);
	}

	~BinarySerialiser()
	{
		os.flush();
		os.close();
	}

	void Serialise(Properties& props);
	void WriteString(const std::string str);
	void WriteWString(const std::wstring wstr);

	template <typename T>
	void Write(T* value)
	{
		os.write(reinterpret_cast<const char*>(value), sizeof(T));
	}
};

struct BinaryDeserialiser
{
	std::ifstream is;

	BinaryDeserialiser(const std::string filename)
	{
		is.open(filename, std::ios::binary | std::ios::in);
	}

	~BinaryDeserialiser()
	{
		is.close();
	}

	void Deserialise(Properties& props);

	void ReadString(std::string* str);
	void ReadWString(std::wstring* wstr);
	
	template <typename T>
	void Read(T* value)
	{
		is.read(reinterpret_cast<char*>(value), sizeof(T));
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

	std::unordered_map<std::type_index, std::function<void(Property& prop)>> typeToReadFuncMap;

	Deserialiser(const std::string filename, const OpenMode mode);
	~Deserialiser();
	void Deserialise(Properties& props);
};
