#pragma once
#include <fstream>
#include <string>
#include <sstream>

struct Properties;

enum class OpenMode
{
	Out = std::ios_base::out,
	In = std::ios_base::in,
	Binary = std::ios_base::binary
};

struct Serialiser
{
private:
	std::ofstream ofs;

	//Serialiser uses a stringstream as a temp buffer in memory before writing out to file with std::ofstream.
	//This way if there's a crash while serialising, partial save files won't be written out.
	std::stringstream ss;

	const std::string filename;
	const OpenMode mode;

public:
	Serialiser(const std::string filename_, const OpenMode mode_);
	~Serialiser();
	void Serialise(Properties props);

	template <typename T>
	void WriteLine(T arg)
	{
		ss << arg << "\n";
	}
};

struct Deserialiser
{
	std::ifstream is;

	Deserialiser(const std::string filename, const OpenMode mode);
	~Deserialiser();
	void Deserialise(Properties props);
};
