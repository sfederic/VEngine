#pragma once
#include <fstream>
#include <string>

struct Properties;

enum class OpenMode
{
	Out = std::ios_base::out,
	In = std::ios_base::in,
	Binary = std::ios_base::binary
};

struct Serialiser
{
	std::ofstream os;

	Serialiser(const std::string filename, const OpenMode mode);
	~Serialiser();
	void Serialise(Properties props);

	template <typename T>
	void WriteLine(T arg)
	{
		os << arg << "\n";
	}
};

struct Deserialiser
{
	std::ifstream is;

	Deserialiser(const std::string filename, const OpenMode mode);
	~Deserialiser();
	void Deserialise(Properties props);
};
