#pragma once
#include <fstream>
#include <string>

struct Properties;

struct Serialiser
{
	std::filebuf fb;

	Serialiser(const std::string& file, std::ios_base::openmode mode);
	static void Serialise(Properties props, std::ostream& os);
	static void Deserialise(Properties props, std::istream& is);
};
