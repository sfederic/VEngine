#pragma once
#include <fstream>
#include <string>
#include <sstream>
#include <cassert>
#include "Properties.h"

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
		os.open(filename, std::ios_base::binary);
		if (os.fail()) throw;
	}

	~BinarySerialiser()
	{
		os.close();
	}

	void Serialise(Properties& props)
	{
		for (auto& propPair : props.propMap)
		{
			auto& prop = propPair.second;

			os.write((const char*)&prop.data, prop.size);
		}
	}

	void WriteLine(const std::string str)
	{
		size_t stringSize = str.size() + 1;
		os.write((const char*)&stringSize, sizeof(size_t));
		os.write(str.data(), stringSize);
	}

	template <typename T>
	void Write(T arg)
	{
		os.write((const char*)&arg, sizeof(T));
	}
};

struct BinaryDeserialiser
{
	std::ifstream is;

	BinaryDeserialiser(const std::string filename)
	{
		is.open(filename, std::ios_base::binary);
		if (is.fail()) throw;
	}

	~BinaryDeserialiser()
	{
		is.close();
	}

	void Deserialise(Properties& props)
	{
		for (auto& propPair : props.propMap)
		{
			auto& prop = propPair.second;

			is.read((char*)&prop.data, prop.size);
		}
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

public:
	Serialiser(const std::string filename_, const OpenMode mode_);
	~Serialiser();
	void Serialise(Properties& props);

	template <typename T>
	void WriteLine(T arg)
	{
		ss << arg << "\n";
	}
};

struct Deserialiser
{
	std::wifstream is;

	Deserialiser(const std::string filename, const OpenMode mode);
	~Deserialiser();
	void Deserialise(Properties& props);
};
